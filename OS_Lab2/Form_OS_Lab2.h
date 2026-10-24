#pragma once
#include "ProcessHelper.h"
#include "AffinityForm.h"

namespace OSLab2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form_OS_Lab2
	/// </summary>
	public ref class Form_OS_Lab2 : public System::Windows::Forms::Form
	{
	public:
		Form_OS_Lab2(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			for each (DataGridViewColumn ^ col in dgvProcesses->Columns) {
				col->SortMode = DataGridViewColumnSortMode::Programmatic;
			}
			for each (DataGridViewColumn ^ col in dgvThreads->Columns) {
				col->SortMode = DataGridViewColumnSortMode::Programmatic;
			}
			dgvProcesses->Columns["colCpu"]->DefaultCellStyle->Format = "F1";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form_OS_Lab2()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		int sortColIndex = -1;
		System::ComponentModel::ListSortDirection sortDirection = System::ComponentModel::ListSortDirection::Ascending;
		int pidToSelect = -1;



	private: System::Windows::Forms::Panel^ pnlTopControls;
	private: System::Windows::Forms::SplitContainer^ splitMain;
	private: System::Windows::Forms::StatusStrip^ statusStrip1;
	private: System::Windows::Forms::NumericUpDown^ numInterval;
	private: System::Windows::Forms::Label^ numIntervalLable;


	public: System::Windows::Forms::CheckBox^ chkSelectedPidOnly;

	private: System::Windows::Forms::CheckBox^ chkHideSystem;

	public: System::Windows::Forms::CheckBox^ chkSuspended;

	private: System::Windows::Forms::Button^ btnLaunch;

	private: System::Windows::Forms::Button^ btnRefresh;
	public: System::Windows::Forms::Timer^ timerRefresh;
	public: System::Windows::Forms::DataGridView^ dgvProcesses;

	private: System::Windows::Forms::ContextMenuStrip^ ctxProcessMenu;











	private: System::Windows::Forms::TabControl^ tabControl1;
	private: System::Windows::Forms::TabPage^ tabPage1;

	private: System::Windows::Forms::TabPage^ tabPage2;
	private: System::Windows::Forms::ContextMenuStrip^ ctxThreadMenu;







	private: System::Windows::Forms::ToolStripMenuItem^ пріоритетToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ highToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ aboveNormalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ normalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ belowNormalToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ idleToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ вибірЯдерCPUToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ SuspendToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^ TerminateProcessToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ PriorityThreadToolStripMenuItem1;


	private: System::Windows::Forms::ToolStripMenuItem^ highest2ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ aboveNormal1ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ normal0ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ belowNormal1ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ lowest2ToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^ SuspendThreadToolStripMenuItem1;
private: System::Windows::Forms::ToolStripMenuItem^ TerminateThreadToolStripMenuItem;



	public: System::Windows::Forms::ToolStripStatusLabel^ lblTotalProcesses;
	private: System::Windows::Forms::ToolStripStatusLabel^ lblVisibleProcesses;
	private: System::Windows::Forms::ToolStripStatusLabel^ lblLogicalCpu;
	private: System::Windows::Forms::ToolStripStatusLabel^ lblAccessLevel;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colPid;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colName;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colCpu;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colPriority;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colState;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colWs;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colVa;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colPrivate;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colThreads;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colOwner;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^ colPath;






private: System::Windows::Forms::DataGridView^ dgvThreads;
private: System::Windows::Forms::TextBox^ Search_Process_textBox;
private: System::Windows::Forms::Label^ Search_label;
private: System::Windows::Forms::Button^ search_button;
private: System::Windows::Forms::ToolStripMenuItem^ showProcessesThreadsToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^ resumeToolStripMenuItem1;
public: System::Windows::Forms::RichTextBox^ rbtLog;
private: System::Windows::Forms::ToolStripMenuItem^ resumeThreadToolStripMenuItem1;
private: System::Windows::Forms::Label^ updating_label;
private: System::Windows::Forms::CheckBox^ ArgsCheckBox;
private: System::Windows::Forms::Button^ reset_filter_button;
private: System::Windows::Forms::Button^ Stop_Resume_Updating_button;
private: System::Windows::Forms::Label^ args_label;
private: System::Windows::Forms::TextBox^ args_textBox;
private: System::Windows::Forms::ToolStripMenuItem^ threadAffinityToolStripMenuItem;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ colTid;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ colOwnerPid;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ colThreadCpu;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ colThreadPriority;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ colThreadState;
private: System::Windows::Forms::DataGridViewTextBoxColumn^ colAffinity;


public:
private:



















	private: System::ComponentModel::IContainer^ components;






	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pnlTopControls = (gcnew System::Windows::Forms::Panel());
			this->args_label = (gcnew System::Windows::Forms::Label());
			this->args_textBox = (gcnew System::Windows::Forms::TextBox());
			this->reset_filter_button = (gcnew System::Windows::Forms::Button());
			this->Stop_Resume_Updating_button = (gcnew System::Windows::Forms::Button());
			this->updating_label = (gcnew System::Windows::Forms::Label());
			this->ArgsCheckBox = (gcnew System::Windows::Forms::CheckBox());
			this->search_button = (gcnew System::Windows::Forms::Button());
			this->Search_label = (gcnew System::Windows::Forms::Label());
			this->Search_Process_textBox = (gcnew System::Windows::Forms::TextBox());
			this->numInterval = (gcnew System::Windows::Forms::NumericUpDown());
			this->numIntervalLable = (gcnew System::Windows::Forms::Label());
			this->chkSelectedPidOnly = (gcnew System::Windows::Forms::CheckBox());
			this->chkHideSystem = (gcnew System::Windows::Forms::CheckBox());
			this->chkSuspended = (gcnew System::Windows::Forms::CheckBox());
			this->btnLaunch = (gcnew System::Windows::Forms::Button());
			this->btnRefresh = (gcnew System::Windows::Forms::Button());
			this->splitMain = (gcnew System::Windows::Forms::SplitContainer());
			this->dgvProcesses = (gcnew System::Windows::Forms::DataGridView());
			this->colPid = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colCpu = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colPriority = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colState = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colWs = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colVa = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colPrivate = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colThreads = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colOwner = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colPath = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ctxProcessMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->пріоритетToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->highToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboveNormalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->normalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->belowNormalToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->idleToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->вибірЯдерCPUToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SuspendToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resumeToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TerminateProcessToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->showProcessesThreadsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->dgvThreads = (gcnew System::Windows::Forms::DataGridView());
			this->colTid = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colOwnerPid = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colThreadCpu = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colThreadPriority = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colThreadState = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->colAffinity = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ctxThreadMenu = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->PriorityThreadToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->highest2ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboveNormal1ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->normal0ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->belowNormal1ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->lowest2ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->threadAffinityToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->SuspendThreadToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resumeThreadToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TerminateThreadToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->rbtLog = (gcnew System::Windows::Forms::RichTextBox());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->lblTotalProcesses = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->lblVisibleProcesses = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->lblLogicalCpu = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->lblAccessLevel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->timerRefresh = (gcnew System::Windows::Forms::Timer(this->components));
			this->pnlTopControls->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numInterval))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitMain))->BeginInit();
			this->splitMain->Panel1->SuspendLayout();
			this->splitMain->Panel2->SuspendLayout();
			this->splitMain->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvProcesses))->BeginInit();
			this->ctxProcessMenu->SuspendLayout();
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvThreads))->BeginInit();
			this->ctxThreadMenu->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// pnlTopControls
			// 
			this->pnlTopControls->Controls->Add(this->args_label);
			this->pnlTopControls->Controls->Add(this->args_textBox);
			this->pnlTopControls->Controls->Add(this->reset_filter_button);
			this->pnlTopControls->Controls->Add(this->Stop_Resume_Updating_button);
			this->pnlTopControls->Controls->Add(this->updating_label);
			this->pnlTopControls->Controls->Add(this->ArgsCheckBox);
			this->pnlTopControls->Controls->Add(this->search_button);
			this->pnlTopControls->Controls->Add(this->Search_label);
			this->pnlTopControls->Controls->Add(this->Search_Process_textBox);
			this->pnlTopControls->Controls->Add(this->numInterval);
			this->pnlTopControls->Controls->Add(this->numIntervalLable);
			this->pnlTopControls->Controls->Add(this->chkSelectedPidOnly);
			this->pnlTopControls->Controls->Add(this->chkHideSystem);
			this->pnlTopControls->Controls->Add(this->chkSuspended);
			this->pnlTopControls->Controls->Add(this->btnLaunch);
			this->pnlTopControls->Controls->Add(this->btnRefresh);
			this->pnlTopControls->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlTopControls->Location = System::Drawing::Point(0, 0);
			this->pnlTopControls->Name = L"pnlTopControls";
			this->pnlTopControls->Size = System::Drawing::Size(1582, 92);
			this->pnlTopControls->TabIndex = 1;
			// 
			// args_label
			// 
			this->args_label->AutoSize = true;
			this->args_label->Location = System::Drawing::Point(500, 62);
			this->args_label->Name = L"args_label";
			this->args_label->Size = System::Drawing::Size(91, 16);
			this->args_label->TabIndex = 14;
			this->args_label->Text = L"Process Args:";
			// 
			// args_textBox
			// 
			this->args_textBox->Location = System::Drawing::Point(597, 59);
			this->args_textBox->Name = L"args_textBox";
			this->args_textBox->Size = System::Drawing::Size(190, 22);
			this->args_textBox->TabIndex = 13;
			// 
			// reset_filter_button
			// 
			this->reset_filter_button->Location = System::Drawing::Point(207, 49);
			this->reset_filter_button->Name = L"reset_filter_button";
			this->reset_filter_button->Size = System::Drawing::Size(101, 29);
			this->reset_filter_button->TabIndex = 12;
			this->reset_filter_button->Text = L"Reset Filter";
			this->reset_filter_button->UseVisualStyleBackColor = true;
			this->reset_filter_button->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::reset_filter_button_Click);
			// 
			// Stop_Resume_Updating_button
			// 
			this->Stop_Resume_Updating_button->Location = System::Drawing::Point(827, 41);
			this->Stop_Resume_Updating_button->Name = L"Stop_Resume_Updating_button";
			this->Stop_Resume_Updating_button->Size = System::Drawing::Size(145, 40);
			this->Stop_Resume_Updating_button->TabIndex = 11;
			this->Stop_Resume_Updating_button->Text = L"Stop Updating";
			this->Stop_Resume_Updating_button->UseVisualStyleBackColor = true;
			this->Stop_Resume_Updating_button->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::Stop_Resume_Updating_button_Click);
			// 
			// updating_label
			// 
			this->updating_label->AutoSize = true;
			this->updating_label->Location = System::Drawing::Point(824, 12);
			this->updating_label->Name = L"updating_label";
			this->updating_label->Size = System::Drawing::Size(148, 16);
			this->updating_label->TabIndex = 10;
			this->updating_label->Text = L"Auto Updating: Working";
			// 
			// ArgsCheckBox
			// 
			this->ArgsCheckBox->AutoSize = true;
			this->ArgsCheckBox->Location = System::Drawing::Point(622, 38);
			this->ArgsCheckBox->Name = L"ArgsCheckBox";
			this->ArgsCheckBox->Size = System::Drawing::Size(147, 20);
			this->ArgsCheckBox->TabIndex = 9;
			this->ArgsCheckBox->Text = L"Launch with terminal";
			this->ArgsCheckBox->UseVisualStyleBackColor = true;
			// 
			// search_button
			// 
			this->search_button->Location = System::Drawing::Point(314, 3);
			this->search_button->Name = L"search_button";
			this->search_button->Size = System::Drawing::Size(87, 31);
			this->search_button->TabIndex = 1;
			this->search_button->Text = L"Search";
			this->search_button->UseVisualStyleBackColor = true;
			this->search_button->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::search_button_Click);
			// 
			// Search_label
			// 
			this->Search_label->AutoSize = true;
			this->Search_label->Location = System::Drawing::Point(12, 10);
			this->Search_label->Name = L"Search_label";
			this->Search_label->Size = System::Drawing::Size(100, 16);
			this->Search_label->TabIndex = 8;
			this->Search_label->Text = L"Process Name:";
			// 
			// Search_Process_textBox
			// 
			this->Search_Process_textBox->Location = System::Drawing::Point(118, 8);
			this->Search_Process_textBox->Name = L"Search_Process_textBox";
			this->Search_Process_textBox->Size = System::Drawing::Size(190, 22);
			this->Search_Process_textBox->TabIndex = 7;
			this->Search_Process_textBox->Enter += gcnew System::EventHandler(this, &Form_OS_Lab2::search_button_Click);
			this->Search_Process_textBox->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form_OS_Lab2::Search_Process_textBox_KeyDown);
			// 
			// numInterval
			// 
			this->numInterval->Location = System::Drawing::Point(1086, 9);
			this->numInterval->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->numInterval->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 500, 0, 0, 0 });
			this->numInterval->Name = L"numInterval";
			this->numInterval->Size = System::Drawing::Size(63, 22);
			this->numInterval->TabIndex = 6;
			this->numInterval->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1000, 0, 0, 0 });
			this->numInterval->ValueChanged += gcnew System::EventHandler(this, &Form_OS_Lab2::numInterval_ValueChanged);
			// 
			// numIntervalLable
			// 
			this->numIntervalLable->AutoSize = true;
			this->numIntervalLable->Location = System::Drawing::Point(998, 11);
			this->numIntervalLable->Name = L"numIntervalLable";
			this->numIntervalLable->Size = System::Drawing::Size(82, 16);
			this->numIntervalLable->TabIndex = 5;
			this->numIntervalLable->Text = L"Interval (ms):";
			// 
			// chkSelectedPidOnly
			// 
			this->chkSelectedPidOnly->AutoSize = true;
			this->chkSelectedPidOnly->Location = System::Drawing::Point(15, 59);
			this->chkSelectedPidOnly->Name = L"chkSelectedPidOnly";
			this->chkSelectedPidOnly->Size = System::Drawing::Size(136, 20);
			this->chkSelectedPidOnly->TabIndex = 4;
			this->chkSelectedPidOnly->Text = L"Only selected PID";
			this->chkSelectedPidOnly->UseVisualStyleBackColor = true;
			this->chkSelectedPidOnly->CheckedChanged += gcnew System::EventHandler(this, &Form_OS_Lab2::chkSelectedPidOnly_CheckedChanged);
			// 
			// chkHideSystem
			// 
			this->chkHideSystem->AutoSize = true;
			this->chkHideSystem->Location = System::Drawing::Point(15, 33);
			this->chkHideSystem->Name = L"chkHideSystem";
			this->chkHideSystem->Size = System::Drawing::Size(171, 20);
			this->chkHideSystem->TabIndex = 3;
			this->chkHideSystem->Text = L"Hide system processes";
			this->chkHideSystem->UseVisualStyleBackColor = true;
			// 
			// chkSuspended
			// 
			this->chkSuspended->AutoSize = true;
			this->chkSuspended->Location = System::Drawing::Point(622, 12);
			this->chkSuspended->Name = L"chkSuspended";
			this->chkSuspended->Size = System::Drawing::Size(127, 20);
			this->chkSuspended->TabIndex = 2;
			this->chkSuspended->Text = L"Start suspended";
			this->chkSuspended->UseVisualStyleBackColor = true;
			// 
			// btnLaunch
			// 
			this->btnLaunch->Location = System::Drawing::Point(478, 12);
			this->btnLaunch->Name = L"btnLaunch";
			this->btnLaunch->Size = System::Drawing::Size(123, 41);
			this->btnLaunch->TabIndex = 1;
			this->btnLaunch->Text = L"Start exe";
			this->btnLaunch->UseVisualStyleBackColor = true;
			this->btnLaunch->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::btnLaunch_Click);
			// 
			// btnRefresh
			// 
			this->btnRefresh->Location = System::Drawing::Point(1035, 39);
			this->btnRefresh->Name = L"btnRefresh";
			this->btnRefresh->Size = System::Drawing::Size(83, 40);
			this->btnRefresh->TabIndex = 0;
			this->btnRefresh->Text = L"Update";
			this->btnRefresh->UseVisualStyleBackColor = true;
			this->btnRefresh->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::refresh_button_Click);
			// 
			// splitMain
			// 
			this->splitMain->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitMain->Location = System::Drawing::Point(0, 92);
			this->splitMain->Name = L"splitMain";
			this->splitMain->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitMain.Panel1
			// 
			this->splitMain->Panel1->Controls->Add(this->dgvProcesses);
			// 
			// splitMain.Panel2
			// 
			this->splitMain->Panel2->Controls->Add(this->tabControl1);
			this->splitMain->Size = System::Drawing::Size(1582, 861);
			this->splitMain->SplitterDistance = 412;
			this->splitMain->TabIndex = 2;
			// 
			// dgvProcesses
			// 
			this->dgvProcesses->AllowUserToAddRows = false;
			this->dgvProcesses->AllowUserToDeleteRows = false;
			this->dgvProcesses->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dgvProcesses->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(11) {
				this->colPid,
					this->colName, this->colCpu, this->colPriority, this->colState, this->colWs, this->colVa, this->colPrivate, this->colThreads,
					this->colOwner, this->colPath
			});
			this->dgvProcesses->ContextMenuStrip = this->ctxProcessMenu;
			this->dgvProcesses->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dgvProcesses->Location = System::Drawing::Point(0, 0);
			this->dgvProcesses->MultiSelect = false;
			this->dgvProcesses->Name = L"dgvProcesses";
			this->dgvProcesses->ReadOnly = true;
			this->dgvProcesses->RowHeadersVisible = false;
			this->dgvProcesses->RowHeadersWidth = 51;
			this->dgvProcesses->RowTemplate->Height = 24;
			this->dgvProcesses->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dgvProcesses->Size = System::Drawing::Size(1582, 412);
			this->dgvProcesses->TabIndex = 0;
			this->dgvProcesses->CellMouseDown += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &Form_OS_Lab2::dgvProcesses_CellMouseDown);
			this->dgvProcesses->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &Form_OS_Lab2::dgvProcesses_ColumnHeaderMouseClick);
			// 
			// colPid
			// 
			this->colPid->HeaderText = L"PID";
			this->colPid->MinimumWidth = 6;
			this->colPid->Name = L"colPid";
			this->colPid->ReadOnly = true;
			this->colPid->Width = 75;
			// 
			// colName
			// 
			this->colName->HeaderText = L"Name";
			this->colName->MinimumWidth = 6;
			this->colName->Name = L"colName";
			this->colName->ReadOnly = true;
			this->colName->Width = 125;
			// 
			// colCpu
			// 
			this->colCpu->HeaderText = L"CPU, %";
			this->colCpu->MinimumWidth = 6;
			this->colCpu->Name = L"colCpu";
			this->colCpu->ReadOnly = true;
			this->colCpu->Width = 125;
			// 
			// colPriority
			// 
			this->colPriority->HeaderText = L"Priority";
			this->colPriority->MinimumWidth = 6;
			this->colPriority->Name = L"colPriority";
			this->colPriority->ReadOnly = true;
			this->colPriority->Width = 125;
			// 
			// colState
			// 
			this->colState->HeaderText = L"State";
			this->colState->MinimumWidth = 6;
			this->colState->Name = L"colState";
			this->colState->ReadOnly = true;
			this->colState->Width = 125;
			// 
			// colWs
			// 
			this->colWs->HeaderText = L"WS, MiB";
			this->colWs->MinimumWidth = 6;
			this->colWs->Name = L"colWs";
			this->colWs->ReadOnly = true;
			this->colWs->Width = 125;
			// 
			// colVa
			// 
			this->colVa->HeaderText = L"VA, MiB";
			this->colVa->MinimumWidth = 6;
			this->colVa->Name = L"colVa";
			this->colVa->ReadOnly = true;
			this->colVa->Width = 125;
			// 
			// colPrivate
			// 
			this->colPrivate->HeaderText = L"Private, MiB";
			this->colPrivate->MinimumWidth = 6;
			this->colPrivate->Name = L"colPrivate";
			this->colPrivate->ReadOnly = true;
			this->colPrivate->Width = 125;
			// 
			// colThreads
			// 
			this->colThreads->HeaderText = L"Threads";
			this->colThreads->MinimumWidth = 6;
			this->colThreads->Name = L"colThreads";
			this->colThreads->ReadOnly = true;
			this->colThreads->Width = 125;
			// 
			// colOwner
			// 
			this->colOwner->HeaderText = L"Owner";
			this->colOwner->MinimumWidth = 6;
			this->colOwner->Name = L"colOwner";
			this->colOwner->ReadOnly = true;
			this->colOwner->Width = 125;
			// 
			// colPath
			// 
			this->colPath->HeaderText = L"Path";
			this->colPath->MinimumWidth = 6;
			this->colPath->Name = L"colPath";
			this->colPath->ReadOnly = true;
			this->colPath->Width = 125;
			// 
			// ctxProcessMenu
			// 
			this->ctxProcessMenu->ImageScalingSize = System::Drawing::Size(20, 20);
			this->ctxProcessMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->пріоритетToolStripMenuItem,
					this->вибірЯдерCPUToolStripMenuItem, this->SuspendToolStripMenuItem, this->resumeToolStripMenuItem1, this->TerminateProcessToolStripMenuItem,
					this->showProcessesThreadsToolStripMenuItem
			});
			this->ctxProcessMenu->Name = L"ctxProcessMenu";
			this->ctxProcessMenu->Size = System::Drawing::Size(238, 148);
			// 
			// пріоритетToolStripMenuItem
			// 
			this->пріоритетToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->highToolStripMenuItem,
					this->aboveNormalToolStripMenuItem, this->normalToolStripMenuItem, this->belowNormalToolStripMenuItem, this->idleToolStripMenuItem
			});
			this->пріоритетToolStripMenuItem->Name = L"пріоритетToolStripMenuItem";
			this->пріоритетToolStripMenuItem->Size = System::Drawing::Size(237, 24);
			this->пріоритетToolStripMenuItem->Text = L"Priority";
			// 
			// highToolStripMenuItem
			// 
			this->highToolStripMenuItem->Name = L"highToolStripMenuItem";
			this->highToolStripMenuItem->Size = System::Drawing::Size(189, 26);
			this->highToolStripMenuItem->Text = L"High";
			this->highToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::highToolStripMenuItem_Click);
			// 
			// aboveNormalToolStripMenuItem
			// 
			this->aboveNormalToolStripMenuItem->Name = L"aboveNormalToolStripMenuItem";
			this->aboveNormalToolStripMenuItem->Size = System::Drawing::Size(189, 26);
			this->aboveNormalToolStripMenuItem->Text = L"Above Normal";
			this->aboveNormalToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::aboveNormalToolStripMenuItem_Click);
			// 
			// normalToolStripMenuItem
			// 
			this->normalToolStripMenuItem->Name = L"normalToolStripMenuItem";
			this->normalToolStripMenuItem->Size = System::Drawing::Size(189, 26);
			this->normalToolStripMenuItem->Text = L"Normal";
			this->normalToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::normalToolStripMenuItem_Click);
			// 
			// belowNormalToolStripMenuItem
			// 
			this->belowNormalToolStripMenuItem->Name = L"belowNormalToolStripMenuItem";
			this->belowNormalToolStripMenuItem->Size = System::Drawing::Size(189, 26);
			this->belowNormalToolStripMenuItem->Text = L"Below Normal";
			this->belowNormalToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::belowNormalToolStripMenuItem_Click);
			// 
			// idleToolStripMenuItem
			// 
			this->idleToolStripMenuItem->Name = L"idleToolStripMenuItem";
			this->idleToolStripMenuItem->Size = System::Drawing::Size(189, 26);
			this->idleToolStripMenuItem->Text = L"Idle";
			this->idleToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::idleToolStripMenuItem_Click);
			// 
			// вибірЯдерCPUToolStripMenuItem
			// 
			this->вибірЯдерCPUToolStripMenuItem->Name = L"вибірЯдерCPUToolStripMenuItem";
			this->вибірЯдерCPUToolStripMenuItem->Size = System::Drawing::Size(237, 24);
			this->вибірЯдерCPUToolStripMenuItem->Text = L"Set affinity";
			this->вибірЯдерCPUToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::SelectCoresCPUToolStripMenuItem_Click);
			// 
			// SuspendToolStripMenuItem
			// 
			this->SuspendToolStripMenuItem->Name = L"SuspendToolStripMenuItem";
			this->SuspendToolStripMenuItem->Size = System::Drawing::Size(237, 24);
			this->SuspendToolStripMenuItem->Text = L"Suspend";
			this->SuspendToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::SuspendToolStripMenuItem_Click);
			// 
			// resumeToolStripMenuItem1
			// 
			this->resumeToolStripMenuItem1->Name = L"resumeToolStripMenuItem1";
			this->resumeToolStripMenuItem1->Size = System::Drawing::Size(237, 24);
			this->resumeToolStripMenuItem1->Text = L"Resume";
			this->resumeToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::resumeToolStripMenuItem1_Click);
			// 
			// TerminateProcessToolStripMenuItem
			// 
			this->TerminateProcessToolStripMenuItem->Name = L"TerminateProcessToolStripMenuItem";
			this->TerminateProcessToolStripMenuItem->Size = System::Drawing::Size(237, 24);
			this->TerminateProcessToolStripMenuItem->Text = L"Terminate process";
			this->TerminateProcessToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::TerminateProcessToolStripMenuItem_Click);
			// 
			// showProcessesThreadsToolStripMenuItem
			// 
			this->showProcessesThreadsToolStripMenuItem->Name = L"showProcessesThreadsToolStripMenuItem";
			this->showProcessesThreadsToolStripMenuItem->Size = System::Drawing::Size(237, 24);
			this->showProcessesThreadsToolStripMenuItem->Text = L"Show Processes Threads";
			this->showProcessesThreadsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::showProcessesThreadsToolStripMenuItem_Click);
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tabControl1->Location = System::Drawing::Point(0, 0);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(1582, 445);
			this->tabControl1->TabIndex = 0;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->dgvThreads);
			this->tabPage1->Location = System::Drawing::Point(4, 25);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(1574, 416);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Threads of selected process";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// dgvThreads
			// 
			this->dgvThreads->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dgvThreads->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(6) {
				this->colTid, this->colOwnerPid,
					this->colThreadCpu, this->colThreadPriority, this->colThreadState, this->colAffinity
			});
			this->dgvThreads->ContextMenuStrip = this->ctxThreadMenu;
			this->dgvThreads->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dgvThreads->Location = System::Drawing::Point(3, 3);
			this->dgvThreads->MultiSelect = false;
			this->dgvThreads->Name = L"dgvThreads";
			this->dgvThreads->ReadOnly = true;
			this->dgvThreads->RowHeadersWidth = 51;
			this->dgvThreads->RowTemplate->Height = 24;
			this->dgvThreads->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dgvThreads->Size = System::Drawing::Size(1568, 410);
			this->dgvThreads->TabIndex = 0;
			this->dgvThreads->CellMouseDown += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &Form_OS_Lab2::dgvThreads_CellMouseDown);
			this->dgvThreads->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &Form_OS_Lab2::dgvThreads_ColumnHeaderMouseClick);
			// 
			// colTid
			// 
			this->colTid->HeaderText = L"TID";
			this->colTid->MinimumWidth = 6;
			this->colTid->Name = L"colTid";
			this->colTid->ReadOnly = true;
			this->colTid->Width = 125;
			// 
			// colOwnerPid
			// 
			this->colOwnerPid->HeaderText = L"PID";
			this->colOwnerPid->MinimumWidth = 6;
			this->colOwnerPid->Name = L"colOwnerPid";
			this->colOwnerPid->ReadOnly = true;
			this->colOwnerPid->Width = 125;
			// 
			// colThreadCpu
			// 
			this->colThreadCpu->HeaderText = L"CPU, %";
			this->colThreadCpu->MinimumWidth = 6;
			this->colThreadCpu->Name = L"colThreadCpu";
			this->colThreadCpu->ReadOnly = true;
			this->colThreadCpu->Width = 125;
			// 
			// colThreadPriority
			// 
			this->colThreadPriority->HeaderText = L"Priority";
			this->colThreadPriority->MinimumWidth = 6;
			this->colThreadPriority->Name = L"colThreadPriority";
			this->colThreadPriority->ReadOnly = true;
			this->colThreadPriority->Width = 125;
			// 
			// colThreadState
			// 
			this->colThreadState->HeaderText = L"State";
			this->colThreadState->MinimumWidth = 6;
			this->colThreadState->Name = L"colThreadState";
			this->colThreadState->ReadOnly = true;
			this->colThreadState->Width = 125;
			// 
			// colAffinity
			// 
			this->colAffinity->HeaderText = L"CPU affinity";
			this->colAffinity->MinimumWidth = 6;
			this->colAffinity->Name = L"colAffinity";
			this->colAffinity->ReadOnly = true;
			this->colAffinity->Width = 250;
			// 
			// ctxThreadMenu
			// 
			this->ctxThreadMenu->ImageScalingSize = System::Drawing::Size(20, 20);
			this->ctxThreadMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->PriorityThreadToolStripMenuItem1,
					this->threadAffinityToolStripMenuItem, this->SuspendThreadToolStripMenuItem1, this->resumeThreadToolStripMenuItem1, this->TerminateThreadToolStripMenuItem
			});
			this->ctxThreadMenu->Name = L"ctxThreadMenu";
			this->ctxThreadMenu->Size = System::Drawing::Size(200, 124);
			// 
			// PriorityThreadToolStripMenuItem1
			// 
			this->PriorityThreadToolStripMenuItem1->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(5) {
				this->highest2ToolStripMenuItem,
					this->aboveNormal1ToolStripMenuItem, this->normal0ToolStripMenuItem, this->belowNormal1ToolStripMenuItem, this->lowest2ToolStripMenuItem
			});
			this->PriorityThreadToolStripMenuItem1->Name = L"PriorityThreadToolStripMenuItem1";
			this->PriorityThreadToolStripMenuItem1->Size = System::Drawing::Size(199, 24);
			this->PriorityThreadToolStripMenuItem1->Text = L"Priority";
			// 
			// highest2ToolStripMenuItem
			// 
			this->highest2ToolStripMenuItem->Name = L"highest2ToolStripMenuItem";
			this->highest2ToolStripMenuItem->Size = System::Drawing::Size(221, 26);
			this->highest2ToolStripMenuItem->Text = L"Highest (+2)";
			this->highest2ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::highest2ToolStripMenuItem_Click);
			// 
			// aboveNormal1ToolStripMenuItem
			// 
			this->aboveNormal1ToolStripMenuItem->Name = L"aboveNormal1ToolStripMenuItem";
			this->aboveNormal1ToolStripMenuItem->Size = System::Drawing::Size(221, 26);
			this->aboveNormal1ToolStripMenuItem->Text = L"Above Normal (+1)";
			this->aboveNormal1ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::aboveNormal1ToolStripMenuItem_Click);
			// 
			// normal0ToolStripMenuItem
			// 
			this->normal0ToolStripMenuItem->Name = L"normal0ToolStripMenuItem";
			this->normal0ToolStripMenuItem->Size = System::Drawing::Size(221, 26);
			this->normal0ToolStripMenuItem->Text = L"Normal (0)";
			this->normal0ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::normal0ToolStripMenuItem_Click);
			// 
			// belowNormal1ToolStripMenuItem
			// 
			this->belowNormal1ToolStripMenuItem->Name = L"belowNormal1ToolStripMenuItem";
			this->belowNormal1ToolStripMenuItem->Size = System::Drawing::Size(221, 26);
			this->belowNormal1ToolStripMenuItem->Text = L"Below Normal (-1)";
			this->belowNormal1ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::belowNormal1ToolStripMenuItem_Click);
			// 
			// lowest2ToolStripMenuItem
			// 
			this->lowest2ToolStripMenuItem->Name = L"lowest2ToolStripMenuItem";
			this->lowest2ToolStripMenuItem->Size = System::Drawing::Size(221, 26);
			this->lowest2ToolStripMenuItem->Text = L"Lowest (-2)";
			this->lowest2ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::lowest2ToolStripMenuItem_Click);
			// 
			// threadAffinityToolStripMenuItem
			// 
			this->threadAffinityToolStripMenuItem->Name = L"threadAffinityToolStripMenuItem";
			this->threadAffinityToolStripMenuItem->Size = System::Drawing::Size(199, 24);
			this->threadAffinityToolStripMenuItem->Text = L"Set Thread affinity";
			this->threadAffinityToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::threadAffinityToolStripMenuItem_Click);
			// 
			// SuspendThreadToolStripMenuItem1
			// 
			this->SuspendThreadToolStripMenuItem1->Name = L"SuspendThreadToolStripMenuItem1";
			this->SuspendThreadToolStripMenuItem1->Size = System::Drawing::Size(199, 24);
			this->SuspendThreadToolStripMenuItem1->Text = L"Suspend";
			this->SuspendThreadToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::SuspendThreadToolStripMenuItem1_Click);
			// 
			// resumeThreadToolStripMenuItem1
			// 
			this->resumeThreadToolStripMenuItem1->Name = L"resumeThreadToolStripMenuItem1";
			this->resumeThreadToolStripMenuItem1->Size = System::Drawing::Size(199, 24);
			this->resumeThreadToolStripMenuItem1->Text = L"Resume";
			this->resumeThreadToolStripMenuItem1->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::resumeThreadToolStripMenuItem1_Click);
			// 
			// TerminateThreadToolStripMenuItem
			// 
			this->TerminateThreadToolStripMenuItem->Name = L"TerminateThreadToolStripMenuItem";
			this->TerminateThreadToolStripMenuItem->Size = System::Drawing::Size(199, 24);
			this->TerminateThreadToolStripMenuItem->Text = L"Terminate thread";
			this->TerminateThreadToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form_OS_Lab2::TerminateThreadToolStripMenuItem_Click);
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->rbtLog);
			this->tabPage2->Location = System::Drawing::Point(4, 25);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(1674, 416);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Operation log";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// rbtLog
			// 
			this->rbtLog->Dock = System::Windows::Forms::DockStyle::Fill;
			this->rbtLog->Location = System::Drawing::Point(3, 3);
			this->rbtLog->Name = L"rbtLog";
			this->rbtLog->ReadOnly = true;
			this->rbtLog->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
			this->rbtLog->Size = System::Drawing::Size(1668, 410);
			this->rbtLog->TabIndex = 0;
			this->rbtLog->Text = L"[Time] Operation | PID/TID | Result\n";
			// 
			// statusStrip1
			// 
			this->statusStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->lblTotalProcesses,
					this->lblVisibleProcesses, this->lblLogicalCpu, this->lblAccessLevel
			});
			this->statusStrip1->Location = System::Drawing::Point(0, 927);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(1582, 26);
			this->statusStrip1->TabIndex = 3;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// lblTotalProcesses
			// 
			this->lblTotalProcesses->Name = L"lblTotalProcesses";
			this->lblTotalProcesses->Size = System::Drawing::Size(124, 20);
			this->lblTotalProcesses->Text = L"Total Processes: 0";
			// 
			// lblVisibleProcesses
			// 
			this->lblVisibleProcesses->Name = L"lblVisibleProcesses";
			this->lblVisibleProcesses->Size = System::Drawing::Size(68, 20);
			this->lblVisibleProcesses->Text = L"Visible: 0";
			// 
			// lblLogicalCpu
			// 
			this->lblLogicalCpu->Name = L"lblLogicalCpu";
			this->lblLogicalCpu->Size = System::Drawing::Size(103, 20);
			this->lblLogicalCpu->Text = L"Logical CPU: 0";
			// 
			// lblAccessLevel
			// 
			this->lblAccessLevel->Name = L"lblAccessLevel";
			this->lblAccessLevel->Size = System::Drawing::Size(84, 20);
			this->lblAccessLevel->Text = L"Mode: User";
			// 
			// timerRefresh
			// 
			this->timerRefresh->Enabled = true;
			this->timerRefresh->Interval = 1000;
			this->timerRefresh->Tick += gcnew System::EventHandler(this, &Form_OS_Lab2::timerRefresh_Tick_1);
			// 
			// Form_OS_Lab2
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1582, 953);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->splitMain);
			this->Controls->Add(this->pnlTopControls);
			this->Name = L"Form_OS_Lab2";
			this->Text = L"Form_OS_Lab2";
			this->Load += gcnew System::EventHandler(this, &Form_OS_Lab2::Form_OS_Lab2_Load);
			this->pnlTopControls->ResumeLayout(false);
			this->pnlTopControls->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numInterval))->EndInit();
			this->splitMain->Panel1->ResumeLayout(false);
			this->splitMain->Panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitMain))->EndInit();
			this->splitMain->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvProcesses))->EndInit();
			this->ctxProcessMenu->ResumeLayout(false);
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dgvThreads))->EndInit();
			this->ctxThreadMenu->ResumeLayout(false);
			this->tabPage2->ResumeLayout(false);
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
public:
		void RefreshProcessList();
		void RefreshThreadList(int pid);
