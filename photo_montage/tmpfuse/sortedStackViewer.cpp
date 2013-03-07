#include "sortedStackViewer.h"

DEFINE_EVENT_TYPE(SS_CANCEL_STROKE)
DEFINE_EVENT_TYPE(SS_INERTIA_CHANGED)
DEFINE_EVENT_TYPE(SS_COMP_CHANGED)
DEFINE_EVENT_TYPE(SS_BRIGHTNESS_CHANGED)
  //DEFINE_EVENT_TYPE(SS_TAKE_STROKE)

BEGIN_EVENT_TABLE(SortedStackViewer, wxFrame)
  EVT_SPINCTRL(ID_ZCHANGE2, SortedStackViewer::zChange)
//  EVT_CLOSE(SortedStackViewer::OnClose)
  EVT_CHECKBOX(ID_VIEW_INDICATOR, SortedStackViewer::ToggleViewIndicator)
  EVT_CHECKBOX(ID_VIEWSTROKE_INDICATOR, SortedStackViewer::ToggleViewStrokeIndicator)
#ifdef __WXMSW__
  EVT_COMMAND_SCROLL_THUMBRELEASE(ID_INERTIA_SLIDER, SortedStackViewer::SliderChanged)
  EVT_COMMAND_SCROLL_THUMBRELEASE(ID_BRIGHTNESS_SLIDER, SortedStackViewer::BrightnessSliderChanged)
#else
  EVT_COMMAND_SCROLL_THUMBTRACK(ID_INERTIA_SLIDER, SortedStackViewer::SliderChanged)
  EVT_COMMAND_SCROLL_THUMBTRACK(ID_BRIGHTNESS_SLIDER, SortedStackViewer::BrightnessSliderChanged)
#endif
  EVT_COMMAND_SCROLL_LINEUP(ID_INERTIA_SLIDER, SortedStackViewer::SliderChanged)
  EVT_COMMAND_SCROLL_LINEDOWN(ID_INERTIA_SLIDER, SortedStackViewer::SliderChanged)
  EVT_COMMAND_SCROLL_LINEUP(ID_BRIGHTNESS_SLIDER, SortedStackViewer::BrightnessSliderChanged)
  EVT_COMMAND_SCROLL_LINEDOWN(ID_BRIGHTNESS_SLIDER, SortedStackViewer::BrightnessSliderChanged)
END_EVENT_TABLE()

SortedStackViewer::SortedStackViewer(StackViewer* oStack, const short* stencil, wxFrame *frame, const wxString& title, 
				     const wxPoint& pos, const wxSize& size) : BaseGLViewer(frame,title,pos,size) {
  _w = oStack->width();
  _h = oStack->height();
  _count = 0;
  assert(_count = oStack->count());
  _stencil = NULL;
  
  CreateStatusBar();
  //  SetStatusText( "Aseem is nosy!" );
}

SortedStackViewer::~SortedStackViewer() {
  //printf("I'm being deleted!  Cool, huh?\n");
  for (uint i=0; i<_masks.size(); ++i)
    delete[] (_masks[i]);
  if (_stencil) delete[] _stencil;
}
/*
void SortedStackViewer::OnClose(wxCloseEvent evt) {
  //wxCommandEvent eventCustom(SS_CANCEL_STROKE);
  //ProcessEvent(eventCustom);
  Destroy();
}
*/

void SortedStackViewer::updateCummOverlay() {
  bool view = _viewIndicator->GetValue() || _viewStrokeIndicator->GetValue();
  m_canvas->viewCummOverlay(view);
  if (view)
    maskToCummOverlay(_currImage);
}

void SortedStackViewer::ToggleViewIndicator(wxCommandEvent& evt) {
  updateCummOverlay();
  m_canvas->Refresh();
}

void SortedStackViewer::ToggleViewStrokeIndicator(wxCommandEvent& evt) {
  updateCummOverlay();
  m_canvas->Refresh();
}

