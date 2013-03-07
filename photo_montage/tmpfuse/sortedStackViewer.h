#ifndef SORTEDSTACKVIEWER_H
#define SORTEDSTACKVIEWER_H

#include "stackViewer.h"
#include "graphcuts/dialCut.h"

#define ID_ZCHANGE2			21000
#define ID_TAKE_CUT			21001
#define ID_VIEW_INDICATOR	21002
#define ID_CANCEL_CUT		21003
#define ID_VIEWSTROKE_INDICATOR	21004
#define ID_INERTIA_SLIDER       21005
#define ID_BRIGHTNESS_SLIDER       21006

class SortedStackViewer : public BaseGLViewer {

 public:
  
  SortedStackViewer(StackViewer* oStack, const short* stencil, wxFrame *frame, const wxString& title, 
		    const wxPoint& pos, const wxSize& size);

  ~SortedStackViewer();

//  void OnClose(wxCloseEvent evt);
  void ToggleViewIndicator(wxCommandEvent& evt);
  void ToggleViewStrokeIndicator(wxCommandEvent& evt);
  void SliderChanged(wxScrollEvent& evt);
  void BrightnessSliderChanged(wxScrollEvent& evt);

  void finishInit(std::vector<CutResult>& cr, const float inertia);
  void takeRefinementStroke(std::vector<CutResult>& cr);

  void zChange(wxSpinEvent& evt);
  void setCurrImage(const int i);
  int getCurrImage() const { return _currImage; }
  int getCurrNum() const { return _nums[_currImage]; }

  const bool* getMask(const int i) const;

  void resize();

  void copyBrushStencil(const short* s);
  const short* getBrushStencil() const { return _stencil; }

  float getInertiaValue() const;
  float getBrightnessValue() const;

  void setCurrNum (int ind, ImageAbs* img) { 
    _prevInd = _nums[_currImage];
    _prevImg = _images[_currImage];
    _nums[_currImage] = ind; 
    _images[_currImage] = img; 
  }

  void revert (int ind) {

    for (int i=0; i < _count; i++) {
      if (_nums[i] == ind) {
	_nums[i] = _prevInd;
	_images[i] = _prevImg;
	return;
      }
    }
    assert(false);
  }

 private:

  void maskToCummOverlay(const int i);
  void updateCummOverlay();

  wxSpinCtrl* _spin;
  std::vector<ImageAbs*> _images;
  int _count;
  int _currImage;
  int _w, _h; // composite space
  
  std::vector<int> _nums;
  std::vector<bool*> _masks;
  
  wxCheckBox* _viewIndicator;
  wxCheckBox* _viewStrokeIndicator;

  wxSlider* _inertiaSlider;
  wxSlider* _brightnessSlider;
  
  int _prevInd;
  ImageAbs* _prevImg;
  short* _stencil;  // brush mask, we hold this and don't really use it (CompViewer does)
  
  

  DECLARE_EVENT_TABLE()
};


BEGIN_DECLARE_EVENT_TYPES()
     DECLARE_EVENT_TYPE(SS_CANCEL_STROKE, 6666)
     DECLARE_EVENT_TYPE(SS_INERTIA_CHANGED, 6667)
     DECLARE_EVENT_TYPE(SS_COMP_CHANGED, 6668)
     DECLARE_EVENT_TYPE(SS_BRIGHTNESS_CHANGED, 6669)
END_DECLARE_EVENT_TYPES()

     //DECLARE_EVENT_TYPE(SS_TAKE_STROKE, 6667)

#endif
