#ifndef MYGLCANVAS_H
#define MYGLCANVAS_H

//#undef _DEBUG
#include "wx/glcanvas.h"
#include "ImageAbs.h"
#include "brush.h"

typedef std::vector <std::pair <Coord,Coord> > CorrVec;

// Define a new frame type
class MyGLCanvas: public wxGLCanvas
{
  //friend class BaseGLViewer;
 public:
  MyGLCanvas(wxWindow *parent, const wxWindowID id = -1, 
	     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
	     long style = 0, const wxString& name = "MyGLCanvas", int* attriblist = NULL);

   
  ~MyGLCanvas(void);

  void OnPaint(wxPaintEvent& event);
  void OnSize(wxSizeEvent& event);
  void OnEraseBackground(wxEraseEvent& event);

  void MouseDown(wxMouseEvent& event);
  void RightMouseDown(wxMouseEvent& event); 
  void MouseUp(wxMouseEvent& event);
  void MouseMotion(wxMouseEvent& event);

  void InitGL(void);

  void redrawRendered();
  void MyRefresh();

  void SetViewImage(const ImageAbs* ia);
  void SetOverlayImage(const ImageAbs* ia);
 
  void enableBrushing() { _brushingEnabled = TRUE;}
  void disableBrushing() { _brushingEnabled = FALSE; }

  // frankenface
  void viewOverlay();
  void dontViewOverlay();
  bool viewingOverlay() const { return _viewOverlay; }
  
  void initStep() { ++_inited;}
  void incInitLevel() { ++_initLevel; }

  Brush* getCurrBrush() { return _currBrush; }

  unsigned char* getCummOverlay() { return _cummOverlay; }
  const unsigned char* getCummOverlay() const { return _cummOverlay; }
  void buildCummOverlay();
  void viewCummOverlay(const bool b);
  void setCompDim(const int w, const int h) { _compw = w; _comph = h; }
  //void blendCummOverlay(const bool b) { _blendCummOverlay = b; } // for sorted stack viewer regions

  //void setBrushType(const StrokeType st) { _brushType = st; }
  void setBrushEraser(const bool b) { _brushIsEraser = b; }

  // mouse info
  int mx() const { return _mx;}  
  int my() const { return _my;}
  bool mouseDown() const { return _mouseDown;}
  bool altDown() const { return _altDown; }
  bool ctrlDown() const { return _ctrlDown; }
  bool rightMDown () const { return _rightMDown; }
  void madeSelection () { _rightMDown = false; }

  void setBrushColor (Vec3f col) { _brushColor = col; }

  void renderCorrPoints (const CorrVec* cps, int which);
  void dontRenderCorrPoints();

 private:
  void myGlLoadIdentity();
  void drawImage(const ImageAbs* ia, const float a = 1.f);
  void drawCummOverlay();

  bool   _brushingEnabled;
  int _w, _h;
  int _mx, _my;
  bool _mouseDown, _altDown, _ctrlDown, _rightMDown;
  const ImageAbs *_viewImage, *_overlayImage; // overlayImage for frankenface
  bool _viewOverlay;
  Brush* _currBrush;
  //StrokeType _brushType;
  bool _brushIsEraser;
  //unsigned char* _rendered; 
  bool _myRefresh;
  int _inited, _initLevel; 
  int _constructed;

  unsigned char* _cummOverlay; // RGBA image, for strokes and sometime labeling
  int _compw, _comph; // composite w,h, dimensions of _cummOverlay
  bool _viewCummOverlay;
  //bool _blendCummOverlay; // for sorted stack viewer regions

  const CorrVec* _currPoints;
  int _whichCorr;

  bool _initGL;
  Vec3f _brushColor;
  DECLARE_EVENT_TABLE()
    };


    BEGIN_DECLARE_EVENT_TYPES()
     DECLARE_EVENT_TYPE(wxEVT_STROKE, 7777)
     DECLARE_EVENT_TYPE(wxEVT_MYMOUSE_MOVE, 7778)
     DECLARE_EVENT_TYPE(wxEVT_MYMOUSE_DOWN, 7779)
     DECLARE_EVENT_TYPE(wxEVT_MYRMOUSE_DOWN, 7780)
     END_DECLARE_EVENT_TYPES()


#endif