void SortedStackViewer::finishInit(std::vector<CutResult>& cr, const float inertia) {
  //printf("sorted finishInit\n");
  _spin = new wxSpinCtrl(this, ID_ZCHANGE2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
			 0, cr.size()-1, 0);
  
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *controlSizer = new wxBoxSizer(wxHORIZONTAL);
  controlSizer->Add(_spin,0,wxALIGN_LEFT,5);

  _viewIndicator = new wxCheckBox(this, ID_VIEW_INDICATOR, "View cut region ");
  _viewIndicator->SetValue(true);
  controlSizer->Add(_viewIndicator, 0, wxALIGN_LEFT, 5);

 _viewStrokeIndicator = new wxCheckBox(this, ID_VIEWSTROKE_INDICATOR, "View stroke");
  _viewStrokeIndicator->SetValue(false);
  controlSizer->Add(_viewStrokeIndicator, 0, wxALIGN_LEFT, 5);

  wxButton* takeCut = new wxButton(this, ID_TAKE_CUT, "Take this cut");
  controlSizer->Add(takeCut,0,wxALIGN_LEFT, 5);
  wxButton* cancelCut = new wxButton(this, ID_CANCEL_CUT, "Cancel");
  controlSizer->Add(cancelCut,0,wxALIGN_LEFT, 5);

  _inertiaSlider = new wxSlider(this, ID_INERTIA_SLIDER, 1/*int(inertia*100.f+.5)*/, 0, 10, wxDefaultPosition, 
				wxDefaultSize, wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
  _inertiaSlider->SetTickFreq(1, 0);

#ifdef _WXMSW_
  controlSizer->Add(_inertiaSlider, 0, wxALIGN_LEFT, 5);
#else
  controlSizer->Add(_inertiaSlider, wxEXPAND, 5);
#endif

/*  _brightnessSlider = new wxSlider(this, ID_BRIGHTNESS_SLIDER, 0, -5, 5, wxDefaultPosition, 
				wxDefaultSize, wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS);
  _brightnessSlider->SetTickFreq(1, 0);
#ifdef _WXMSW_
  controlSizer->Add(_brightnessSlider, 0, wxALIGN_LEFT, 5);
#else
  controlSizer->Add(_brightnessSlider, wxEXPAND, 5);
#endif
*/
  sizer->Add(controlSizer, 0, wxEXPAND, 5);
  sizer->Add(m_canvas,1,wxGROW, 5);
  SetSizer( sizer );      // use the sizer for layout

  std::vector<CutResult>::const_iterator c;
  for (c=cr.begin(); c!= cr.end(); ++c) {
    _images.push_back(c->_im);
    _masks.push_back(c->_mask);
    _nums.push_back(c->_num);
  }
  _count = _images.size(); 
  assert(_count > 0);

  this->m_canvas->setCompDim(_w, _h);
  this->m_canvas->buildCummOverlay();

  _spin->SetValue(0);
  setCurrImage(0);
  m_canvas->viewCummOverlay(true);
  //m_canvas->blendCummOverlay(true);
  m_canvas->initStep();

  resize();
}

void SortedStackViewer::takeRefinementStroke(std::vector<CutResult>& cr) {
  assert(cr.size() == 1 && cr[0]._num == _nums[_currImage]);
  delete[] _masks[_currImage];
  _masks[_currImage] = cr[0]._mask;
  updateCummOverlay();

  m_canvas->Refresh ();

//  wxCommandEvent eventCustom(SS_COMP_CHANGED);
  //ProcessEvent(eventCustom);
}

void SortedStackViewer::zChange(wxSpinEvent& evt) {
  //printf("Sorted Spin event\n");
  setCurrImage(evt.GetPosition());
  //m_canvas->redrawRendered();
  m_canvas->Refresh();

  wxCommandEvent eventCustom(SS_COMP_CHANGED);
  ProcessEvent(eventCustom);

  evt.Skip();
}

void SortedStackViewer::setCurrImage(const int i) {
  assert(i>=0 && i<_count);
  _currImage = i;
  maskToCummOverlay(i);
  SetViewImage(_images[i]);
}

void SortedStackViewer::maskToCummOverlay(const int c) {
  assert(c>=0 && c< (int)_masks.size());
  
  int i,j,index=0, index4=0;
  unsigned char* cummOverlay = this->m_canvas->getCummOverlay();
  const bool* mask = _masks[c];
  bool viewStroke = _viewStrokeIndicator->GetValue();
  bool viewMask = _viewIndicator->GetValue();
  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index, index4+=4) {
      if (_stencil[index] && viewStroke) {
	cummOverlay[index4+1] = 255;
	cummOverlay[index4+3] = 255;
      }
      else if (viewMask && mask[index]) {
	cummOverlay[index4+1] = 0;
	cummOverlay[index4+3] = 128;
      }
      else {
	cummOverlay[index4+1] = 0;
	cummOverlay[index4+3] = 0;
      }
    }
}


void SortedStackViewer::resize() {
	int x, y;
	_inertiaSlider->GetSize(&x, &y);
    SetClientSize(_images[0]->getWidth(), _images[0]->getHeight() + y + 2);
}

const bool* SortedStackViewer::getMask(const int i) const {
  assert(i>=0 && i<(int)_masks.size());
  return _masks[i];
}

void SortedStackViewer::copyBrushStencil(const short* s) {
  if (!_stencil)
    _stencil = new short[_w*_h];
  memcpy(_stencil, s, sizeof(short)*_w*_h);
}


void SortedStackViewer::SliderChanged(wxScrollEvent& evt) {
  printf("slider changed, sortedstackviewer\n");
  wxCommandEvent eventCustom(SS_INERTIA_CHANGED);
  ProcessEvent(eventCustom);

}

void SortedStackViewer::BrightnessSliderChanged(wxScrollEvent& evt) {
  printf("brightness slider changed, sortedstackviewer\n");
  wxCommandEvent eventCustom(SS_BRIGHTNESS_CHANGED);
  ProcessEvent(eventCustom);

}

float SortedStackViewer::getInertiaValue() const {
  int val = _inertiaSlider->GetValue();
  if (val==0) return 0;
  if (val==1) return .005;
  if (val==2) return .01;
  return (float(val-1)*.01);
  return ( float(_inertiaSlider->GetValue()) * .01 );
}

float SortedStackViewer::getBrightnessValue() const {
	return _brightnessSlider->GetValue ();
}
