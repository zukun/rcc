#ifndef CONSTANTEDITOR_H
#define CONSTANTEDITOR_H

//#undef _DEBUG
#include <wx/wx.h>
#include "graphcuts/dialCut.h"
//#include "graphcuts/holeFillCut.h"

//--------------------


class wxFloatValidator : public wxValidator {

 public:
  
  wxFloatValidator(float* val);
  wxFloatValidator(const wxFloatValidator& o);
  bool Validate(wxWindow*);
  bool TransferToWindow();
  bool TransferFromWindow();
  wxObject* Clone() const;

 private:
  float* _val;

};

class wxFloatCtrl : public wxTextCtrl {
 public:
  wxFloatCtrl(wxWindow* parent, const wxValidator& validator = wxDefaultValidator);  
};

//--------------------

class wxIntValidator : public wxValidator {

 public:
  
  wxIntValidator(int* val);
  wxIntValidator(const wxIntValidator& o);

  bool Validate(wxWindow*);
  bool TransferToWindow();
  bool TransferFromWindow();
  wxObject* Clone() const;

 private:
  int* _val;

};

class wxIntCtrl : public wxTextCtrl {
 public:
  wxIntCtrl(wxWindow* parent, const wxValidator& validator = wxDefaultValidator);  
};

//--------------------

class ConstantEditor : public wxDialog {

 public:

  ConstantEditor(wxWindow* parent, DialCut *dc);

  void handleCuttype(wxCommandEvent& evt);

  void setSelectorRight();
  
  float _edgeThres;
  DialCut *_dc;
  int _currSel;
  wxChoice* _choice;

  DECLARE_EVENT_TABLE()
};



#endif
