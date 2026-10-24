#pragma once
#include <windows.h>

namespace OSLab2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for AffinityForm
	/// </summary>
	public ref class AffinityForm : public System::Windows::Forms::Form
	{
	public:
		AffinityForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			ResultAffinityMask = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AffinityForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::CheckedListBox^ CheckBoxCores;
	private: System::Windows::Forms::Button^ btnOk;
	private: System::Windows::Forms::Button^ btnCancel;
	public: DWORD_PTR ResultAffinityMask;
	private: System::Windows::Forms::Button^ ToggleAllbutton;
	public:

	public:
	protected:



	protected:

	protected:


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->CheckBoxCores = (gcnew System::Windows::Forms::CheckedListBox());
			this->btnOk = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->ToggleAllbutton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// CheckBoxCores
			// 
			this->CheckBoxCores->CheckOnClick = true;
			this->CheckBoxCores->FormattingEnabled = true;
			this->CheckBoxCores->Items->AddRange(gcnew cli::array< System::Object^  >(16) {
				L"CPU 0", L"CPU 1", L"CPU 2", L"CPU 3", L"CPU 4",
					L"CPU 5", L"CPU 6", L"CPU 7", L"CPU 8", L"CPU 9", L"CPU 10", L"CPU 11", L"CPU 12", L"CPU 13", L"CPU 14", L"CPU 15"
			});
			this->CheckBoxCores->Location = System::Drawing::Point(3, 12);
			this->CheckBoxCores->Name = L"CheckBoxCores";
			this->CheckBoxCores->Size = System::Drawing::Size(311, 378);
			this->CheckBoxCores->TabIndex = 0;
			// 
			// btnOk
			// 
			this->btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnOk->Location = System::Drawing::Point(12, 443);
			this->btnOk->Name = L"btnOk";
			this->btnOk->Size = System::Drawing::Size(82, 41);
			this->btnOk->TabIndex = 1;
			this->btnOk->Text = L"OK";
			this->btnOk->UseVisualStyleBackColor = true;
			this->btnOk->Click += gcnew System::EventHandler(this, &AffinityForm::btnOk_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnCancel->Location = System::Drawing::Point(101, 443);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(94, 41);
			this->btnCancel->TabIndex = 2;
			this->btnCancel->Text = L"Cancel";
			this->btnCancel->UseVisualStyleBackColor = true;
			// 
			// ToggleAllbutton
			// 
			this->ToggleAllbutton->Location = System::Drawing::Point(201, 443);
			this->ToggleAllbutton->Name = L"ToggleAllbutton";
			this->ToggleAllbutton->Size = System::Drawing::Size(97, 41);
			this->ToggleAllbutton->TabIndex = 3;
			this->ToggleAllbutton->Text = L"Toggle All";
			this->ToggleAllbutton->UseVisualStyleBackColor = true;
			this->ToggleAllbutton->Click += gcnew System::EventHandler(this, &AffinityForm::ToggleAllbutton_Click);
			// 
			// AffinityForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(326, 496);
			this->Controls->Add(this->ToggleAllbutton);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnOk);
			this->Controls->Add(this->CheckBoxCores);
			this->Name = L"AffinityForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"AffinityForm";
			this->ResumeLayout(false);

		}
#pragma endregion
	public: System::Void btnOk_Click(System::Object^ sender, System::EventArgs^ e);
		   void SetCurrentMask(DWORD_PTR mask, int logicalCores);
private: System::Void ToggleAllbutton_Click(System::Object^ sender, System::EventArgs^ e);
};
}
