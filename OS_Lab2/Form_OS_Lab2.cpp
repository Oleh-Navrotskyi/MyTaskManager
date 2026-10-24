#include "Form_OS_Lab2.h"
#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#include "ProcessHelper.h"
#include <msclr/marshal_cppstd.h>

using namespace System::Windows::Forms;


namespace OSLab2 {

	//
	//==Function Buttons==
	//
	
	//Оновлення списку процесів
	void Form_OS_Lab2::RefreshProcessList() {
		int selectedPid = -1;

		// Перевіряємо, чи є запит на примусове виділення нового процесу
		if (pidToSelect != -1) {
			selectedPid = pidToSelect;
			pidToSelect = -1; // Скидаємо після застосування
		}
		else if (dgvProcesses->SelectedRows->Count > 0) {
			selectedPid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
		}

		// Оптимізація: якщо увімкнено "Only selected PID", просимо Helper шукати тільки його
		int optimizePid = (chkSelectedPidOnly->Checked && selectedPid != -1) ? selectedPid : -1;
		List<ProcessInfo^>^ processes = ProcessHelper::GetAllProcesses(optimizePid, selectedPid);

		lblTotalProcesses->Text = "Total Processes: " + processes->Count.ToString();
		dgvProcesses->Rows->Clear();
		DataGridViewRow^ rowToScroll = nullptr;

		for each(ProcessInfo ^ pi in processes) {
			// 1. Фільтр: Лише обраний PID
			if (chkSelectedPidOnly->Checked && selectedPid != -1 && pi->PID != selectedPid) {
				continue;
			}

			// 2. Фільтр: Приховати системні
			if (chkHideSystem->Checked) {
				if (pi->PID == 0 || pi->PID == 4) continue; // Системні PID[cite: 14]

				String^ ownerUp = pi->Owner->ToUpper();
				String^ nameUp = pi->Name->ToUpper();

				// Перевірка за власником[cite: 14]
				if (ownerUp->Contains("SYSTEM") || ownerUp->Contains("СИСТЕМА") ||
					ownerUp->Contains("NETWORK SERVICE") || ownerUp->Contains("LOCAL SERVICE")) {
					continue;
				}

				// Додаткова перевірка за назвою для захищених процесів, власник яких "N/A"
				if (nameUp == "SECURE SYSTEM" || nameUp == "REGISTRY" || nameUp == "SMSS.EXE" || nameUp == "CSRSS.EXE") {
					continue;
				}
			}

			int rowIndex = dgvProcesses->Rows->Add();
			DataGridViewRow^ row = dgvProcesses->Rows[rowIndex];

			row->Cells["colPid"]->Value = pi->PID;
			row->Cells["colName"]->Value = pi->Name;
			row->Cells["colCpu"]->Value = Math::Round(pi->CpuUsage, 1); // Записуємо як число для коректного сортування
			row->Cells["colPriority"]->Value = pi->Priority;
			row->Cells["colWs"]->Value = pi->WsMiB;
			row->Cells["colVa"]->Value = pi->VaMiB;
			row->Cells["colPrivate"]->Value = pi->PrivateMiB;
			row->Cells["colThreads"]->Value = pi->Threads;
			row->Cells["colOwner"]->Value = pi->Owner;
			row->Cells["colPath"]->Value = pi->Path;
			row->Cells["colState"]->Value = pi->State;

			// Візуальне підсвічування статусів для процесів
			if (pi->State == "Terminated") {
				row->DefaultCellStyle->ForeColor = System::Drawing::Color::Gray;
			}
			else if (pi->State == "Suspended by Dispatcher" || pi->State == "Partially Suspended") {
				row->DefaultCellStyle->ForeColor = System::Drawing::Color::OrangeRed;
			}
			else {
				row->DefaultCellStyle->ForeColor = System::Drawing::Color::Black;
			}

			// Зберігаємо посилання на рядок, якщо він був виділений
			if (pi->PID == selectedPid) {
				row->Selected = true;
				rowToScroll = row;
			}
		}

		// Застосовуємо сортування після заповнення таблиці
		if (sortColIndex >= 0 && sortColIndex < dgvProcesses->Columns->Count) {
			dgvProcesses->Sort(dgvProcesses->Columns[sortColIndex], sortDirection);
		}

		// Автоматична прокрутка до виділеного елемента
		if (rowToScroll != nullptr && !rowToScroll->Displayed) {
			dgvProcesses->FirstDisplayedScrollingRowIndex = rowToScroll->Index;
		}

		lblVisibleProcesses->Text = "Visible: " + dgvProcesses->Rows->Count.ToString();
		lblLogicalCpu->Text = "Logical CPU: " + ProcessHelper::GetLogicalProcessorCount().ToString();
		if (selectedPid == -1) {
			dgvProcesses->ClearSelection();
		}
	}

