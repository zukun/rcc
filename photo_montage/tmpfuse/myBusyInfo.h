#ifndef MYBUSYINFO_H
#define MYBUSYINFO_H

#include <wx/wx.h>

class MyBusyInfo : public wxDialog {

 public:
  MyBusyInfo(wxWindow *parent);

  void OnButton(wxCommandEvent& evt);

 private:

  wxWindow* _parent;
  DECLARE_EVENT_TABLE();
  bool _pressed;
};

BEGIN_DECLARE_EVENT_TYPES()
     DECLARE_EVENT_TYPE(INTERRUPT_GLOBAL_COMPUTE, 9999)
END_DECLARE_EVENT_TYPES()

#endif
