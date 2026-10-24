#include "AffinityForm.h"
using namespace System::Windows::Forms;

namespace OSLab2 {
    // Встановлення галочок на основі поточної маски
    void AffinityForm::SetCurrentMask(DWORD_PTR mask, int logicalCores) {
        for (int i = 0; i < logicalCores && i < CheckBoxCores->Items->Count; i++) {
            bool isSet = (mask & (1ULL << i)) != 0;
            CheckBoxCores->SetItemChecked(i, isSet);
        }
    }
    System::Void AffinityForm::ToggleAllbutton_Click(System::Object^ sender, System::EventArgs^ e) {
        bool allChecked = true;
        for (int i = 0; i < CheckBoxCores->Items->Count; i++) {
           if (!CheckBoxCores->GetItemChecked(i)) {
               allChecked = false;
               break;
          }
    }

     // Якщо всі вибрані - знімаємо всі. Якщо хоча б одна не вибрана - вибираємо всі.
      bool newState = !allChecked;
      for (int i = 0; i < CheckBoxCores->Items->Count; i++) {
          CheckBoxCores->SetItemChecked(i, newState);
     }
    }
    System::Void AffinityForm::btnOk_Click(System::Object^ sender, System::EventArgs^ e) {
    ResultAffinityMask = 0;
    // Зчитування маски з галочок
    for (int i = 0; i < CheckBoxCores->Items->Count; i++) {
        if (CheckBoxCores->GetItemChecked(i)) {
            ResultAffinityMask |= (1ULL << i);
        }
    }
    // Маска має бути ненульовою[cite: 15]
    if (ResultAffinityMask == 0) {
        MessageBox::Show("Error: mask can`t be empty!", "Error");
        this->DialogResult = System::Windows::Forms::DialogResult::None;
    }
}
}

