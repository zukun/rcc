#ifndef COMPVIEWER_H
#define COMPVIEWER_H
//#undef _DEBUG
//#include <deque>
#include <wx/wx.h>
#include <wx/image.h>
#include "baseGLViewer.h"
#include "stackViewer.h"
#include "sortedStackViewer.h"
#include "globFunc.h"
#include "graphcuts/dialCut.h"
#include "blending/blend.h"
#include "constantEditor.h"
#include "paintCheck.h"
#include "graphcuts/histogram.h"
#include "myBusyInfo.h"
//#include <wx/busyinfo.h>

#define C_COLORICONSIZE 15
#define C_POPUPICONSIZE 10

// 1 for simplified interface, 0 for pro
#define SIMPLE_INTERFACE 1

enum WhichView {W_SIMPCOMP, W_LABELS, W_BLEND, W_DATA, W_INTER, W_CUSTOM, W_LABELS_STROKES, W_LABELS_ALPHA, W_STROKES_ONLY};
//enum BrushType {B_HIGHLIGHT, B_SHADOW, B_COLORCLONE, B_IMAGE, B_ERASE};

// T_BRUSH encompasses all of StrokeType
enum ToolType { T_BRUSH, T_TRANSLATE, T_ROTATE, T_SCALE, T_POINT  };

class MyStatusBar : public wxStatusBar {