	//Оновлення списку потоків
	void Form_OS_Lab2::RefreshThreadList(int pid) {
		int selectedTid = -1;
		if (dgvThreads->SelectedRows->Count > 0) {
			selectedTid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
		}

		List<ThreadInfo^>^ threads = ProcessHelper::GetProcessThreads(pid);

		dgvThreads->Rows->Clear();
		DataGridViewRow^ rowToScroll = nullptr;

		for each (ThreadInfo ^ ti in threads) {
			int rowIndex = dgvThreads->Rows->Add();
			DataGridViewRow^ row = dgvThreads->Rows[rowIndex];

			row->Cells["colTid"]->Value = ti->TID;
			row->Cells["colOwnerPid"]->Value = ti->OwnerPID;
			row->Cells["colThreadCpu"]->Value = Math::Round(ti->CpuUsage, 1);
			row->Cells["colThreadPriority"]->Value = ti->BasePriority;
			row->Cells["colThreadState"]->Value = ti->State;
			row->Cells["colAffinity"]->Value = ti->Affinity;

			if (ti->TID == selectedTid) {
				row->Selected = true;
				rowToScroll = row;
			}
		}

		if (sortColIndex >= 0 && sortColIndex < dgvThreads->Columns->Count) {
			dgvThreads->Sort(dgvThreads->Columns[sortColIndex], sortDirection);
		}

		if (rowToScroll != nullptr && !rowToScroll->Displayed) {
			dgvThreads->FirstDisplayedScrollingRowIndex = rowToScroll->Index;
		}
		if (selectedTid == -1) {
			dgvThreads->ClearSelection();
		}
	}

