#include "myBusyInfo.h"

#define B_INTERRUPT 14000
DEFINE_EVENT_TYPE (INTERRUPT_GLOBAL_COMPUTE);

BEGIN_EVENT_TABLE(MyBusyInfo, wxDialog)
EVT_BUTTON(B_INTERRUPT, MyBusyInfo::OnButton)
END_EVENT_TABLE()

MyBusyInfo::MyBusyInfo(wxWindow *parent) 
: wxDialog(parent, -1, wxString("Global Compute")) {

  _parent = parent;
   wxBoxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);

   sizerTop->Add(new wxStaticText(this, -1, "Performing long computation.  Click button to"));
   sizerTop->Add(new wxStaticText(this, -1, "interrupt and accept the current result."));
   wxButton *btn = new wxButton(this, B_INTERRUPT, _T("Interrupt"));
   sizerTop->Add(btn);
   SetAutoLayout(TRUE);
   SetSizer(sizerTop);
   //sizerTop->SetSizeHints(this);
   sizerTop->SetMinSize(200, 100);
   sizerTop->Fit(this);
   _pressed = false;
}

void MyBusyInfo::OnButton(wxCommandEvent& evt) {
  if (_pressed) return; // double-click
  _pressed = true;
   wxCommandEvent evt2(INTERRUPT_GLOBAL_COMPUTE);
   wxPostEvent(_parent,evt2);

   // should get destroyed by finishGlobalCompute()
   //Destroy();
}