private: System::Void btnLaunch_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void refresh_button_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void Stop_Resume_Updating_button_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void numInterval_ValueChanged(System::Object^ sender, System::EventArgs^ e);
private: System::Void timerRefresh_Tick_1(System::Object^ sender, System::EventArgs^ e);
private: System::Void search_button_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void reset_filter_button_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void chkSelectedPidOnly_CheckedChanged(System::Object^ sender, System::EventArgs^ e);

	   //==Process Menu Strips==
private: System::Void dgvProcesses_ColumnHeaderMouseClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^ e);
private: System::Void SelectCoresCPUToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void showProcessesThreadsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void SuspendToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void resumeToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void TerminateProcessToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void dgvProcesses_CellMouseDown(System::Object^ sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^ e);

private: System::Void highToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void aboveNormalToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void normalToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void belowNormalToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void idleToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);

	   //==Threads Menu Strips==
private: System::Void dgvThreads_ColumnHeaderMouseClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^ e);
private: System::Void SuspendThreadToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void resumeThreadToolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void TerminateThreadToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void threadAffinityToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void dgvThreads_CellMouseDown(System::Object^ sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^ e);

private: System::Void highest2ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void aboveNormal1ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void normal0ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void belowNormal1ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void lowest2ToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
private: System::Void Search_Process_textBox_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);

private: System::Void Form_OS_Lab2_Load(System::Object^ sender, System::EventArgs^ e) {
}
};
}