	//Прискорення для дослідження
	System::Void Form_OS_Lab2::chkSelectedPidOnly_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		if (chkSelectedPidOnly->Checked) {
			timerRefresh->Interval = 100; // Прискорення до 100 мс
		}
		else {
			timerRefresh->Interval = Convert::ToInt32(numInterval->Value); // Повертаємо базовий інтервал
		}
		RefreshProcessList();
	}

	//Ентер активує пошук
	System::Void Form_OS_Lab2::Search_Process_textBox_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		// Якщо натиснуто Enter
		if (e->KeyCode == System::Windows::Forms::Keys::Enter) {
			search_button_Click(sender, e); // Викликаємо існуючий метод пошуку

			// Ці два рядки запобігають звуковому сигналу "ding" від Windows
			e->Handled = true;
			e->SuppressKeyPress = true;
		}
	}

	//Запуск процесу
	System::Void Form_OS_Lab2::btnLaunch_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
		openFileDialog->Filter = "Executables (*.exe)|*.exe|All files (*.*)|*.*";

		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			String^ filePath = openFileDialog->FileName;
			String^ args = args_textBox->Text;
			bool createSuspended = chkSuspended->Checked;
			bool isConsole = ArgsCheckBox->Checked;

			// ВАЖЛИВО: Витягуємо папку, в якій лежить .exe, щоб зробити її робочим каталогом
			String^ workDir = System::IO::Path::GetDirectoryName(filePath);

			int newPid = 0;
			DWORD err = 0;

			bool success = ProcessHelper::LaunchProcessWithArgs(filePath, args, workDir, createSuspended, isConsole, newPid, err);

			if (success) {
				rbtLog->AppendText(String::Format("[{0}] Launch | PID {1} | Success\n", DateTime::Now.ToLongTimeString(), newPid));
				pidToSelect = newPid;
				RefreshProcessList();
			}
			else {
				// Тепер форма обов'язково покаже, чому процес не запустився!
				MessageBox::Show("Failed to launch the process. Error Code: " + err.ToString(), "Launch Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				rbtLog->AppendText(String::Format("[{0}] Launch Error | File {1} | Code {2}\n", DateTime::Now.ToLongTimeString(), filePath, err));
			}
		}
	}

	// Кнопка Оновити
	System::Void Form_OS_Lab2::refresh_button_Click(System::Object^ sender, System::EventArgs^ e) {
		RefreshProcessList();
		if (tabControl1->SelectedTab == tabPage1 && dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			RefreshThreadList(pid); // Потрібно створити цей метод за аналогією з RefreshProcessList
		}
	}

	// Подія Timer (автоматичне оновлення)
	System::Void Form_OS_Lab2::timerRefresh_Tick_1(System::Object^ sender, System::EventArgs^ e) {
		RefreshProcessList();
		// Оновлюємо потоки, лише якщо відкрита відповідна вкладка, щоб не перевантажувати UI
		if (tabControl1->SelectedTab == tabPage1 && dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			RefreshThreadList(pid); // Потрібно створити цей метод за аналогією з RefreshProcessList
		}
	}

	// Зупинка / Запуск таймера
	System::Void Form_OS_Lab2::Stop_Resume_Updating_button_Click(System::Object^ sender, System::EventArgs^ e) {
		timerRefresh->Enabled = !timerRefresh->Enabled;
		String^ state = timerRefresh->Enabled ? L"Working" : L"Stoped";
		Stop_Resume_Updating_button->Text = timerRefresh->Enabled ? L"Stop Updating" : L"Start Updating";
		updating_label->Text = L"Auto Updating: " + state;
		rbtLog->AppendText(String::Format(L"[{0}] Auto Udating Timer: {1}\n", DateTime::Now.ToLongTimeString(), state));
	}

	// Зміна інтервалу таймера
	System::Void Form_OS_Lab2::numInterval_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		timerRefresh->Interval = Convert::ToInt32(numInterval->Value);
	}

	//Кнопка пошуку
	System::Void Form_OS_Lab2::search_button_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ query = Search_Process_textBox->Text->ToLower(); // Ігнорування регістру
		if (String::IsNullOrWhiteSpace(query)) return;

		for each (DataGridViewRow ^ row in dgvProcesses->Rows) {
			if (row->Cells["colName"]->Value != nullptr) {
				String^ name = row->Cells["colName"]->Value->ToString()->ToLower();
				if (name->Contains(query)) {
					dgvProcesses->ClearSelection();
					row->Selected = true;
					// Прокрутити таблицю до знайденого рядка
					dgvProcesses->FirstDisplayedScrollingRowIndex = row->Index;
					break;
				}
			}
		}
	}

	// Скинути фільтри
	System::Void Form_OS_Lab2::reset_filter_button_Click(System::Object^ sender, System::EventArgs^ e) {
		chkSelectedPidOnly->Checked = false;
		chkHideSystem->Checked = false;
		Search_Process_textBox->Text = L"";
		sortColIndex = -1;

		dgvProcesses->ClearSelection();

		// Очищаємо таблицю потоків, щоб вона не залишалася на екрані
		dgvThreads->Rows->Clear();

		RefreshProcessList();
	}







	//
	//==Process Menu Stripses==
	//
	
	//Застосування сортування потоків
	System::Void Form_OS_Lab2::dgvProcesses_ColumnHeaderMouseClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^ e) {
		dgvProcesses->ClearSelection(); // Знімаємо виділення

		if (sortColIndex == e->ColumnIndex) {
			sortDirection = (sortDirection == System::ComponentModel::ListSortDirection::Ascending) ?
				System::ComponentModel::ListSortDirection::Descending : System::ComponentModel::ListSortDirection::Ascending;
		}
		else {
			sortColIndex = e->ColumnIndex;
			sortDirection = System::ComponentModel::ListSortDirection::Ascending;
		}

		RefreshProcessList(); // Оскільки виділення зняте, selectedPid буде -1

		if (dgvProcesses->Rows->Count > 0) {
			dgvProcesses->FirstDisplayedScrollingRowIndex = 0; // Прокручуємо до верху
		}
	}

	// Завершення процесу з контекстного меню
	System::Void Form_OS_Lab2::TerminateProcessToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			String^ name = dgvProcesses->SelectedRows[0]->Cells["colName"]->Value->ToString();
			DWORD errCode;
			System::Windows::Forms::DialogResult dr = MessageBox::Show(
				"Are you sure you want to terminate " + name + " (PID: " + pid + ")?",
				"Confirm Termination", MessageBoxButtons::YesNo, MessageBoxIcon::Warning);

			if (dr == System::Windows::Forms::DialogResult::Yes) {
				bool success = ProcessHelper::TerminateProcessByPid(pid, errCode);

				if (success) {
					MessageBox::Show("Process terminated successfully.", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
					rbtLog->AppendText(String::Format("[{0}] Terminate | PID {1} | Success\r\n", DateTime::Now.ToLongTimeString(), pid));
				}
				else {
					MessageBox::Show("Failed to terminate process. It might be a system process or access is denied.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
					rbtLog->AppendText(String::Format("[{0}] Terminate | PID {1} | Error {2}\r\n", DateTime::Now.ToLongTimeString(), pid, errCode));
				}

				RefreshProcessList();
			}
		}
	}

	//Вивести список потоків процесу
	System::Void Form_OS_Lab2::showProcessesThreadsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			RefreshThreadList(pid); // Викликаємо вашу існуючу функцію
			tabControl1->SelectedTab = tabPage1; // Перемикаємо вкладку
		}
	}

	//Призупинити процес
	System::Void Form_OS_Lab2::SuspendToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			String^ oldState = dgvProcesses->SelectedRows[0]->Cells["colState"]->Value->ToString();
			String^ resultStr = ProcessHelper::SuspendProcess(pid);

			rbtLog->AppendText(String::Format("[{0}] Suspend Process | PID {1} | Old: {2} -> Result: {3}\n",
				DateTime::Now.ToLongTimeString(), pid, oldState, resultStr));
			RefreshProcessList();
		}
	}

	//Відновити процес
	System::Void Form_OS_Lab2::resumeToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			ProcessHelper::ResumeProcess(pid);
			rbtLog->AppendText(String::Format("[{0}] Resume Process | PID {1}\r\n", DateTime::Now.ToLongTimeString(), pid));
			RefreshProcessList();
		}
	}

	//Вибір ядер виконання
	System::Void Form_OS_Lab2::SelectCoresCPUToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			DWORD_PTR pMask = 0, sMask = 0;

			if (ProcessHelper::GetAffinity(pid, pMask, sMask)) {
				AffinityForm^ form = gcnew AffinityForm();
				form->SetCurrentMask(pMask, ProcessHelper::GetLogicalProcessorCount());
				if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					ProcessHelper::SetAffinity(pid, form->ResultAffinityMask); //[cite: 10, 15]
					rbtLog->AppendText(String::Format("[{0}] SetAffinity | PID {1} | Mask {2}\r\n",
						DateTime::Now.ToLongTimeString(), pid, form->ResultAffinityMask));
				}
			}
			else {
				MessageBox::Show("Access to the core mask of this process is denied.", "Error");
			}
		}
	}

	//Налаштування на натискання на процес
	System::Void Form_OS_Lab2::dgvProcesses_CellMouseDown(System::Object^ sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^ e) {
		if (e->Button == System::Windows::Forms::MouseButtons::Right && e->RowIndex >= 0) {
			dgvProcesses->ClearSelection();
			dgvProcesses->Rows[e->RowIndex]->Selected = true;
		}
	}

	//Вибір високого пріоритету процесу
	System::Void Form_OS_Lab2::highToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			ProcessHelper::SetProcessPriority(pid, HIGH_PRIORITY_CLASS); //[cite: 5]
			rbtLog->AppendText(String::Format("[{0}] SetPriority High | PID {1}\r\n", DateTime::Now.ToLongTimeString(), pid));
			RefreshProcessList();
		}
	}

	//Вибір понад нормального пріоритету процесу
	System::Void Form_OS_Lab2::aboveNormalToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			ProcessHelper::SetProcessPriority(pid, ABOVE_NORMAL_PRIORITY_CLASS); //[cite: 5]
			rbtLog->AppendText(String::Format("[{0}] SetPriority Above Normal | PID {1}\r\n", DateTime::Now.ToLongTimeString(), pid));
			RefreshProcessList();
		}
	}

	//Вибір нормального пріоритету процесу
	System::Void Form_OS_Lab2::normalToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvProcesses->SelectedRows->Count > 0) {
		int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
		ProcessHelper::SetProcessPriority(pid, NORMAL_PRIORITY_CLASS); //[cite: 5]
		rbtLog->AppendText(String::Format("[{0}] SetPriority Normal | PID {1}\r\n", DateTime::Now.ToLongTimeString(), pid));
		RefreshProcessList();
		}
	}
	
	//Вибір нижче нормального пріоритету процесу
	System::Void Form_OS_Lab2::belowNormalToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
				if (dgvProcesses->SelectedRows->Count > 0) {
				int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
				ProcessHelper::SetProcessPriority(pid, BELOW_NORMAL_PRIORITY_CLASS); //[cite: 5]
				rbtLog->AppendText(String::Format("[{0}] SetPriority Below Normal | PID {1}\r\n", DateTime::Now.ToLongTimeString(), pid));
				  RefreshProcessList();
		  }
	  }

	//Вибір найнижчого пріоритету процесу
	System::Void Form_OS_Lab2::idleToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvProcesses->SelectedRows->Count > 0) {
				int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
				ProcessHelper::SetProcessPriority(pid, IDLE_PRIORITY_CLASS); //[cite: 5]
				rbtLog->AppendText(String::Format("[{0}] SetPriority Idle | PID {1}\r\n", DateTime::Now.ToLongTimeString(), pid));
				RefreshProcessList();
			}
	}






	//
	//==Threads Menu Stripses==
	//

	//Застосування сортування потоків
	System::Void Form_OS_Lab2::dgvThreads_ColumnHeaderMouseClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^ e) {
		dgvThreads->ClearSelection(); // Знімаємо виділення

		if (sortColIndex == e->ColumnIndex) {
			sortDirection = (sortDirection == System::ComponentModel::ListSortDirection::Ascending) ?
				System::ComponentModel::ListSortDirection::Descending : System::ComponentModel::ListSortDirection::Ascending;
		}
		else {
			sortColIndex = e->ColumnIndex;
			sortDirection = System::ComponentModel::ListSortDirection::Ascending;
		}

		// Отримуємо PID процесу-власника для оновлення потоків
		if (dgvProcesses->SelectedRows->Count > 0) {
			int pid = Convert::ToInt32(dgvProcesses->SelectedRows[0]->Cells["colPid"]->Value);
			RefreshThreadList(pid);

			if (dgvThreads->Rows->Count > 0) {
				dgvThreads->FirstDisplayedScrollingRowIndex = 0; // Прокручуємо до верху
			}
		}
	}

	//Призупинення потоку
	System::Void Form_OS_Lab2::SuspendThreadToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvThreads->SelectedRows->Count > 0) {
			int tid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
			int ownerPid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colOwnerPid"]->Value);
			ProcessHelper::SuspendSingleThread(tid, ownerPid);
			rbtLog->AppendText(String::Format("[{0}] Suspend Thread | TID {1}\n", DateTime::Now.ToLongTimeString(), tid));
			RefreshProcessList();
			RefreshThreadList(ownerPid);
		}
	}

	//Відновлення потоку
	System::Void Form_OS_Lab2::resumeThreadToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvThreads->SelectedRows->Count > 0) {
			int tid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
			int ownerPid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colOwnerPid"]->Value);
			ProcessHelper::ResumeSingleThread(tid, ownerPid);
			rbtLog->AppendText(String::Format("[{0}] Resume Thread | TID {1}\n", DateTime::Now.ToLongTimeString(), tid));
			RefreshProcessList();
			RefreshThreadList(ownerPid);
		}
	}

	//Видалення потоку
	System::Void Form_OS_Lab2::TerminateThreadToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		DWORD errCode;
		if (dgvThreads->SelectedRows->Count > 0) {
			int tid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
			if (ProcessHelper::TerminateSingleThread(tid, errCode)) {
			rbtLog->AppendText(String::Format("[{0}] Terminate Thread | TID {1} | Success\n", DateTime::Now.ToLongTimeString(), tid));
			}
			else {
				MessageBox::Show("Failed to terminate tread.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
				rbtLog->AppendText(String::Format("[{0}] Terminate | TID {1} | Error {2}\r\n", DateTime::Now.ToLongTimeString(), tid, errCode));
			}
		}
	}

	//Вибір ядер для потоку
	System::Void Form_OS_Lab2::threadAffinityToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (dgvThreads->SelectedRows->Count > 0) {
			int tid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
			int pid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colOwnerPid"]->Value);
			DWORD_PTR processMask = 0, systemMask = 0;

			// Спочатку отримуємо маску процесу-власника
			if (ProcessHelper::GetAffinity(pid, processMask, systemMask)) {
				AffinityForm^ form = gcnew AffinityForm();

				// Встановлюємо галочки відповідно до маски процесу, щоб користувач бачив доступні межі
				form->SetCurrentMask(processMask, ProcessHelper::GetLogicalProcessorCount());

				if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
					DWORD_PTR chosenMask = form->ResultAffinityMask;

					// Перевіряємо, чи є обрана маска потоку підмножиною маски процесу
					if ((chosenMask & processMask) == chosenMask) {
						if (ProcessHelper::SetThreadAffinity(tid, chosenMask)) { 
							rbtLog->AppendText(String::Format("[{0}] SetThreadAffinity | TID {1} | Mask {2}\r\n",
								DateTime::Now.ToLongTimeString(), tid, chosenMask));
							RefreshThreadList(pid);
						}
						else {
							MessageBox::Show("Setting thread mask error", "Error");
						}
					}
					else {
						MessageBox::Show("Threads mask is not subset of process mask!", "Limit Error");
					}
				}
			}
			else {
				MessageBox::Show("Owner process is not allowed to verify process mask.", "Error");
			}
		}
	}

	//Налаштування на натискання на потік
	System::Void Form_OS_Lab2::dgvThreads_CellMouseDown(System::Object^ sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^ e) {
		if (e->Button == System::Windows::Forms::MouseButtons::Right && e->RowIndex >= 0) {
			dgvThreads->ClearSelection();
			dgvThreads->Rows[e->RowIndex]->Selected = true;
		}
	}

	//Встановити високий пріоритет потоку
	   System::Void Form_OS_Lab2::highest2ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		   if (dgvThreads->SelectedRows->Count > 0) {
			   int tid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
			   ProcessHelper::SetThreadPriorityLevel(tid, THREAD_PRIORITY_HIGHEST); //[cite: 5]
			   rbtLog->AppendText(String::Format("[{0}] SetThreadPriority Highest | TID {1}\r\n", DateTime::Now.ToLongTimeString(), tid));
			   int ownerPid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colOwnerPid"]->Value);
			   RefreshThreadList(ownerPid);
		   }
	   }

	   //Встановити понад нормальний пріоритет потоку
	   System::Void Form_OS_Lab2::aboveNormal1ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		   if (dgvThreads->SelectedRows->Count > 0) {
			   int tid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
			   ProcessHelper::SetThreadPriorityLevel(tid, THREAD_PRIORITY_ABOVE_NORMAL); //[cite: 5]
			   rbtLog->AppendText(String::Format("[{0}] SetThreadPriority Above Normal | TID {1}\r\n", DateTime::Now.ToLongTimeString(), tid));
			   int ownerPid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colOwnerPid"]->Value);
			   RefreshThreadList(ownerPid);
		   }
	   }

	   //Встановити нормальний пріоритет потоку
	   System::Void Form_OS_Lab2::normal0ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		   if (dgvThreads->SelectedRows->Count > 0) {
			   int tid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
			   ProcessHelper::SetThreadPriorityLevel(tid, THREAD_PRIORITY_NORMAL); //[cite: 5]
			   rbtLog->AppendText(String::Format("[{0}] SetThreadPriority Normal | TID {1}\r\n", DateTime::Now.ToLongTimeString(), tid));
			   int ownerPid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colOwnerPid"]->Value);
			   RefreshThreadList(ownerPid);
		   }
	   }

	   //Встановити нижче  нормального пріоритет потоку
	   System::Void Form_OS_Lab2::belowNormal1ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		   if (dgvThreads->SelectedRows->Count > 0) {
			   int tid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
			   ProcessHelper::SetThreadPriorityLevel(tid, THREAD_PRIORITY_BELOW_NORMAL); //[cite: 5]
			   rbtLog->AppendText(String::Format("[{0}] SetThreadPriority Below Normal | TID {1}\r\n", DateTime::Now.ToLongTimeString(), tid));
			   int ownerPid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colOwnerPid"]->Value);
			   RefreshThreadList(ownerPid);
		   }
	   }

	   //Встановити низький пріоритет потоку
	   System::Void Form_OS_Lab2::lowest2ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		   if (dgvThreads->SelectedRows->Count > 0) {
			   int tid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colTid"]->Value);
			   ProcessHelper::SetThreadPriorityLevel(tid, THREAD_PRIORITY_LOWEST); //[cite: 5]
			   rbtLog->AppendText(String::Format("[{0}] SetThreadPriority Lowest | TID {1}\r\n", DateTime::Now.ToLongTimeString(), tid));
			   int ownerPid = Convert::ToInt32(dgvThreads->SelectedRows[0]->Cells["colOwnerPid"]->Value);
			   RefreshThreadList(ownerPid);
		   }
	   }
}
[STAThreadAttribute]
int main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	OSLab2::Form_OS_Lab2 form;
	Application::Run(% form);
	return 0;
}