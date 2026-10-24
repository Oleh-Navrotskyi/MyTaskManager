#pragma once

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <msclr/marshal_cppstd.h>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "advapi32.lib")

using namespace System;
using namespace System::Collections::Generic;

namespace OSLab2 {

    //Модель процеса
    public ref class ProcessInfo {
    public:
        int PID;
        String^ Name;
        double CpuUsage;
        String^ Priority;
        String^ State;
        long long WsMiB;
        long long VaMiB;
        long long PrivateMiB;
        int Threads;
        String^ Owner;
        String^ Path;
    };

    //Модель потоку
    public ref class ThreadInfo {
    public:
        int TID;
        int OwnerPID;
        int BasePriority;
        double CpuUsage;
        String^ State;
        String^ Affinity;
    };

    //Час виконання процесу
    private ref class ProcessTimeRecord {
    public:
        long long LastSystemCounter;
        unsigned long long LastProcessTime;
    };

    //Час виконання потоку
    private ref class ThreadTimeRecord {
    public:
        long long LastSystemCounter;
        unsigned long long LastThreadTime;
    };

    //Клас для роботи з процесами та їх потоками
    public ref class ProcessHelper {
        // Кеш для збереження станів ("Призупинено цим диспетчером", "Частково призупинено") та CPU %
    private:
        static Dictionary<int, ProcessTimeRecord^>^ cpuTracker = gcnew Dictionary<int, ProcessTimeRecord^>();
        static Dictionary<int, ThreadTimeRecord^>^ threadCpuTracker = gcnew Dictionary<int, ThreadTimeRecord^>();
        static Dictionary<int, String^>^ ProcessStateCache = gcnew Dictionary<int, String^>();
        static Dictionary<int, Dictionary<int, ThreadInfo^>^>^ ProcessThreadsHistory =
            gcnew Dictionary<int, Dictionary<int, ThreadInfo^>^>();
        static Dictionary<int, ProcessInfo^>^ CachedProcesses = gcnew Dictionary<int, ProcessInfo^>();
        static List<int>^ TrackedPids = gcnew List<int>();

        //Функція отримання поточного часу
        static unsigned long long GetTimeFromFILETIME(const FILETIME& ft) {
            ULARGE_INTEGER uli;
            uli.LowPart = ft.dwLowDateTime;
            uli.HighPart = ft.dwHighDateTime;
            return uli.QuadPart;
        }

        //Функція отримання віртуальної пам'яті процесу
        static long long GetVirtualMemory(HANDLE hProcess) {
            long long totalVA = 0;
            MEMORY_BASIC_INFORMATION mbi;
            unsigned char* addr = 0;
            while (VirtualQueryEx(hProcess, addr, &mbi, sizeof(mbi)) == sizeof(mbi)) {
                if (mbi.State == MEM_COMMIT || mbi.State == MEM_RESERVE) {
                    totalVA += mbi.RegionSize;
                }
                addr += mbi.RegionSize;
            }
            return totalVA;
        }

        //Функція знаходження батькового процесу
        static String^ GetProcessOwner(HANDLE hProcess) {
            String^ ownerName = "N/A";
            HANDLE hToken = NULL;
            if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                DWORD dwSize = 0;
                GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
                if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                    PTOKEN_USER pTokenUser = (PTOKEN_USER)malloc(dwSize);
                    if (pTokenUser != NULL) {
                        if (GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize)) {
                            SID_NAME_USE sidType;
                            wchar_t name[256];
                            DWORD nameSize = 256;
                            wchar_t domain[256];
                            DWORD domainSize = 256;
                            if (LookupAccountSidW(NULL, pTokenUser->User.Sid, name, &nameSize, domain, &domainSize, &sidType)) {
                                ownerName = gcnew String(name);
                            }
                        }
                        free(pTokenUser);
                    }
                }
                CloseHandle(hToken);
            }
            return ownerName;
        }

        //Функція перевірки стану процесу
        static void EvaluateProcessState(int pid) {
            HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (hThreadSnap == INVALID_HANDLE_VALUE) return;

            THREADENTRY32 te32;
            te32.dwSize = sizeof(THREADENTRY32);
            int total = 0, suspended = 0;

            if (Thread32First(hThreadSnap, &te32)) {
                do {
                    if (te32.th32OwnerProcessID == pid) {
                        total++;
                        HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION | THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                        if (hThread) {
                            // Трюк із Suspend/Resume для зчитування стану
                            DWORD sc = SuspendThread(hThread); 
                            if (sc != (DWORD)-1) {
                                ResumeThread(hThread); 
                                if (sc > 0) suspended++; // Якщо був призупинений до нашої перевірки
                            }
                            CloseHandle(hThread);
                        }
                    }
                } while (Thread32Next(hThreadSnap, &te32)); 
            }
            CloseHandle(hThreadSnap);

            // Точна оцінка стану
            if (total > 0) {
                if (suspended == 0) ProcessStateCache[pid] = "Active";
                else if (suspended == total) ProcessStateCache[pid] = "Suspended by Dispatcher";
                else ProcessStateCache[pid] = "Partially Suspended";
            }
        }

    public:
        //Функція отримання кількості логічних ядер
        static int GetLogicalProcessorCount() {
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            return sysInfo.dwNumberOfProcessors;
        }

        // Функція отримання всіх процесів
        static List<ProcessInfo^>^ GetAllProcesses(int filterPid, int selectedPid) {
            List<ProcessInfo^>^ list = gcnew List<ProcessInfo^>();
            List<int>^ livePids = gcnew List<int>(); // Список живих PID

            HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hProcessSnap == INVALID_HANDLE_VALUE) return list;

            PROCESSENTRY32W pe32;
            pe32.dwSize = sizeof(PROCESSENTRY32W);

            if (!Process32FirstW(hProcessSnap, &pe32)) {
                CloseHandle(hProcessSnap);
                return list;
            }

            int L = GetLogicalProcessorCount();
            LARGE_INTEGER perfCounter, perfFreq;
            QueryPerformanceCounter(&perfCounter);
            QueryPerformanceFrequency(&perfFreq);

            do {
                livePids->Add(pe32.th32ProcessID);

                if (filterPid != -1 && pe32.th32ProcessID != filterPid) {
                    continue;
                }

                ProcessInfo^ pi = gcnew ProcessInfo();
                pi->PID = pe32.th32ProcessID;
                pi->Name = gcnew String(pe32.szExeFile);
                pi->Threads = pe32.cntThreads;
                pi->WsMiB = 0; pi->VaMiB = 0; pi->PrivateMiB = 0;
                pi->CpuUsage = 0.0;
                pi->Priority = "Unknown";
                pi->Owner = "N/A";
                pi->Path = "N/A";

                pi->State = ProcessStateCache->ContainsKey(pi->PID) ? ProcessStateCache[pi->PID] : "Active";

                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
                if (!hProcess) hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe32.th32ProcessID);

                if (hProcess != NULL) {
                    pi->Owner = GetProcessOwner(hProcess);
                    wchar_t pathBuf[MAX_PATH];
                    DWORD pathSize = MAX_PATH;
                    if (QueryFullProcessImageNameW(hProcess, 0, pathBuf, &pathSize)) pi->Path = gcnew String(pathBuf);

                    PROCESS_MEMORY_COUNTERS_EX pmcx;
                    if (GetProcessMemoryInfo(hProcess, (PROCESS_MEMORY_COUNTERS*)&pmcx, sizeof(pmcx))) {
                        pi->WsMiB = pmcx.WorkingSetSize / (1024 * 1024);
                        pi->PrivateMiB = pmcx.PrivateUsage / (1024 * 1024);
                    }

                    pi->VaMiB = GetVirtualMemory(hProcess) / (1024 * 1024);
                    DWORD priorityClass = GetPriorityClass(hProcess);
                    switch (priorityClass) {
                    case IDLE_PRIORITY_CLASS: pi->Priority = "Idle"; break;
                    case BELOW_NORMAL_PRIORITY_CLASS: pi->Priority = "Below Normal"; break;
                    case NORMAL_PRIORITY_CLASS: pi->Priority = "Normal"; break;
                    case ABOVE_NORMAL_PRIORITY_CLASS: pi->Priority = "Above Normal"; break;
                    case HIGH_PRIORITY_CLASS: pi->Priority = "High"; break;
                    }

                    FILETIME ftCreation, ftExit, ftKernel, ftUser;
                    if (GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel, &ftUser)) {
                        unsigned long long processTime = GetTimeFromFILETIME(ftKernel) + GetTimeFromFILETIME(ftUser);
                        if (cpuTracker->ContainsKey(pi->PID)) {
                            ProcessTimeRecord^ record = cpuTracker[pi->PID];
                            double deltaProcSec = (processTime - record->LastProcessTime) * 1e-7;
                            double deltaSysSec = (double)(perfCounter.QuadPart - record->LastSystemCounter) / perfFreq.QuadPart;
                            if (deltaSysSec > 0) pi->CpuUsage = 100.0 * deltaProcSec / (deltaSysSec * L);
                            record->LastSystemCounter = perfCounter.QuadPart;
                            record->LastProcessTime = processTime;
                        }
                        else {
                            ProcessTimeRecord^ record = gcnew ProcessTimeRecord();
                            record->LastSystemCounter = perfCounter.QuadPart;
                            record->LastProcessTime = processTime;
                            cpuTracker[pi->PID] = record;
                        }
                    }
                    CloseHandle(hProcess);
                }

                CachedProcesses[pi->PID] = pi; // Зберігаємо копію в кеш
                list->Add(pi);
            } while (Process32NextW(hProcessSnap, &pe32));

            CloseHandle(hProcessSnap);

            // --- ПЕРЕВІРКА ЗАВЕРШЕНИХ ПРОЦЕСІВ ---
            List<int>^ pidsToCheck = gcnew List<int>();
            if (selectedPid != -1) pidsToCheck->Add(selectedPid);
            for each(int p in TrackedPids) {
                if (!pidsToCheck->Contains(p)) pidsToCheck->Add(p);
            }

            for each(int p in pidsToCheck) {
                // Якщо процес був важливим (виділений/запущений), але його немає в знімку:
                if (!livePids->Contains(p) && CachedProcesses->ContainsKey(p)) {
                    if (filterPid != -1 && p != filterPid) continue; // Якщо увімкнено "Тільки вибраний"

                    ProcessInfo^ deadPi = CachedProcesses[p];
                    deadPi->State = "Terminated"; //[cite: 13]
                    deadPi->CpuUsage = 0.0;
                    deadPi->Threads = 0;
                    deadPi->WsMiB = 0;
                    deadPi->VaMiB = 0;
                    deadPi->PrivateMiB = 0;

                    bool exists = false;
                    for each(ProcessInfo ^ existing in list) {
                        if (existing->PID == p) { exists = true; break; }
                    }
                    if (!exists) list->Add(deadPi); // Додаємо мертвий процес у таблицю
                }
            }

            return list;
        }

        // Перевантажена функція отримання всіх процесів
        static List<ProcessInfo^>^ GetAllProcesses() {
            return GetAllProcesses(-1, -1);
        }

        //Бітове представлення шістнадцяткового числа у десятковому вигляді
        static String^ FormatAffinity(DWORD_PTR mask, int logicalCores) {
            DWORD_PTR fullMask = (1ULL << logicalCores) - 1; // Максимальна маска для всіх доступних ядер
            if (mask == fullMask) return "All";

            List<String^>^ cores = gcnew List<String^>();
            for (int i = 0; i < logicalCores; i++) {
                if ((mask & (1ULL << i)) != 0) {
                    cores->Add(i.ToString());
                }
            }
            return String::Join(", ", cores);
        }

        // --- Запуск процесу з аргументами ---
        // --- Запуск процесу з аргументами ---
        static bool LaunchProcessWithArgs(String^ exePath, String^ args, String^ workDir, bool createSuspended, bool isConsole, int% outPid, DWORD% outError) {
            msclr::interop::marshal_context context;
            LPCWSTR nativeExe = context.marshal_as<const wchar_t*>(exePath);

            // Формуємо рядок коректно: якщо аргументів немає, передаємо лише шлях
            String^ fullCmd = String::IsNullOrWhiteSpace(args) ?
                String::Format(L"\"{0}\"", exePath) :
                String::Format(L"\"{0}\" {1}", exePath, args);
            std::wstring cmdStr = context.marshal_as<std::wstring>(fullCmd);

            // Встановлюємо робочий каталог!
            LPCWSTR nativeWorkDir = String::IsNullOrWhiteSpace(workDir) ? NULL : context.marshal_as<const wchar_t*>(workDir);

            STARTUPINFOW si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));

            DWORD flags = 0;
            if (createSuspended) flags |= CREATE_SUSPENDED;
            if (isConsole) flags |= CREATE_NEW_CONSOLE;

            BOOL success = CreateProcessW(nativeExe, &cmdStr[0], NULL, NULL, FALSE, flags, NULL, nativeWorkDir, &si, &pi);

            if (success) {
                outPid = pi.dwProcessId;
                TrackedPids->Add(outPid);
                // Якщо створено призупиненим, одразу записуємо правильний стан у кеш!
                if (createSuspended) {
                    ProcessStateCache[outPid] = "Suspended by Dispatcher";
                }

                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
                return true;
            }
            outError = GetLastError();
            return false;
        }

        // --- Операції над процесами (Terminate) ---
        static bool TerminateProcessByPid(int pid, DWORD% errCode) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
            if (hProcess != NULL) {
                BOOL result = TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
                if (result) {
                    ProcessStateCache[pid] = "Terminated"; 
                    TrackedPids->Add(pid);
                    return true;
                }
            }
            errCode = GetLastError();
            return false;
        }

        // --- Операції над процесами (Suspend) ---
        static String^ SuspendProcess(int pid) {
            HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (hThreadSnap == INVALID_HANDLE_VALUE) return "Error snapshot";

            THREADENTRY32 te32;
            te32.dwSize = sizeof(THREADENTRY32);
            int total = 0, suspendedNow = 0, alreadySuspended = 0, errors = 0;

            if (Thread32First(hThreadSnap, &te32)) {
                do {
                    if (te32.th32OwnerProcessID == pid) {
                        total++;
                        HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                        if (hThread) {
                            DWORD prev = SuspendThread(hThread);
                            if (prev == (DWORD)-1) errors++;
                            else if (prev == 0) suspendedNow++;
                            else alreadySuspended++;
                            CloseHandle(hThread);
                        }
                        else {
                            errors++;
                        }
                    }
                } while (Thread32Next(hThreadSnap, &te32));
            }
            CloseHandle(hThreadSnap);

            int totalSuspended = suspendedNow + alreadySuspended;
            String^ newState;
            if (totalSuspended == 0) newState = "Active";
            else if (totalSuspended == total && errors == 0) newState = "Suspended by Dispatcher";
            else newState = "Partially Suspended"; 

            ProcessStateCache[pid] = newState;
            if (suspendedNow == 0) return "No new threads suspended";
            return String::Format("Success: {0}/{1} threads suspended", suspendedNow, total);
        }

        // --- Операції над процесами (Resume) ---
        static String^ ResumeProcess(int pid) {
            HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (hThreadSnap == INVALID_HANDLE_VALUE) return "Error snapshot";
            THREADENTRY32 te32;
            te32.dwSize = sizeof(THREADENTRY32);
            int resumedNow = 0;
            if (Thread32First(hThreadSnap, &te32)) {
                do {
                    if (te32.th32OwnerProcessID == pid) {
                        HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID); 
                        if (hThread) {
                            DWORD prev = ResumeThread(hThread); 
                            if (prev > 0 && prev != (DWORD)-1) resumedNow++;
                            CloseHandle(hThread);
                        }
                    }
                } while (Thread32Next(hThreadSnap, &te32));
            }
            CloseHandle(hThreadSnap);
            EvaluateProcessState(pid);
            return String::Format("Resumed {0} threads", resumedNow);
        }

        // --- Операції над процесами (Зміна пріоритету) ---
        static bool SetProcessPriority(int pid, DWORD priorityClass) {
            HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);
            if (hProcess) {
                BOOL result = SetPriorityClass(hProcess, priorityClass);
                CloseHandle(hProcess);
                return result != 0;
            }
            return false;
        }

        // --- Операції над процесами (Отримати доступні для виконання ядра) ---
        static bool GetAffinity(int pid, DWORD_PTR% processMask, DWORD_PTR% systemMask) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
            if (hProcess) {
                DWORD_PTR pMask, sMask;
                if (GetProcessAffinityMask(hProcess, &pMask, &sMask)) {
                    processMask = pMask; systemMask = sMask;
                    CloseHandle(hProcess);
                    return true;
                }
                CloseHandle(hProcess);
            }
            return false;
        }

        // --- Операції над процесами (Змінити доступні для виконання ядра) ---
        static bool SetAffinity(int pid, DWORD_PTR processMask) {
            HANDLE hProcess = OpenProcess(PROCESS_SET_INFORMATION, FALSE, pid);
            if (hProcess) {
                BOOL result = SetProcessAffinityMask(hProcess, processMask);
                CloseHandle(hProcess);
                return result != 0;
            }
            return false;
        }

        // --- Операції над потоками (Отримати потоки процесу)---
        static List<ThreadInfo^>^ GetProcessThreads(int pid) {
            if (!ProcessThreadsHistory->ContainsKey(pid)) {
                ProcessThreadsHistory[pid] = gcnew Dictionary<int, ThreadInfo^>();
            }
            Dictionary<int, ThreadInfo^>^ history = ProcessThreadsHistory[pid];

            // Заміна HashSet на звичайний List для сумісності з будь-якими збірками C++
            List<int>^ liveTids = gcnew List<int>();

            HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (hThreadSnap != INVALID_HANDLE_VALUE) {
                int L = GetLogicalProcessorCount();
                LARGE_INTEGER perfCounter, perfFreq;
                QueryPerformanceCounter(&perfCounter);
                QueryPerformanceFrequency(&perfFreq);

                THREADENTRY32 te32;
                te32.dwSize = sizeof(THREADENTRY32);

                if (Thread32First(hThreadSnap, &te32)) {
                    do {
                        if (te32.th32OwnerProcessID == pid) {
                            liveTids->Add(te32.th32ThreadID);

                            ThreadInfo^ ti;
                            if (history->ContainsKey(te32.th32ThreadID)) {
                                ti = history[te32.th32ThreadID];
                            }
                            else {
                                ti = gcnew ThreadInfo();
                                ti->TID = te32.th32ThreadID;
                                ti->OwnerPID = te32.th32OwnerProcessID;
                                history[ti->TID] = ti;
                            }

                            ti->BasePriority = te32.tpBasePri;
                            ti->State = "Active";

                            HANDLE hThread = OpenThread(THREAD_QUERY_INFORMATION | THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                            if (hThread) {
                                GROUP_AFFINITY groupAffinity;
                                if (GetThreadGroupAffinity(hThread, &groupAffinity)) {
                                    ti->Affinity = FormatAffinity(groupAffinity.Mask, L);
                                }
                                else {
                                    ti->Affinity = "Unknown";
                                }

                                DWORD suspendCount = SuspendThread(hThread);
                                if (suspendCount != (DWORD)-1) {
                                    ResumeThread(hThread);
                                    if (suspendCount > 0) ti->State = "Suspended";
                                }

                                FILETIME ftCreation, ftExit, ftKernel, ftUser;
                                if (GetThreadTimes(hThread, &ftCreation, &ftExit, &ftKernel, &ftUser)) {
                                    unsigned long long threadTime = GetTimeFromFILETIME(ftKernel) + GetTimeFromFILETIME(ftUser);
                                    if (threadCpuTracker->ContainsKey(ti->TID)) {
                                        ThreadTimeRecord^ rec = threadCpuTracker[ti->TID];
                                        double dProc = (threadTime - rec->LastThreadTime) * 1e-7;
                                        double dSys = (double)(perfCounter.QuadPart - rec->LastSystemCounter) / perfFreq.QuadPart;
                                        if (dSys > 0) ti->CpuUsage = 100.0 * dProc / (dSys * L);
                                        rec->LastSystemCounter = perfCounter.QuadPart;
                                        rec->LastThreadTime = threadTime;
                                    }
                                    else {
                                        ThreadTimeRecord^ rec = gcnew ThreadTimeRecord();
                                        rec->LastSystemCounter = perfCounter.QuadPart;
                                        rec->LastThreadTime = threadTime;
                                        threadCpuTracker[ti->TID] = rec;
                                    }
                                }
                                CloseHandle(hThread);
                            }
                        }
                    } while (Thread32Next(hThreadSnap, &te32));
                }
                CloseHandle(hThreadSnap);
            }

            for each(KeyValuePair<int, ThreadInfo^> ^ kvp in history) {
                if (!liveTids->Contains(kvp->Key)) {
                    kvp->Value->State = "Terminated";
                    kvp->Value->CpuUsage = 0.0;
                }
            }

            List<ThreadInfo^>^ resultList = gcnew List<ThreadInfo^>();
            for each(ThreadInfo ^ ti in history->Values) {
                resultList->Add(ti);
            }
            return resultList;
        }

        // --- Операції над потоками (Змінити пріоритет потоку)---
        static bool SetThreadPriorityLevel(int tid, int priority) {
            HANDLE hThread = OpenThread(THREAD_SET_INFORMATION, FALSE, tid);
            if (hThread) {
                BOOL res = SetThreadPriority(hThread, priority);
                CloseHandle(hThread);
                return res != 0;
            }
            return false;
        }

        // --- Операції над потоками (Змінити афініті потоку)---
        static bool SetThreadAffinity(int tid, DWORD_PTR threadMask) {
            HANDLE hThread = OpenThread(THREAD_SET_INFORMATION | THREAD_QUERY_INFORMATION, FALSE, tid);
            if (hThread != NULL) {
                DWORD_PTR result = SetThreadAffinityMask(hThread, threadMask);
                CloseHandle(hThread);
                return result != 0;
            }
            return false;
        }

        // --- Операції над потоками (Призупинити потік)---
        static String^ SuspendSingleThread(int tid, int pid) {
            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, tid);
            if (hThread) {
                DWORD prev = SuspendThread(hThread);
                CloseHandle(hThread);

                EvaluateProcessState(pid); // <--- Оновлюємо кеш правильно!

                if (prev == (DWORD)-1) return "Error";
                if (prev > 0) return "Already suspended";
                return "Suspended";
            }
            return "Access denied";
        }

        // --- Операції над потоками (Відновити потік)---
        static String^ ResumeSingleThread(int tid, int pid) {
            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, tid);
            if (hThread) {
                DWORD prev = ResumeThread(hThread);
                CloseHandle(hThread);

                EvaluateProcessState(pid); // <--- Оновлюємо кеш правильно!

                if (prev == 0) return "Was not suspended";
                return "Resumed";
            }
            return "Access denied";
        }

        // --- Операції над потоками (Видалити потік)---
        static bool TerminateSingleThread(int tid, DWORD% err) {
            HANDLE hThread = OpenThread(THREAD_TERMINATE, FALSE, tid);
            if (hThread) {
                BOOL res = TerminateThread(hThread, 0); 
                CloseHandle(hThread);
                if (res) return true;
            }
            err = GetLastError();
            return false;
        }
    };
}