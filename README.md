# Custom Windows Task Manager (MyTaskManager)

Advanced process and thread manager for Windows, developed using **C++/CLI (Windows Forms)** for the UI and **Native C++ (Win32 API)** for system interactions. This project was built as a comprehensive Operating Systems laboratory work, moving beyond standard abstractions to interact directly with the Windows kernel via `Toolhelp32`, `PSAPI`, and `Advapi32`.

## 🚀 Features

### Process Management
*   **Real-time Monitoring:** Tracks PID, Process Name, CPU Usage (%), Physical/Virtual/Private Memory (MiB), Thread Count, Owner (SID/Name), and Executable Path.
*   **Lifecycle Control:** Safely Suspend, Resume, or Terminate processes.
*   **Execution States:** Accurately detects and caches states like *Active*, *Suspended by Dispatcher*, *Partially Suspended*, and *Terminated*.
*   **Process Launcher:** Start new executables with custom arguments, working directories, console allocation, or in a `CREATE_SUSPENDED` state.
*   **Priority & Affinity:** Modify process base priority and bind execution to specific CPU cores.

### Thread Management
*   **Deep Dive:** Inspect individual threads of any selected process.
*   **Thread Metrics:** Tracks TID, Owner PID, Thread CPU Usage (%), Base Priority, State, and Core Affinity.
*   **Thread Control:** Suspend, Resume, Terminate, or change Priority/Affinity for specific threads independently of their parent process.

### UI & UX Features
*   **Smart Filtering:** Search by process name, isolate a specific PID, or hide critical Windows system processes (filters out `SYSTEM`, `LOCAL SERVICE`, etc., to prevent accidental system crashes).
*   **Operation Log:** Detailed, time-stamped history of all user actions and API responses (success/error codes).
*   **Dynamic Sorting & Auto-scroll:** Clickable column headers for programmatic sorting that persists through auto-updates.

## 🛠️ Tech Stack

*   **Language:** C++, C++/CLI
*   **Framework:** .NET Framework (Windows Forms)
*   **System API:** Win32 API (`windows.h`, `tlhelp32.h`, `psapi.h`, `advapi32.lib`)
*   **IDE:** Visual Studio

## 📁 Architecture

The project strictly separates the UI layer from the system logic:
*   `Form_OS_Lab2` & `AffinityForm`: Managed C++/CLI classes handling the visual representation, timers, and user events.
*   `ProcessHelper.h`: A static native/managed wrapper class that handles all WinAPI calls. It caches CPU times via `QueryPerformanceCounter` and `GetProcessTimes` to calculate accurate CPU loads without freezing the UI.

## ⚙️ How to Build and Run

1. Open the `.sln` file in **Visual Studio**.
2. Ensure you have the **Desktop development with C++** and **C++/CLI support** workloads installed.
3. Set the build configuration to `Release` and architecture to `x64`.
4. Build the solution (`Ctrl + Shift + B`).
5. Run the executable as Administrator (recommended for full access to memory metrics and owner SIDs of other users' processes).

---
*Developed as part of the Operating Systems course (Lab 2).*