 public:
  MyStatusBar(wxWindow* parent) : wxStatusBar(parent, -1) {}
  void OnSize(wxSizeEvent& event);
  wxStaticBitmap _colorIconStaticBmp;
  DECLARE_EVENT_TABLE()
    };

    class CompViewer: public BaseGLViewer {
    public:

      CompViewer(const wxString& title, const wxPoint& pos, const wxSize& size);
      ~CompViewer();
      void finishInit();

      //--------------
      // events:
      void NewComposite(wxCommandEvent& event);
      void LoadStack(wxCommandEvent& event);
      void SaveImage(wxCommandEvent& event);
      void SaveComp(wxCommandEvent& event);
      void SaveBlend(wxCommandEvent& event);

      void SaveLabeling(wxCommandEvent& event);
      void LoadLabeling(wxCommandEvent& event);
      void SaveMichael(wxCommandEvent& event);
      void SaveSources(wxCommandEvent& event);
      void OnExit(wxCommandEvent& event);

      void DefaultFamilySettings (wxCommandEvent& event);
      void DefaultFaceSettings (wxCommandEvent& event);
      void DefaultBackgSettings (wxCommandEvent& event);
      void DefaultEdofSettings (wxCommandEvent& event);
      void DefaultSettings (wxCommandEvent& event);

      void Undo(wxCommandEvent* event);
      void Redo(wxCommandEvent* event);

      void ViewCeditor(wxCommandEvent& event);
      //   void ViewStrokes(wxCommandEvent& event);
      void ViewOverlay(wxCommandEvent& event);
      void ViewSimpleComp(wxCommandEvent& event);
      void ViewLabels(wxCommandEvent& event);
      void ViewBlend(wxCommandEvent& event);
      void ViewData(wxCommandEvent& event);
      void ViewInter(wxCommandEvent& event);
      void ViewCustom(wxCommandEvent& event);
      void ViewLabelsAndStrokes(wxCommandEvent& event);
      void ViewLabelsWalpha(wxCommandEvent& event);
      void ViewStrokesOnly(wxCommandEvent& event);
      //void ViewColorDistribution(wxCommandEvent& event);
    
      void ImageObjective(wxCommandEvent& event);
      void ColorCloneObjective(wxCommandEvent& event);
      void MaxLuminanceObjective(wxCommandEvent& event);
      void MinLuminanceObjective(wxCommandEvent& event);
      void MaxLikelyhoodObjective(wxCommandEvent& event);
      void MinLikelyhoodObjective(wxCommandEvent& event);
      void MaxContrastObjective(wxCommandEvent& event);
      void MaxDifferenceObjective(wxCommandEvent& event);
      void TimelapseObjective(wxCommandEvent& event);
      void DifferenceObjective (wxCommandEvent& event);
      void PanoramaObjective (wxCommandEvent& event);

      //	void BrushIntrinsic(wxCommandEvent& event);
      //    void BrushNeg(wxCommandEvent& event);
      void BrushErase(wxCommandEvent& event);
      void ClearAllStrokes(wxCommandEvent& event);

      void ToolTranslate(wxCommandEvent& event);
      void ToolScale(wxCommandEvent& event);
      void ToolRotate(wxCommandEvent& event);
      void ToolPoint(wxCommandEvent& event);

      void handleStroke(wxCommandEvent& event);
      void handleStroke2(wxCommandEvent& event);
      void handleMouseMove(wxCommandEvent& event);
      void handleMouseDown(wxCommandEvent& event);
      void handleRMouseDown(wxCommandEvent& event);
      void handlePopupSelection(wxCommandEvent& event);

      void NewBlend(wxCommandEvent& event);
      void OldBlend(wxCommandEvent& event);

      void stackCurrImageChanged(wxCommandEvent& evt);

      //    void MaxHistFilter(wxCommandEvent& evt);
      //    void MaxDiffFilter(wxCommandEvent& evt);
      //    void MinHistFilter(wxCommandEvent& evt);
      //    void IntrinsicFilter(wxCommandEvent& evt);
      //    void ContrastFilter(wxCommandEvent& evt);
      //    void BrightFilter(wxCommandEvent& evt);
      //    void TimelapseFilter(wxCommandEvent& evt);
      //    void HDRFilter(wxCommandEvent& evt);
      void ComputeObjectiveGlobally(wxCommandEvent& event);
	
      void AddCompToStack(wxCommandEvent& evt);
      void WipeLabels(wxCommandEvent& evt);
      void Recompute(wxCommandEvent& evt);

      void takeSortedCut(wxCommandEvent& evt);
      void cancelSortedCut(wxCommandEvent& evt);

      void SortedInertiaChange(wxCommandEvent& evt);
      //void SortedBrightnessChange(wxCommandEvent& evt);
      void SortedCompChange(wxCommandEvent& evt);

      void ThreadRefreshViewer(wxCommandEvent& evt);
      void finishGlobalCompute(wxCommandEvent& evt);
      void interruptGlobalCompute(wxCommandEvent& evt);

      void BlendingRefreshTimer(wxTimerEvent& evt);
      //--------------

      void BatchProcess (wxCommandEvent& evt);
      void RunMinHist (float potts, float interCost, CutType c, char* fileName);
      void RunMaxDiff (float potts, float interCost, float bright, CutType c, char* fileName);
      //--------------


      void SaveImageName(char* name);
      void SaveLabelingName(char* name);

      void SetViewLabels(const ushort* l);
      void CreateTermVisualizations(PortraitCut* pc);

      void saveCheckpoint();

      Histogram* GetHistograms () { return _histo; }
    
      wxString	  _preLoadedComp;
      wxArrayString _preLoadedStack;
    
      void SetInertiaConstant (float f) { if (_dc) _dc->_inertia = f; }
      void SetCutType (CutType c) { if (_dc) _dc->setCuttype(c); }

    private:
	
      void uncheckAllBrushMenus ();
      void decideStackViewerBrushing();
      void updateColorImage();
      void addStrokeToOverlay(const short* stencil);
      //void addMaskToOverlay(const bool* mask);
      void myUndo();
      void partialStrokeUndo();
      float calcMouseAngle(const int x, const int y);

      void fillPopUpIcon (wxImage* bmp, Vec3i color);
      void renderCustomView ();

      StackViewer* _stackWidget;
      SortedStackViewer* _sortedStackWidget;
      ConstantEditor* _ce;
      ImageAbs _simpleCompImage, _labelImage, _blendedImage, _dataTermImage, _interTermImage;
      ImageAbs *_blendTemp; // G!
      wxMenuBar* _menuBar;
      wxMenu *_menuView, *_menuFile, *_menuObjective, *_menuAlignment, *_menuAction, *_menuEdit;
      wxString _baseName; // name of base image file

      WhichView _whichview;
      bool _viewStrokes;
      int _w, _h;  // composite dimensions, does not change with window size
      Vec3i _eyedropColor;
      Vec2i _dragLoc;

      StrokeType _brushType;
      ToolType _toolType;
    
      Histogram* _histo;
      DialCut* _dc;
      ushort* _labels;
      Adrien::GlobalBlender* _blender;
      //unsigned char* _blendChars;

      //wxStaticBitmap _colorIconStaticBmp;
      wxImage _colorIconImage;
      MyStatusBar* _statusBar;
      PaintCheck* _check; 

      std::deque<Vec3i> _colorOptions;
      //PortraitCut* _thread;
      int _currNum; //number of image becoming brighter or darker
      ImageAbs* _currImg; //new image - brighter/darker version of the one being manipulated
      std::vector<Vec3i> _labelColors;
      int _customStrokeAlpha, _customLabelAlpha;

      wxWindowDisabler* _disabler;
      MyBusyInfo* _busyInfo;

      wxTimer* _timer;

      DECLARE_EVENT_TABLE()
	};

	BEGIN_DECLARE_EVENT_TYPES()
     DECLARE_EVENT_TYPE(REFRESH_COMP, 6669)
     END_DECLARE_EVENT_TYPES()

#endif
