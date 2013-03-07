#include "constantEditor.h"
#include <wx/valgen.h>


wxFloatValidator::wxFloatValidator(float* val) : _val(val) {
}

bool wxFloatValidator::Validate(wxWindow*) {
  wxFloatCtrl* ctrl = (wxFloatCtrl*) GetWindow();
  wxString str = ctrl->GetValue();
  double res;
  bool ok = str.ToDouble(&res);
  //printf("validating %f\n",res);
  if (!ok) {
    wxLogWarning(wxT("I don't like your sorry excuse for a number!  Fix it."));
    return false;
  }
  else
    return true;
}

bool wxFloatValidator::TransferToWindow() {
  wxFloatCtrl* ctrl = (wxFloatCtrl*) GetWindow();
  wxString str;
  str << *_val;
  ctrl->SetValue(str);
 return true;
}

bool wxFloatValidator::TransferFromWindow() {
  wxFloatCtrl* ctrl = (wxFloatCtrl*) GetWindow();
  wxString str = ctrl->GetValue();
  double res;
  bool ok = str.ToDouble(&res);
  if (!ok)
    return false;

  //printf("transferring %f\n",res);
  *_val = (float) res;
 return true;
}

wxObject*  wxFloatValidator::Clone() const {
  return  new wxFloatValidator(_val);
}

wxFloatValidator::wxFloatValidator(const wxFloatValidator& o) {
  _val = o._val;
}

wxFloatCtrl::wxFloatCtrl(wxWindow* parent, const wxValidator& validator) : 
wxTextCtrl(parent,-1,"",wxDefaultPosition,wxDefaultSize,0,validator) {
}


//---------------------



wxIntValidator::wxIntValidator(int* val) : _val(val) {
}

wxIntValidator::wxIntValidator(const wxIntValidator& o) {
  _val = o._val;
}

bool wxIntValidator::Validate(wxWindow*) {
  wxIntCtrl* ctrl = (wxIntCtrl*) GetWindow();
  wxString str = ctrl->GetValue();
  long res;
  bool ok = str.ToLong(&res);
  printf("validating %li\n",res);
  if (!ok) {
    wxLogWarning(wxT("I don't like your sorry excuse for a number!  Fix it."));
    return false;
  }
  else
    return true;
}

bool wxIntValidator::TransferToWindow() {
  wxIntCtrl* ctrl = (wxIntCtrl*) GetWindow();
  wxString str;
  str << *_val;
  ctrl->SetValue(str);
 return true;
}

bool wxIntValidator::TransferFromWindow() {
  wxIntCtrl* ctrl = (wxIntCtrl*) GetWindow();
  wxString str = ctrl->GetValue();
  long res;
  bool ok = str.ToLong(&res);
  if (!ok)
    return false;

  printf("transferring %li\n",res);
  *_val = (int) res;
 return true;
}

wxObject*  wxIntValidator::Clone() const {
  return  new wxIntValidator(_val);
}

wxIntCtrl::wxIntCtrl(wxWindow* parent, const wxValidator& validator) : 
wxTextCtrl(parent,-1,"",wxDefaultPosition,wxDefaultSize,0,validator) {
}

//---------------------

BEGIN_EVENT_TABLE(ConstantEditor, wxDialog)
  EVT_BUTTON(wxID_OK, ConstantEditor::OnApply)
  EVT_CHOICE(-1, ConstantEditor::handleCuttype)
END_EVENT_TABLE()


ConstantEditor::ConstantEditor(wxWindow* parent, DialCut *dc) : 
  wxDialog(parent,-1,"ConstantEditor",wxDefaultPosition, wxSize(300,300)), _dc(dc) {
  
  wxGridSizer *sizer = new wxGridSizer(2,5,5);
  SetSizer(sizer);
  _edgeThres = 100.;
  
  sizer->Add(new wxStaticText(this,-1, "Inertia:"));
  sizer->Add(new wxFloatCtrl(this, wxFloatValidator(&(dc->_inertia))));

  sizer->Add(new wxStaticText(this,-1, "Potts:"));
  sizer->Add(new wxFloatCtrl(this, wxFloatValidator(&(dc->_potts))));

  sizer->Add(new wxStaticText(this,-1, "Seam:"));
  sizer->Add(new wxFloatCtrl(this, wxFloatValidator(&(dc->_regular_inter))));

  sizer->Add(new wxStaticText(this,-1, "Brightness:"));
  sizer->Add(new wxFloatCtrl(this, wxFloatValidator(&(dc->_bright))));

  sizer->Add(new wxStaticText(this,-1, "Edge Threshold:"));
  sizer->Add(new wxFloatCtrl(this, wxFloatValidator(&_edgeThres)));

  sizer->Add(new wxStaticText(this,-1, "Seam Cost Type:"));
  wxString strs[4] = {wxT("Image only"), wxT("Image + Gradient"), wxT("Gradients only"), wxT("Both")};
  _choice = new wxChoice(this,-1,wxDefaultPosition, wxDefaultSize, 4,strs, 0);
  setSelectorRight();
  sizer->Add(_choice);

  sizer->Add(new wxStaticText(this,-1, "Num Dial:"));
  sizer->Add(new wxIntCtrl(this, wxIntValidator(&dc->_numDial)));

  sizer->Add(new wxStaticText(this,-1, "Force Stroke:"));
  sizer->Add(new wxCheckBox(this, -1, "", wxDefaultPosition, wxDefaultSize, 0, 
			    wxGenericValidator(&(dc->_forceStroke))));

  sizer->Add(new wxStaticText(this,-1, "Multi-region brush:"));
  sizer->Add(new wxCheckBox(this, -1, "", wxDefaultPosition, wxDefaultSize, 0, 
			    wxGenericValidator(&(dc->_multiRegion))));

  sizer->Add(new wxButton(this, wxID_OK, "Apply"));

}

void ConstantEditor::setSelectorRight() {
  if (_dc->getCuttype() == C_NORMAL) {
    _choice->SetSelection(0);
	_currSel = 0;
  }
  else if (_dc->getCuttype() == C_GRAD) {
    _choice->SetSelection(1);
	_currSel = 1;
  }
  else if (_dc->getCuttype() == C_GRAD2) {
    _choice->SetSelection(2);
	_currSel = 2;
  }
  else {
    _choice->SetSelection(3);
	_currSel = 3;
  }
}


void ConstantEditor::handleCuttype(wxCommandEvent& evt) {
  _currSel = evt.GetSelection();
  if (_currSel==0){
    _dc->setCuttype(C_NORMAL);
  }
  else if (_currSel==1) {
    _dc->setCuttype(C_GRAD);
  }
  else if (_currSel==2) {
    _dc->setCuttype(C_GRAD2);
  }
  else if (_currSel==3) {
    _dc->setCuttype(C_BOTH);
  }
  else
    assert(0);
}
