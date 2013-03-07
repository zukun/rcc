#include "compViewer.h"
#include <wx/image.h>
#include "compViewer.h"
#include <wx/image.h>
#include <wx/busyinfo.h>
#include "graphcuts/histogramCut.h"
#include "graphcuts/intrinsicCut.h"
#include "graphcuts/contrastCut.h"
#include "graphcuts/timelapseCut.h" 
//#include "distributionPlot.h"
#include "graphcuts/maxDiffCut.h"
#include "graphcuts/incredibleCut.h"
#include "graphcuts/hdrCut.h"
#include "graphcuts/panoCut.h"
#include "graphcuts/brightCut.h"
#include "graphcuts/imageCut.h"
#include "graphcuts/colorCloneCut.h"
#include "graphcuts/luminanceCut.h"

#include <algorithm>

#define ID_NEW_COMPOSITE  10000
#define ID_LOAD_STACK     10001
#define ID_VIEW_STROKES   10002
#define ID_VIEW_SIMP_COMP 10003
#define ID_VIEW_LABELS    10004
#define ID_VIEW_BLEND     10005
#define ID_UNDO           10006
#define ID_REDO           10007
#define ID_HIGHLIGHT      10008
#define ID_SHADOW         10009
#define ID_COLORCLONE     10010
#define ID_IMAGE          10011
#define ID_ERASE          10012
#define ID_NEW_BLEND      10013
#define ID_OLD_BLEND      10014
#define ID_CEDITOR        10015
#define ID_VIEW_OVERLAY   10016
#define ID_SAVE_IMAGE     10017
#define ID_SAVE_LABELING  10018
#define ID_LOAD_LABELING  10019
#define ID_SAVE_STACK_DESC 10020
#define ID_LOAD_STACK_DESC 10021
#define ID_FILTER_MAXHIST  10022
#define ID_ADDCOMPTOSTACK  10023
#define ID_WIPELABELS      10024
#define ID_BRUSHEDITOR     10025
#define ID_RECOMPUTE       10026
#define ID_INTRINSIC       10027
#define ID_INTRINSIC_BRUSH 10028
#define ID_VIEW_DATA       10029
#define ID_VIEW_INTER      10030
#define ID_VIEW_DISTR      10031
#define ID_BATCH_PROC	   10032
#define ID_FILTER_MINHIST  10033
#define ID_DIFF_BRUSH       10034
#define ID_CONTRAST_FILTER 10035
#define ID_TIMELAPSE_FILTER 10036
#define ID_FILTER_MAXDIFF  10037
#define ID_SAVE_COMP	   10038
#define ID_FILTER_HDR	   10039
#define ID_BRIGHT_FILTER   10040
#define ID_SAVE_MICHAEL    10041
#define ID_SAVE_SOURCES    10042
#define ID_VIEW_CUSTOM     10043
#define ID_VIEW_LABELS_STROKES 10044
#define ID_VIEW_LABELS_ALPHA 10045
#define ID_VIEW_STROKES_ONLY 10046
#define ID_TOOL_TRANSLATE  10047
#define ID_TOOL_SCALE      10048
#define ID_TOOL_ROTATE     10049
#define ID_SAVE_BLEND      10050
#define ID_MAXHIST	   10051
#define ID_MINHIST	   10052
#define ID_MAXCONTRAST	   10053
#define ID_MAXDIFF	   10054
#define ID_TIMELAPSE	   10055
#define ID_LOAD_IMAGES	   10056
#define ID_ERASE_ALL	   10057
#define ID_GLOBAL_OBJECTIVE 10058
#define ID_FAMILY_APP	    10059
#define ID_FACE_APP	    10060
#define ID_BACKGROUND_APP   10061
#define ID_DEPTH_APP	    10062
#define ID_DEFAULT_APP	    10063
#define ID_PANORAMA	    10064
#define ID_TOOL_POINT       10065
#define ID_TIMER            10066
#define ID_POPUP	    11000

BEGIN_EVENT_TABLE(CompViewer, wxFrame)
  EVT_MENU(ID_LOAD_STACK, CompViewer::LoadStack)
  EVT_MENU(ID_NEW_COMPOSITE, CompViewer::NewComposite)
  EVT_MENU(ID_SAVE_IMAGE, CompViewer::SaveImage)
  EVT_MENU(ID_SAVE_COMP, CompViewer::SaveComp)
  EVT_MENU(ID_SAVE_BLEND, CompViewer::SaveBlend)
  EVT_MENU(ID_SAVE_LABELING, CompViewer::SaveLabeling)
  EVT_MENU(ID_LOAD_LABELING, CompViewer::LoadLabeling)
  EVT_MENU(ID_SAVE_MICHAEL, CompViewer::SaveMichael)
  EVT_MENU(ID_SAVE_SOURCES, CompViewer::SaveSources)
  EVT_MENU(ID_NEW_BLEND, CompViewer::NewBlend)
  EVT_MENU(ID_OLD_BLEND, CompViewer::OldBlend)
  EVT_MENU(wxID_EXIT, CompViewer::OnExit)
  EVT_MENU(ID_UNDO, CompViewer::Undo)
  EVT_MENU(ID_REDO, CompViewer::Redo)
  EVT_MENU(ID_CEDITOR, CompViewer::ViewCeditor)
  //  EVT_MENU(ID_VIEW_STROKES, CompViewer::ViewStrokes)
  EVT_MENU(ID_VIEW_OVERLAY, CompViewer::ViewOverlay)
  EVT_MENU(ID_VIEW_SIMP_COMP, CompViewer::ViewSimpleComp)
  EVT_MENU(ID_VIEW_LABELS, CompViewer::ViewLabels)
  EVT_MENU(ID_VIEW_BLEND, CompViewer::ViewBlend)
  EVT_MENU(ID_VIEW_DATA, CompViewer::ViewData)
  EVT_MENU(ID_VIEW_INTER, CompViewer::ViewInter)
  EVT_MENU(ID_VIEW_CUSTOM, CompViewer::ViewCustom)
  EVT_MENU(ID_VIEW_LABELS_STROKES, CompViewer::ViewLabelsAndStrokes)
  EVT_MENU(ID_VIEW_LABELS_ALPHA, CompViewer::ViewLabelsWalpha)
  EVT_MENU(ID_VIEW_STROKES_ONLY, CompViewer::ViewStrokesOnly)
  //EVT_MENU(ID_VIEW_DISTR, CompViewer::ViewColorDistribution)
  EVT_MENU(ID_IMAGE, CompViewer::ImageObjective)
  EVT_MENU(ID_COLORCLONE, CompViewer::ColorCloneObjective)
  EVT_MENU(ID_HIGHLIGHT, CompViewer::MaxLuminanceObjective)
  EVT_MENU(ID_SHADOW, CompViewer::MinLuminanceObjective)
  EVT_MENU(ID_MAXHIST, CompViewer::MaxLikelyhoodObjective)
  EVT_MENU(ID_MINHIST, CompViewer::MinLikelyhoodObjective)
  EVT_MENU(ID_MAXCONTRAST, CompViewer::MaxContrastObjective)
  EVT_MENU(ID_MAXDIFF, CompViewer::MaxDifferenceObjective)
  EVT_MENU(ID_TIMELAPSE, CompViewer::TimelapseObjective)
  EVT_MENU(ID_DIFF_BRUSH, CompViewer::DifferenceObjective)
  EVT_MENU(ID_PANORAMA, CompViewer::PanoramaObjective)
  // EVT_MENU(ID_INTRINSIC_BRUSH, CompViewer::BrushIntrinsic)
  //  EVT_MENU(ID_NEG_BRUSH, CompViewer::BrushNeg)
  EVT_MENU(ID_ERASE, CompViewer::BrushErase)
  EVT_MENU(ID_ERASE_ALL, CompViewer::ClearAllStrokes)
  EVT_MENU(ID_TOOL_TRANSLATE, CompViewer::ToolTranslate)
  EVT_MENU(ID_TOOL_SCALE, CompViewer::ToolScale)
  EVT_MENU(ID_TOOL_ROTATE, CompViewer::ToolRotate)
  EVT_MENU(ID_TOOL_POINT, CompViewer::ToolPoint)
  EVT_CUSTOM(wxEVT_STROKE, -1, CompViewer::handleStroke2)
  EVT_CUSTOM(wxEVT_MYMOUSE_MOVE, -1, CompViewer::handleMouseMove)
  EVT_CUSTOM(wxEVT_MYMOUSE_DOWN, -1, CompViewer::handleMouseDown)
  EVT_CUSTOM(wxEVT_MYRMOUSE_DOWN, -1, CompViewer::handleRMouseDown)
  EVT_SPINCTRL(ID_ZCHANGE, CompViewer::stackCurrImageChanged)
  //  EVT_MENU(ID_FILTER_MAXHIST, CompViewer::MaxHistFilter)
  EVT_MENU(ID_ADDCOMPTOSTACK, CompViewer::AddCompToStack)
  //  EVT_MENU(ID_INTRINSIC, CompViewer::IntrinsicFilter)
  // EVT_MENU(ID_CONTRAST_FILTER, CompViewer::ContrastFilter)
  // EVT_MENU(ID_BRIGHT_FILTER, CompViewer::BrightFilter)
  //  EVT_MENU(ID_TIMELAPSE_FILTER, CompViewer::TimelapseFilter)
  EVT_MENU(ID_WIPELABELS, CompViewer::WipeLabels)
  EVT_MENU(ID_RECOMPUTE, CompViewer::Recompute)
  //  EVT_MENU(ID_BATCH_PROC, CompViewer::BatchProcess)
  //  EVT_MENU(ID_FILTER_MINHIST, CompViewer::MinHistFilter)
  // EVT_MENU(ID_FILTER_MAXDIFF, CompViewer::MaxDiffFilter)
  EVT_MENU_RANGE(ID_POPUP, ID_POPUP+100, CompViewer::handlePopupSelection)

  EVT_BUTTON(ID_TAKE_CUT, CompViewer::takeSortedCut)
  EVT_CUSTOM(SS_CANCEL_STROKE, -1, CompViewer::cancelSortedCut)
  EVT_BUTTON(ID_CANCEL_CUT, CompViewer::cancelSortedCut)
  //  EVT_MENU(ID_FILTER_HDR, CompViewer::HDRFilter)

  EVT_MENU(ID_FAMILY_APP, CompViewer::DefaultFamilySettings)
  EVT_MENU(ID_FACE_APP, CompViewer::DefaultFaceSettings)
  EVT_MENU(ID_BACKGROUND_APP, CompViewer::DefaultBackgSettings)
  EVT_MENU(ID_DEPTH_APP, CompViewer::DefaultEdofSettings)
  EVT_MENU(ID_DEFAULT_APP, CompViewer::DefaultSettings)

  EVT_CUSTOM(SS_INERTIA_CHANGED, -1, CompViewer::SortedInertiaChange)
  EVT_CUSTOM(SS_COMP_CHANGED, -1, CompViewer::SortedCompChange)
  //EVT_CUSTOM(SS_BRIGHTNESS_CHANGED, -1, CompViewer::SortedBrightnessChange)

  EVT_CUSTOM(TH_REFRESH_VIEWER, -1, CompViewer::ThreadRefreshViewer)
  EVT_CUSTOM(END_GLOBAL_COMPUTE, -1, CompViewer::finishGlobalCompute)
  EVT_CUSTOM(INTERRUPT_GLOBAL_COMPUTE, -1, CompViewer::interruptGlobalCompute)
  EVT_MENU(ID_GLOBAL_OBJECTIVE, CompViewer::ComputeObjectiveGlobally)
  EVT_TIMER(ID_TIMER, CompViewer::BlendingRefreshTimer)
  END_EVENT_TABLE()

  wxCondition* finishedCondition;
wxMutex* finishedMutex;
wxMutex* dataMutex;

Vec3f HSVtoRGB( Vec3f hsv )
{
  int i;
  float f, p, q, t;
  if( hsv.y () == 0 ) {
    // achromatic (grey)
    return Vec3f(hsv.z (), hsv.z (), hsv.z ());
  }
  hsv.set_x (hsv.x ()/60);			// sector 0 to 5
  i = (int) floor( hsv.x () );
  f = hsv.x () - i;			// factorial part of h
  p = hsv.z () * ( 1 - hsv.y () );
  q = hsv.z () * ( 1 - hsv.y () * f );
  t = hsv.z () * ( 1 - hsv.y () * ( 1 - f ) );
  switch( i ) {
  case 0:
    return Vec3f(hsv.z (), t, p);
  case 1:
    return Vec3f(q, hsv.z (), p);
  case 2:
    return Vec3f(p, hsv.z (), t);
  case 3:
    return Vec3f(p, q, hsv.z ());
  case 4:
    return Vec3f(t, p, hsv.z ());
  default:		// case 5:
    return Vec3f(hsv.z (), p, q);
  }
}

CompViewer::CompViewer(const wxString& title, const wxPoint& pos, const wxSize& size)
  : BaseGLViewer(NULL, title,pos,size)
{

  _preLoadedComp.Clear ();
  _preLoadedStack.Clear ();
  _stackWidget = NULL;
  _ce = NULL;
  _whichview = W_SIMPCOMP;
  _dc = NULL;
  _histo = NULL;
  _labels = NULL;
  _blender = NULL;
  _sortedStackWidget = NULL;
  _brushType = S_NUM;
  _toolType = T_BRUSH;
  ImageAbs::initBlack();


  _menuBar = new wxMenuBar;
  SetMenuBar( _menuBar );

  _busyInfo = NULL;
  _disabler = NULL;

  _timer = new wxTimer(this, ID_TIMER);
  

  _menuFile = new wxMenu;
  _menuBar->Append( _menuFile, "&File" );  
  _menuFile->Append(ID_NEW_COMPOSITE, "New Composite...");
  _menuFile->Append(ID_LOAD_STACK, "&Load Stack...");
  //  _menuFile->Append(ID_LOAD_IMAGES, "Load Images...");
  _menuFile->Append(ID_LOAD_LABELING,"Load Labeling...");
  _menuFile->Append(ID_SAVE_IMAGE,"Save image...");
  _menuFile->Append(ID_SAVE_COMP, "Save composite...");
  _menuFile->Append(ID_SAVE_BLEND, "Save blended composite...");
  _menuFile->Append(ID_SAVE_LABELING,"Save labeling...");
  _menuFile->AppendSeparator();
  _menuFile->AppendRadioItem(ID_DEFAULT_APP,"Default");
  _menuFile->AppendRadioItem(ID_FAMILY_APP, "Group Portrait");
  _menuFile->AppendRadioItem(ID_FACE_APP, "Face Merging");
  _menuFile->AppendRadioItem(ID_BACKGROUND_APP,"Background Reconstruction");
  _menuFile->AppendRadioItem(ID_DEPTH_APP,"Extended Depth-of-field");
  _menuFile->AppendSeparator();
  _menuFile->Append( wxID_EXIT, "E&xit" );

  _menuEdit = new wxMenu;
  _menuBar->Append( _menuEdit, "&Edit" );  
  _menuEdit->Append(ID_UNDO, "Undo" );
  _menuEdit->AppendSeparator();
  _menuEdit->AppendCheckItem (ID_ERASE, "Brush: Erase Stroke");
  _menuEdit->Append (ID_ERASE_ALL, "Clear All Strokes");

  _menuView = new wxMenu;
  //wxMenu* _menuDialog = new wxMenu;
  _menuBar->Append( _menuView, "&View" );  
  //_menuView->Append( 19249, "Dialogs", _menuDialog );
  //if (!SIMPLE_INTERFACE) {
    _menuView->Append(ID_CEDITOR, "Constants");
    _menuView->AppendSeparator();
    //}
  _menuView->AppendRadioItem( ID_VIEW_SIMP_COMP, "Simple composite\ta");
  _menuView->AppendRadioItem( ID_VIEW_BLEND, "Gradient composite\tf" );
  _menuView->AppendRadioItem(ID_VIEW_LABELS_ALPHA, "Overlay Labeling\ts");
  _menuView->AppendRadioItem(ID_VIEW_STROKES_ONLY, "Overlay Strokes\tx");
  _menuView->AppendRadioItem(ID_VIEW_LABELS_STROKES, "Labeling and Strokes\td");
  if (!SIMPLE_INTERFACE) {
    _menuView->AppendRadioItem(ID_VIEW_CUSTOM, "Custom");
    _menuView->AppendRadioItem(ID_VIEW_DATA, "Data term");
    _menuView->AppendRadioItem(ID_VIEW_INTER, "Interaction term"); 
  }

  _menuAction = new wxMenu;
  _menuBar->Append( _menuAction, "&Actions");
  _menuAction->Append(ID_GLOBAL_OBJECTIVE, "Compute objective globally");
  _menuAction->AppendSeparator();
  _menuAction->Append(ID_WIPELABELS, "Set all labels to current image");
  _menuAction->Append(ID_ADDCOMPTOSTACK, "Add composite to stack");
  _menuAction->Append(ID_RECOMPUTE, "Recompute composite");
  _menuAction->AppendSeparator();
  //_menuAction->Append(ID_OLD_BLEND,"Fast blend");
  _menuAction->Append(ID_NEW_BLEND,"Quality blend");

  _menuObjective = new wxMenu;
  _menuBar->Append(_menuObjective, "&Objective");
  _menuObjective->AppendCheckItem (ID_IMAGE, "Source");
  _menuObjective->AppendCheckItem (ID_COLORCLONE, "Color");
  _menuObjective->AppendCheckItem (ID_HIGHLIGHT, "Max Luminance");
  _menuObjective->AppendCheckItem (ID_SHADOW, "Min Luminance");
  _menuObjective->AppendCheckItem (ID_MAXHIST, "Max Likelihood");
  _menuObjective->AppendCheckItem (ID_MINHIST, "Min Likelihood");
  _menuObjective->AppendCheckItem (ID_MAXCONTRAST, "Max Contrast");
  _menuObjective->AppendCheckItem (ID_MAXDIFF, "Max Difference");
  _menuObjective->AppendCheckItem (ID_TIMELAPSE, "Timelapse");
  _menuObjective->AppendCheckItem (ID_PANORAMA, "Panorama");
  _menuObjective->AppendCheckItem (ID_DIFF_BRUSH, "Eraser");

  _menuAlignment = new wxMenu;
  _menuBar->Append(_menuAlignment, "&Alignment");
  _menuAlignment->AppendCheckItem( ID_VIEW_OVERLAY, "Overlay stack image");
  //  _menuAlignment->Enable(ID_VIEW_OVERLAY, false);
  _menuAlignment->AppendCheckItem(ID_TOOL_TRANSLATE, "Translate");
  _menuAlignment->AppendCheckItem(ID_TOOL_ROTATE, "Rotate");
  _menuAlignment->AppendCheckItem(ID_TOOL_SCALE, "Scale");
  _menuAlignment->AppendCheckItem(ID_TOOL_POINT, "Points");

  uncheckAllBrushMenus ();
  _menuObjective->Check (ID_IMAGE, true);

  ///CreateStatusBar(2);
  _statusBar = new MyStatusBar(this);
  SetStatusBar(_statusBar);
  int widths[2] = { -1, C_COLORICONSIZE+10};
  _statusBar->SetFieldsCount(2);
  _statusBar->SetStatusWidths(2,widths);
  _colorIconImage.Create(C_COLORICONSIZE, C_COLORICONSIZE);
  updateColorImage();
  _statusBar->_colorIconStaticBmp.Create(_statusBar, -1, wxBitmap(_colorIconImage));
  _statusBar->_colorIconStaticBmp.Hide();
  
  finishedMutex = new wxMutex ();
  dataMutex = new wxMutex ();
  finishedCondition = new wxCondition (*finishedMutex);
  _currNum = -1;
  _currImg = NULL;
  srand ((unsigned) time (NULL));

  wxAcceleratorEntry entries[5];
  entries[0].Set(wxACCEL_NORMAL,  (int) 'a',     ID_VIEW_SIMP_COMP);
  entries[1].Set(wxACCEL_NORMAL,  (int) 's',     ID_VIEW_LABELS_ALPHA);
  entries[2].Set(wxACCEL_NORMAL, (int) 'd',     ID_VIEW_LABELS_STROKES);
  entries[3].Set(wxACCEL_NORMAL, (int) 'x',     ID_VIEW_STROKES_ONLY);
  entries[4].Set(wxACCEL_NORMAL, (int) 'f',     ID_VIEW_BLEND);
  wxAcceleratorTable accel(5, entries);
  this->SetAcceleratorTable(accel);

  int i;
  for (i=1; i<=5; ++i)
    _menuBar->EnableTop(i, false);
  _menuFile->Enable(ID_LOAD_LABELING, false);
  _menuFile->Enable(ID_LOAD_STACK, false);
  _menuFile->Enable(ID_SAVE_IMAGE, false);
  _menuFile->Enable(ID_SAVE_LABELING, false);
  _menuFile->Enable(ID_SAVE_BLEND, false);
  _menuFile->Enable(ID_SAVE_COMP, false);
  _menuFile->Enable(ID_DEFAULT_APP, false);
  _menuFile->Enable(ID_FAMILY_APP, false);
  _menuFile->Enable(ID_FACE_APP, false);
  _menuFile->Enable(ID_BACKGROUND_APP, false);
  _menuFile->Enable(ID_DEFAULT_APP, false);
  _menuFile->Enable(ID_DEPTH_APP, false);
}

CompViewer::~CompViewer() {
  if (_labels) delete[] _labels;
  if (_dc) delete _dc;
  //if (_blendChars) delete[] _blendChars;
  if (_blender) delete _blender;
  //  delete labelingMutex;
  delete finishedMutex;
  delete finishedCondition;
  delete dataMutex;
}

void CompViewer::finishInit() {
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(m_canvas,1,wxGROW, 5);
  SetSizer( sizer );   
}


void CompViewer::updateColorImage() {
  int i,j;
  for (j=0; j<C_COLORICONSIZE; ++j)
    for (i=0; i<C_COLORICONSIZE; ++i) {
      _colorIconImage.SetRGB(i,j,_eyedropColor.r(), _eyedropColor.g(), _eyedropColor.b());
    }
}

void CompViewer::fillPopUpIcon (wxImage* bmp, Vec3i color) {

  int i,j;
  for (j=0; j<C_POPUPICONSIZE; ++j)
    for (i=0; i<C_POPUPICONSIZE; ++i) {
      bmp->SetRGB(i,j,color.r(), color.g(), color.b());
    }
}

//---------- EDIT MENU


void CompViewer::Undo(wxCommandEvent* event) {
  myUndo();
}

void CompViewer::partialStrokeUndo() {
  assert(_check->undoable());
  memcpy(_labels, _check->_labels, _w*_h*sizeof(ushort));
  _dc->takeAllStrokeData(_check->_allStrokes);
  _dc->recreateRelevant();
}

void CompViewer::myUndo() {
  if (!_check->undoable()) return;
  _blendedImage = _check->_blend;
  memcpy(_labels, _check->_labels, _w*_h*sizeof(ushort));
  memcpy(m_canvas->getCummOverlay(), _check->_overlay, 4*_w*_h*sizeof(unsigned char));
  _dc->takeAllStrokeData(_check->_allStrokes);
  SetViewLabels(_labels);
  CreateTermVisualizations(_dc);
  _dc->recreateRelevant();
  _simpleCompImage.takeData(_dc->comp());
  m_canvas->Refresh();
}

void CompViewer::Redo(wxCommandEvent* event) {

}

void CompViewer::stackCurrImageChanged(wxCommandEvent& evt) {
  //printf("event happened\n");
  if (_stackWidget && m_canvas->viewingOverlay()) {
    m_canvas->SetOverlayImage(_stackWidget->getCurrImage());
    m_canvas->Refresh();
  }
  if (_stackWidget && _toolType == T_POINT) {
    m_canvas->renderCorrPoints(&(_stackWidget->getCurrCorrVec()), 0);
    _stackWidget->m_canvas->renderCorrPoints(&(_stackWidget->getCurrCorrVec()), 1);
    m_canvas->Refresh();
  }
}

//---------- VIEW MENU

void CompViewer::ViewCeditor(wxCommandEvent& event) {
  if (_dc) {
    if (!_ce)
      _ce = new ConstantEditor(this, _dc);
    _ce->setSelectorRight();
    _ce->Show(true);
  }
}
/*
//deprecated
void CompViewer::ViewStrokes(wxCommandEvent& event) {
_viewStrokes = event.IsChecked();
this->m_canvas->viewCummOverlay(_viewStrokes);
m_canvas->Refresh();
}
*/
void CompViewer::ViewOverlay(wxCommandEvent& event) {
  if (!_stackWidget) {
    _menuView->Check(ID_VIEW_OVERLAY, false);
    return;
  }

  if (event.IsChecked()) {
    m_canvas->SetOverlayImage(_stackWidget->getCurrImage());
    m_canvas->viewOverlay();
  }
  else
    m_canvas->dontViewOverlay();
}

void CompViewer::ViewSimpleComp(wxCommandEvent& event) {
  SetViewImage(&_simpleCompImage);
  //  if(_whichview == W_LABELS_STROKES || _whichview == W_LABELS_ALPHA || _whichview == W_STROKES_ONLY) {
  //_customStrokeAlpha = 0;
  //	_customLabelAlpha = 0;	
  //renderCustomView ();
	
  //}
  this->m_canvas->viewCummOverlay(false);
  _whichview = W_SIMPCOMP;
  m_canvas->Refresh();
}

void CompViewer::ViewLabels(wxCommandEvent& event) {
  SetViewImage(&_labelImage);
  _whichview = W_LABELS;
  m_canvas->Refresh();
}

void CompViewer::ViewBlend(wxCommandEvent& event) {
  SetViewImage(&_blendedImage);
  _whichview = W_BLEND;
  m_canvas->Refresh();
}

void CompViewer::ViewData(wxCommandEvent& event) {
  //printf("view data term");
  SetViewImage(&_dataTermImage);
  _whichview = W_DATA;
  m_canvas->Refresh();
}

void CompViewer::ViewInter(wxCommandEvent& event) {
  SetViewImage(&_interTermImage);
  _whichview = W_INTER;
  m_canvas->Refresh();
}

void CompViewer::ViewCustom(wxCommandEvent& event) {
  SetViewImage(&_simpleCompImage);
  _whichview = W_CUSTOM;
  wxString answer = wxGetTextFromUser ("Set stroke alpha, labeling alpha", "", "0 0"); 
  if (!answer.IsEmpty ()) {
    size_t space = answer.Find(" ");
    wxString xStr = answer.Mid(0, space);
    wxString yStr = answer.Mid(space+1, answer.Len() - space);
    long x,y;
    xStr.ToLong (&x);
    yStr.ToLong (&y);
    _customStrokeAlpha = x;
    _customLabelAlpha = y;
  }
  else {
    _customLabelAlpha = _customStrokeAlpha = 0;
  }

  renderCustomView ();
}

void CompViewer::ViewLabelsAndStrokes(wxCommandEvent& event) {
  this->m_canvas->viewCummOverlay(true);	
  SetViewImage(&_simpleCompImage);
  _whichview = W_LABELS_STROKES;
  _customStrokeAlpha = 255;
  _customLabelAlpha = 128;
  
  renderCustomView ();
}

void CompViewer::ViewLabelsWalpha(wxCommandEvent& event) {
  this->m_canvas->viewCummOverlay(true);	
  SetViewImage(&_simpleCompImage);
  _whichview = W_LABELS_ALPHA;
  _customStrokeAlpha = 128;
  _customLabelAlpha = 128;	
  renderCustomView ();
}

void CompViewer::ViewStrokesOnly(wxCommandEvent& event) {
  this->m_canvas->viewCummOverlay(true);	
  SetViewImage(&_simpleCompImage);
  _whichview = W_STROKES_ONLY;
  _customStrokeAlpha = 255;
  _customLabelAlpha = 0;	
  renderCustomView ();
}

void CompViewer::renderCustomView () {

  int index=0,sIndex=0, i,j;
  unsigned char* strokes = m_canvas->getCummOverlay();
  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index, sIndex+=4) {
      Vec3i color(0,0,0);
      if(!_labelColors.empty())
	color = _labelColors[_labels[index]];
      if (strokes != NULL) {
      	strokes[sIndex+0] = color.r ();
      	strokes[sIndex+1] = color.g ();
      	strokes[sIndex+2] = color.b ();
	const StrokeData& sd = _dc->getStrokeData(i,j);
	if (sd._st != S_NONE)
	  strokes[sIndex+3] = _customStrokeAlpha;
	else
	  strokes[sIndex+3] = _customLabelAlpha;
      }
    }

  this->m_canvas->viewCummOverlay(true);
  m_canvas->Refresh();

}

/*void CompViewer::ViewColorDistribution(wxCommandEvent& event) {
  wxBusyCursor wait;
  if (_histo == NULL) {
  _histo = new Histogram (&(_stackWidget->getImageVector()), _w, _h);
  }
  DistributionPlot* distPlot = new DistributionPlot(this, _stackWidget);	
  distPlot->Show();
  }*/

//---------- OBJECTIVE MENU

void CompViewer::ImageObjective(wxCommandEvent& event) { 

  uncheckAllBrushMenus();
  _menuObjective->Check(ID_IMAGE, true);
  _brushType = S_NUM;
  _toolType = T_BRUSH;


  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, true);

}

void CompViewer::ColorCloneObjective(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_COLORCLONE, true);
  _brushType = S_COLOR;
  _toolType = T_BRUSH;

  Vec3f dialColor;
  dialColor.Set(float(_eyedropColor.r())/255.f,
		float(_eyedropColor.g())/255.f,   
		float(_eyedropColor.b())/255.f);
  _dc->setDialColor(dialColor);
  
  decideStackViewerBrushing();
  //m_canvas->setBrushType(S_COLOR);
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}

void CompViewer::MaxLuminanceObjective(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_HIGHLIGHT, true);

  _brushType = S_HIGHLIGHT;
  _toolType = T_BRUSH;

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}

void CompViewer::MinLuminanceObjective(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_SHADOW, true);
  _brushType = S_SHADOW;
  _toolType = T_BRUSH;

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}

void CompViewer::MaxLikelyhoodObjective(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_MAXHIST, true);
  _brushType = S_MAXHIST;
  _toolType = T_BRUSH;

  if (_histo == NULL) 
    _histo = new Histogram (_dc->getImageData());


  _dc->_allObj._maxhist->setHistogram(_histo);

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}

void CompViewer::MinLikelyhoodObjective(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_MINHIST, true);
  _brushType = S_MINHIST;
  _toolType = T_BRUSH;


  if (_histo == NULL)
    _histo = new Histogram (_dc->getImageData());
  _dc->_allObj._minhist->setHistogram(_histo);

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}

void CompViewer::MaxContrastObjective(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_MAXCONTRAST, true);
  _brushType = S_MAXCONTRAST;
  _toolType = T_BRUSH;

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}


void CompViewer::MaxDifferenceObjective(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_MAXDIFF, true);
  _brushType = S_MAXDIFF;
  _toolType = T_BRUSH;



  wxArrayString choices;
  for (int i=0; i < (int) _stackWidget->getImageVector().size (); i++) {
    char name[100];
    sprintf(name, "%d", i);
    choices.Add (name);
  }
  wxSingleChoiceDialog dial(this, "Select stack image", "",_stackWidget->getImageVector ().size (),&choices.Item(0));
  int res = dial.ShowModal();
  if (res==wxID_CANCEL) return;
  int sel = dial.GetSelection ();
  _dc->_allObj._maxdiff->setSel(sel);

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}

void CompViewer::TimelapseObjective(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_TIMELAPSE, true);
  _brushType = S_TIMELAPSE;
  _toolType = T_BRUSH;
 
  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}

void CompViewer::DifferenceObjective (wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_DIFF_BRUSH, true);
  _brushType = S_DIFF;
  _toolType = T_BRUSH;


  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}

void CompViewer::PanoramaObjective(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_PANORAMA, true);
  _brushType = S_PANO;
  _toolType = T_BRUSH;
 
  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}


// ----------

void CompViewer::BrushErase(wxCommandEvent& event) {
  _brushType = S_ERASE;
  _toolType = T_BRUSH;
  decideStackViewerBrushing();
  //m_canvas->setBrushType(S_ERASE);
  m_canvas->setBrushEraser(true);
  _menuView->Check(ID_VIEW_OVERLAY, false);

}

void CompViewer::ClearAllStrokes(wxCommandEvent& event) {
  _dc->clearAllStrokes ();
  _dc->recreateRelevant();
  SetViewLabels(_labels);
  CreateTermVisualizations(_dc);
  _dc->recreateRelevant();
  _simpleCompImage.takeData(_dc->comp()); 
  m_canvas->Refresh();

}

//----------  Tools

void CompViewer::ToolTranslate(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuAlignment->Check(ID_TOOL_TRANSLATE, true);
  _brushType = S_NONE;
  _toolType = T_TRANSLATE;
  m_canvas->setBrushEraser(false);
  decideStackViewerBrushing();
}

void CompViewer::ToolScale(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuAlignment->Check(ID_TOOL_SCALE, true);
  _brushType = S_NONE;
  _toolType = T_SCALE;
  m_canvas->setBrushEraser(false);
  decideStackViewerBrushing();
}

void CompViewer::ToolRotate(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuAlignment->Check(ID_TOOL_ROTATE, true);
  _brushType = S_NONE;
  _toolType = T_ROTATE;
  m_canvas->setBrushEraser(false);
  decideStackViewerBrushing();
}

void CompViewer::ToolPoint(wxCommandEvent& event) {
  uncheckAllBrushMenus();
  _menuAlignment->Check(ID_TOOL_POINT, true);
  _brushType = S_NONE;
  _toolType = T_POINT;
  m_canvas->setBrushEraser(false);
  decideStackViewerBrushing();
}

//----------
void CompViewer::NewComposite(wxCommandEvent& event) {

  if (_preLoadedComp.IsEmpty ()) {
    wxFileDialog dial(this);
    int res = dial.ShowModal();
    if (res==wxID_CANCEL) return;
    
    _baseName = dial.GetPath();
  }
  else
    _baseName = _preLoadedComp;
  
  wxImage* wxim  = new wxImage(_baseName);
  assert(wxim->Ok());
  wxImageToImageAbs(_simpleCompImage, *wxim); 
  delete wxim;
  _w = _simpleCompImage.getWidth(), _h = _simpleCompImage.getHeight();
  SetClientSize(_w, _h + 2);
  _blendedImage = ImageAbs(_w, _h);
  _blendTemp = new ImageAbs(_w, _h);
  
  if (_whichview == W_SIMPCOMP)
    SetViewImage(&_simpleCompImage);

  _menuFile->Enable(ID_LOAD_STACK, true);
  //_menuFile->Enable(ID_VIEW_OVERLAY, true);

  _labelImage.init(_w,_h);
  _dataTermImage.init(_w, _h);
  _interTermImage.init(_w, _h);

  _labels = new ushort[_w*_h];
  memset(_labels, 0, _w*_h*sizeof(ushort));
  fprintf(stdout, "calling set view labels\n");
  SetViewLabels(_labels);
  fprintf(stdout, "done with set view labels\n");
  //CreateTermVisualizations(_labels, _dc);

  this->m_canvas->setCompDim(_w, _h);
  this->m_canvas->buildCummOverlay();

  _check = new PaintCheck(_w, _h);

  _menuFile->Enable(ID_NEW_COMPOSITE, false);
}

void CompViewer::LoadStack(wxCommandEvent& event) {
  wxArrayString names;
  
  if (_preLoadedStack.GetCount ()== 0) {

    wxFileDialog dial(this, "Choose multiple images", "","","*.*", wxOPEN | wxMULTIPLE, wxDefaultPosition);
    int res = dial.ShowModal();
    if (res==wxID_CANCEL) return;
    dial.GetPaths(names);
  }
  else
    names = _preLoadedStack;
	
  wxBusyCursor wait;

  _stackWidget = new StackViewer(_w, _h, _baseName, this, "Sources", wxPoint(460, 50),
				 wxSize(400, 300), names);
  
  int attribList[4];
  attribList[0] = WX_GL_RGBA;
  attribList[1] = WX_GL_STENCIL_SIZE;
  attribList[2] = 8;
  attribList[3] = WX_GL_DOUBLEBUFFER;
  _stackWidget->m_canvas = new MyGLCanvas(_stackWidget,10, wxDefaultPosition, wxDefaultSize, 0, "shit2", attribList);
  _stackWidget->m_canvas->incInitLevel();
  //_stackWidget->m_canvas = new MyGLCanvas(_stackWidget);
  _stackWidget->finishInit();
  decideStackViewerBrushing();
  _stackWidget->Show(TRUE);

  m_canvas->enableBrushing();
  _stackWidget->m_canvas->setCompDim(_w, _h);
  _dc = new DialCut(_w,_h, &(_stackWidget->getImageVector()), _labels);
  
  _blender = new Adrien::GlobalBlender(_w,_h,&(_stackWidget->getImageVector()), _blendedImage.mutableData(), 1000., stdout);


  _labelColors.push_back(Vec3i(255,0,0));
  _labelColors.push_back(Vec3i(0,255,0));
  _labelColors.push_back(Vec3i(0,0,255));
  _labelColors.push_back(Vec3i(255,255,0));
  _labelColors.push_back(Vec3i(255,0,255));
  _labelColors.push_back(Vec3i(0,255,255));
  _labelColors.push_back(Vec3i(255,128,0));
  _labelColors.push_back(Vec3i(255,0,128));
  _labelColors.push_back(Vec3i(128,255,0));
  _labelColors.push_back(Vec3i(0,255,128));
  _labelColors.push_back(Vec3i(128,0,255));
  _labelColors.push_back(Vec3i(0,128,255));
  _labelColors.push_back(Vec3i(255,255,128));
  _labelColors.push_back(Vec3i(255,128,255));
  _labelColors.push_back(Vec3i(128,255,255));
  _labelColors.push_back(Vec3i(255,128,128));
  _labelColors.push_back(Vec3i(128,255,128));
  _labelColors.push_back(Vec3i(128,128,255));
  _labelColors.push_back(Vec3i(128,64,128));
  _labelColors.push_back(Vec3i(128,128,64));

  int i;
  for (i=20; i < _stackWidget->count (); i++) {
    int h = int((i-20)*(360.f/_stackWidget->count ()-20));
    Vec3f col (HSVtoRGB(Vec3f(0+h, (70)/100., (90)/100.)));
    Vec3i newC(int(col.x () * 255), int(col.y () * 255), int(col.z () * 255));
    _labelColors.push_back(newC);
  }


  _menuFile->Enable(ID_LOAD_LABELING, true);
  _menuFile->Enable(ID_LOAD_STACK, true);
  _menuFile->Enable(ID_SAVE_IMAGE, true);
  _menuFile->Enable(ID_SAVE_LABELING, true);
  _menuFile->Enable(ID_SAVE_BLEND, true);
  _menuFile->Enable(ID_SAVE_COMP, true);
  _menuFile->Enable(ID_DEFAULT_APP, true);
  _menuFile->Enable(ID_FAMILY_APP, true);
  _menuFile->Enable(ID_FACE_APP, true);
  _menuFile->Enable(ID_BACKGROUND_APP, true);
  _menuFile->Enable(ID_DEPTH_APP, true);
  
  for (i=1; i<=5; ++i)
    _menuBar->EnableTop(i, true);

  if (_stackWidget->getImageVector().size() <= 2) {
    _menuObjective->Enable(ID_MAXHIST, false);
    _menuObjective->Enable(ID_MINHIST, false);
  }
}

void CompViewer::SaveComp(wxCommandEvent& event) {
  wxImage im(_w,_h, _simpleCompImage.mutableData(), TRUE);
  wxString name = wxFileSelector("","","","png","*.*",wxSAVE,this);
  if (name.empty()) return;
  bool res = im.SaveFile(name);
  if (!res)
    wxLogWarning("Failed to save image");
}

void CompViewer::SaveBlend(wxCommandEvent& event) {
  wxImage im(_w,_h, _blendedImage.mutableData(), TRUE);
  wxString name = wxFileSelector("","","","png","*.*",wxSAVE,this);
  if (name.empty()) return;
  bool res = im.SaveFile(name);
  if (!res)
    wxLogWarning("Failed to save image");
}

void CompViewer::SaveImage(wxCommandEvent& event) {
  m_canvas->redrawRendered();
  wxImage im(_w,_h);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, _w);
  glReadPixels(0,0,_w,_h,GL_RGB, GL_UNSIGNED_BYTE, im.GetData());
  im = im.Mirror(false);
  wxString name = wxFileSelector("","","","png","*.*",wxSAVE,this);
  if (name.empty()) return;
  bool res = im.SaveFile(name);
  if (!res)
    wxLogWarning("Failed to save image");
}

void CompViewer::SaveImageName(char* name) {
  m_canvas->redrawRendered();
  wxImage im(_w,_h);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, _w);
  glReadPixels(0,0,_w,_h,GL_RGB, GL_UNSIGNED_BYTE, im.GetData());
  im = im.Mirror(false);
  bool res = im.SaveFile(name);
  if (!res)
    wxLogWarning("Failed to save image");
}

void CompViewer::SaveLabeling(wxCommandEvent& event) {
  wxString name = wxFileSelector("","","labels.lab",".lab","*.*",wxSAVE,this);
  if (name.empty()) return;
  if (_dc)
    _dc->saveLabels(name.c_str());
}

void CompViewer::SaveLabelingName(char* name) {

  if (_dc)
    _dc->saveLabels(name);
}

void CompViewer::LoadLabeling(wxCommandEvent& event) {
  wxString name = wxFileSelector("","","labels.lab",".lab","*.*",wxOPEN,this);
  if (name.empty() || !_dc || !_stackWidget) return;
  _dc->loadLabeling(name.c_str());
  _dc->externalLabellingChange();
  SetViewLabels(_labels);
  CreateTermVisualizations(_dc);
  _simpleCompImage.takeData(_dc->comp());
  m_canvas->Refresh();
}

void CompViewer::decideStackViewerBrushing() {

  if (_stackWidget && _toolType == T_BRUSH)
    m_canvas->enableBrushing();
  else
    m_canvas->disableBrushing();

  if (_stackWidget) {
    if (_brushType == S_NUM && _toolType == T_BRUSH)
      _stackWidget->m_canvas->enableBrushing();
    else
      _stackWidget->m_canvas->disableBrushing();
  }

  if (_brushType == S_COLOR && _toolType == T_BRUSH)
    _statusBar->_colorIconStaticBmp.Show();
  else
    _statusBar->_colorIconStaticBmp.Hide();

  if (_toolType == T_POINT && _stackWidget) {
    m_canvas->renderCorrPoints(&(_stackWidget->getCurrCorrVec()), 0);
    _stackWidget->m_canvas->renderCorrPoints(&(_stackWidget->getCurrCorrVec()), 1);
  }
  else {
    m_canvas->dontRenderCorrPoints();
    _stackWidget->m_canvas->dontRenderCorrPoints();
    if (_stackWidget) _stackWidget->disableCreatePoint();
  }

  if ((_brushType == S_ERASE || _brushType == S_DIFF) && _toolType == T_BRUSH) 
    _menuAction->Enable(ID_GLOBAL_OBJECTIVE, false);
  else
    _menuAction->Enable(ID_GLOBAL_OBJECTIVE, true);

  m_canvas->Refresh();
  if (_stackWidget)
    _stackWidget->m_canvas->Refresh();
}


void CompViewer::OnExit(wxCommandEvent& event) {
  if (_stackWidget)
    _stackWidget->Close();
  BaseGLViewer::OnExit(event);
}




void CompViewer::SetViewLabels(const ushort* l) {
  int index=0,sIndex=0, i,j;
  if (!_labelImage.allocated()) return;
  unsigned char* strokes = m_canvas->getCummOverlay();
  unsigned char* data = _labelImage.mutableData();

  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index, sIndex+=4) {
      //(labels.find(l[index]) - labels.begin()) * step;
      Vec3i color(0,0,0);
      if(!_labelColors.empty())
	color = _labelColors[l[index]];

      data[3*index+0] = color.r();//20*l[index];  
      data[3*index+1] = color.g (); //20*l[index];  
      data[3*index+2] =color.b (); //20*l[index];  
      if (strokes != NULL) {
      	strokes[sIndex+0] = color.r ();
      	strokes[sIndex+1] = color.g ();
      	strokes[sIndex+2] = color.b ();
      	//strokes[sIndex+3] = 0;
      }
    }
}

void CompViewer::CreateTermVisualizations(PortraitCut* pc) {
  if (SIMPLE_INTERFACE) return;
  int index=0,i,j;
  if (!_labelImage.allocated()) return;
  unsigned char* datadata = _dataTermImage.mutableData();
  unsigned char* interdata = _interTermImage.mutableData();

  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) {

      int cost = (int) (100. * pc->getCurrDataTerm(i,j)); 
      datadata[3*index+0] = cost;
      datadata[3*index+1] = cost;
      datadata[3*index+2] = cost;

      cost = (int) (100. * pc->getCurrMaxInterTerm(i,j));
      //if (cost != 0) printf("%d\n",cost);
      interdata[3*index+0] = cost;
      interdata[3*index+1] = cost;
      interdata[3*index+2] = cost;

    }
}


float CompViewer::calcMouseAngle(const int x, const int y) {
  Vec2i center(_w/2, _h/2);
  float rad = atan2(float(y-center.y()), float(x-center.x()));
  float deg = rad * 180.f / M_PI;
  return deg;
}


void CompViewer::handleRMouseDown(wxCommandEvent& event) {
	
  int x = m_canvas->mx(), y = m_canvas->my();

  if (m_canvas->rightMDown () && _brushType == S_COLOR) {

    wxMenu popupMenu;
    Vec3i color;
    std::vector<ImageAbs*> images = _stackWidget->getImageVector();
    _colorOptions.clear ();
    for (uint i=0; i < images.size (); i++) {
      color = images[i]->getBlurColor (x,y);
      if (_colorOptions.size () == 0)
	_colorOptions.push_back (color);
      else {
	bool included = false;
	int tempSize = _colorOptions.size ();
	std::deque <Vec3i>::iterator it = _colorOptions.begin ();
	while (it != _colorOptions.end ()) {

	  if (color.distanceTo2 (*it) < 20) 
	    {
	      included = true;
	      break;
	    }
	  float ave_col = color.r () + color.g () + color.b ();
	  ave_col /= 3.f;
	  float ave_it = it->r () + it->g () + it->b ();
	  ave_it /= 3.f;

	  if (ave_col > ave_it) {
	    _colorOptions.insert (it, color);
	    break;
	  }
	  it++;
	}
	if (!included && tempSize == (int)_colorOptions.size ())
	  _colorOptions.push_back (color);
      }
    }

    for (uint i=0; i < _colorOptions.size (); i++) {
      wxMenuItem* item = new wxMenuItem();
      assert (i < 100);
      wxImage bmp;
      bmp.Create(C_POPUPICONSIZE, C_POPUPICONSIZE);
      fillPopUpIcon (&bmp, _colorOptions[i]);
      wxBitmap icon(bmp);
      item->SetBitmap (icon);
      item->SetMenu (&popupMenu);
      item->SetId (ID_POPUP+i);
      item->SetText ("");
      popupMenu.Append (item);
    }
		
    PopupMenu(&popupMenu, x, y);	
  }

}


void CompViewer::handlePopupSelection(wxCommandEvent& event) {
	
  int selection = event.GetId () - ID_POPUP;
  assert (selection >= 0 && selection < (int)_colorOptions.size ());
  
  fprintf (stderr, "selection is %d\n", selection);
  m_canvas->madeSelection ();
  
  _eyedropColor = _colorOptions[selection];      
  m_canvas->setBrushColor(Vec3f(_eyedropColor.r()/255., _eyedropColor.g()/255., _eyedropColor.b()/255.));
  printf("Updating with %d %d %d\n", _eyedropColor.r(), _eyedropColor.g(), _eyedropColor.b());
  updateColorImage();
  _statusBar->_colorIconStaticBmp.SetBitmap(_colorIconImage);
  _statusBar->Refresh();
}


void CompViewer::handleMouseDown(wxCommandEvent& event) {
  
  if(_brushType != S_COLOR)
    m_canvas->setBrushColor(Vec3f(0,1.,1.));
  int x = m_canvas->mx(), y = m_canvas->my();
  if (m_canvas->altDown() && _brushType == S_COLOR) {
    if (_whichview == W_SIMPCOMP) 
      _eyedropColor = _simpleCompImage.getBlurColor(x,y);
    else if (_whichview == W_BLEND)
      _eyedropColor = _blendedImage.getBlurColor(x,y);      
    m_canvas->setBrushColor(Vec3f(_eyedropColor.r()/255., _eyedropColor.g()/255., _eyedropColor.b()/255.));
    printf("Updating with %d %d %d\n", _eyedropColor.r(), _eyedropColor.g(), _eyedropColor.b());
    updateColorImage();
    _statusBar->_colorIconStaticBmp.SetBitmap(_colorIconImage);
    _statusBar->Refresh();
  }

  if (/*m_canvas->ctrlDown()*/_toolType==T_TRANSLATE)
    _dragLoc.Set(x,y);
  else if (_toolType == T_SCALE && _stackWidget) {
    _dragLoc.Set(x,y);
    //_scaleOffset = _stackWidget->getCurrImageScale();
  }
  else if (_toolType ==T_ROTATE) {
    _dragLoc.Set(x,y);
    //_rotOffset = calcMouseAngle(x,y);
  }
  else if (_toolType == T_POINT && _stackWidget) {
    _stackWidget->getCurrCorrVec().push_back(pair<Coord, Coord>(Coord(x,y), Coord(0,0)));
    _stackWidget->enableCreatePoint();
    m_canvas->Refresh();
    printf("added %d %d\n",x,y);
  }

  if (m_canvas->altDown() && _brushType == S_NUM && m_canvas->mouseDown()) {
    if (x>=0 && x<_w && y>=0 && y<_h)
      _stackWidget->externalSetImage(_labels[y*_w+x]);
  }

}

void CompViewer::handleMouseMove(wxCommandEvent& event) {
  wxString caption;
  int x=m_canvas->mx(), 
    y=m_canvas->my();
  caption << "(" << x << ", " << y << "): ";

  if (_dc && x>=0 && x<_w && y>=0 && y<_h) {

    const unsigned char* c;
    if (_whichview == W_BLEND)
      c = _blendedImage.data(x,y);
    else
      c = _simpleCompImage.data(x,y);
    caption << "(" << int(c[0]) << ", " << int(c[1]) << ", " << int(c[2]) << "): ";      
    
    if (_labels)
      caption << _labels[y*_w+x];
    const StrokeData& sd = _dc->getStrokeData(x,y);
    if (sd._st != S_NONE) {
      //assert(m_canvas->mouseDown() || _sortedStackWidget || 
      //     m_canvas->getCummOverlay()[4*(y*_w+x)+3]==255); // consistency between overlay & labels
      caption << "    Stroke: ";
      if (sd._st == S_HIGHLIGHT)
	caption << "Highlight";
      else if (sd._st == S_SHADOW)
	caption << "Shadow";
      else if (sd._st == S_COLOR) {
	caption << "(" <<  int(sd._dialColor.r()*255.f) <<  ", " << 
	  int(sd._dialColor.g()*255.f) <<   ", " << 
	  int(sd._dialColor.b()*255.f) << ")";
      }
      else if (sd._st == S_NUM)
	caption << "image " << sd._num;
    }
  }

  if (/*m_canvas->ctrlDown()*/_toolType==T_TRANSLATE && _stackWidget && m_canvas->mouseDown()) {
    _stackWidget->translateCurrImage(x-_dragLoc.x(), y-_dragLoc.y());
    _dragLoc.Set(x,y);
    _simpleCompImage.takeData(_dc->comp());
    m_canvas->Refresh();
    _stackWidget->m_canvas->Refresh();
  }
  else if (_toolType==T_SCALE && _stackWidget && m_canvas->mouseDown()) {
    Vec2i newLoc(x,y);
    Vec2i ou(newLoc, _dragLoc);
    Vec2i ul(_w, _h);
    float w = ul.Len();
    //float scale = (w + float(ou.Dot2(ul)) / w) / w;
    float scale = float(ou.Dot2(ul)) / (w*w);
    _stackWidget->scaleCurrImage(1.+scale);
    //_stackWidget->setCurrImageScale(_scaleOffset.x() + scale, _scaleOffset.y() + scale);
    _dragLoc = newLoc;
    _simpleCompImage.takeData(_dc->comp());
    m_canvas->Refresh();
    _stackWidget->m_canvas->Refresh();
  }
  else if (_toolType==T_ROTATE && _stackWidget && m_canvas->mouseDown()) {
    float newAngle = calcMouseAngle(x,y);
    float oldAngle = calcMouseAngle(_dragLoc.x(), _dragLoc.y());
    newAngle -= oldAngle;
    _stackWidget->rotateCurrImage(newAngle);
    _dragLoc.Set(x,y);
    _simpleCompImage.takeData(_dc->comp());
    m_canvas->Refresh();
    _stackWidget->m_canvas->Refresh();
  }

  SetStatusText(caption);
}

void CompViewer::handleStroke2(wxCommandEvent& event) {


  if (_dc->multiRegion() || _dc->hasFilter() || _brushType==S_NUM) {
    handleStroke(event);
    return;
  }

  Brush* brush = NULL;
  if (_stackWidget->m_canvas->getCurrBrush())
    brush = _stackWidget->m_canvas->getCurrBrush();
  if (m_canvas->getCurrBrush()) {
    assert(brush == NULL);              // should only be one brush
    brush = m_canvas->getCurrBrush();
  }
  if (_sortedStackWidget && _sortedStackWidget->m_canvas->getCurrBrush()) {
    assert(brush == NULL);
    brush = _sortedStackWidget->m_canvas->getCurrBrush();
  }
  assert(brush != NULL);

  if (!_sortedStackWidget || brush != _sortedStackWidget->m_canvas->getCurrBrush())
    saveCheckpoint();  // no undo update if sorted brushing

  //// REFINEMENT STROKE IN SORTEDSTACKVIEWER
  else {
    partialStrokeUndo(); // undo labeling, _allStrokes effect of this stroke
    assert(_sortedStackWidget);
    brush->orStencil(_sortedStackWidget->getBrushStencil());

    int dialNum = _sortedStackWidget->getCurrNum();
    _dc->setDialNum(dialNum);
    const short* stencil = brush->getStencil();
    std::vector<CutResult> cr;
    float tmp = _dc->_inertia;
    _dc->_inertia = _sortedStackWidget->getInertiaValue();
    _dc->takeStroke2(stencil, S_NUM, cr);
    _dc->_inertia = tmp;
    
    _sortedStackWidget->copyBrushStencil(stencil);
    _sortedStackWidget->takeRefinementStroke(cr);

    int i = _sortedStackWidget->getCurrImage();
    const short* stencil2 = _sortedStackWidget->getBrushStencil();
    _dc->takeMaskAndStencil(_sortedStackWidget->getMask(i), stencil2, _sortedStackWidget->getCurrNum());
    _simpleCompImage.takeData(_dc->comp());

    _sortedStackWidget->m_canvas->Refresh();
    m_canvas->Refresh ();
    return;
  }
  ////---------

  // get stencil
  const short* stencil = brush->getStencil();
  //int index,i,j;
  // inform dc as necessary
  if (_brushType==S_COLOR) {
    Vec3f dialColor;
    dialColor.Set(float(_eyedropColor.r())/255.f,
		  float(_eyedropColor.g())/255.f,   
		  float(_eyedropColor.b())/255.f);
    _dc->setDialColor(dialColor);
  }
  else if (_brushType==S_NUM) 
    _dc->setDialNum(_stackWidget->getCurrNum());


  std::vector<CutResult> cr;
  _dc->takeStroke2(stencil, _brushType, cr);

  if (_brushType == S_ERASE) {
    assert(cr.empty());
    addStrokeToOverlay(stencil);
    return;
  }

  if (cr.empty()) {
    myUndo();
    return;
  }

  
  // throw up sortedstackviewer
  
  if (_sortedStackWidget) {
    _sortedStackWidget->Destroy ();
    _sortedStackWidget = NULL;
  }

  _sortedStackWidget = new SortedStackViewer(_stackWidget, stencil, this, "Selections", _stackWidget->GetPosition () + wxPoint(0,50)/*wxPoint(460, 50)*/,
					     wxSize(600, 300));

  int attribList[4];
  attribList[0] = WX_GL_RGBA;
  attribList[1] = WX_GL_STENCIL_SIZE;
  attribList[2] = 8;
  attribList[3] = WX_GL_DOUBLEBUFFER;
  _sortedStackWidget->m_canvas = new MyGLCanvas(_sortedStackWidget,10, wxDefaultPosition, 
						wxDefaultSize, 0, "shit3", attribList);

  _sortedStackWidget->m_canvas->incInitLevel();

  _sortedStackWidget->copyBrushStencil(stencil);
  _sortedStackWidget->finishInit(cr, _dc->_inertia);
  _sortedStackWidget->m_canvas->enableBrushing();  
  _sortedStackWidget->Show(TRUE);

  // we're adding this so we can see effect in composite view, can be undone if cancelled
  int i = _sortedStackWidget->getCurrImage();
  const short* stencil2 = _sortedStackWidget->getBrushStencil();
  _dc->takeMaskAndStencil(_sortedStackWidget->getMask(i), stencil2, _sortedStackWidget->getCurrNum());
  _simpleCompImage.takeData(_dc->comp());
  m_canvas->Refresh();
}

void CompViewer::takeSortedCut(wxCommandEvent& evt) {
  int i = _sortedStackWidget->getCurrImage();
  printf("take sorted cut %d %d\n", i, _sortedStackWidget->getCurrNum());
  const short* stencil = _sortedStackWidget->getBrushStencil(); 
  //_dc->takeMaskAndStencil(_sortedStackWidget->getMask(i), stencil, _sortedStackWidget->getCurrNum());
  //_dc->takeMaskAsNumBrush(_sortedStackWidget->getMask(i), _sortedStackWidget->getCurrNum());
  //addMaskToOverlay(_sortedStackWidget->getMask(i));
  addStrokeToOverlay(stencil);

  //this was used for when we were creating brighter and darker images while
  //in the sorted stack viewer. Since we're not doing that at the moment, let's
  //ignore it...but we may need to add it later
  //  if (_sortedStackWidget->getCurrNum () != (_stackWidget->getImageVector().size () - 1)) {
  //	_stackWidget->removeLastImage ();
  //	_dc->removeLastImage ();
  //	delete _currImg;
  //}
  _currNum = -1;
  _currImg = NULL;

  _sortedStackWidget->Destroy();
  _sortedStackWidget = NULL;

  SetViewLabels(_labels);
  CreateTermVisualizations(_dc);
  _simpleCompImage.takeData(_dc->comp());
  m_canvas->Refresh();
}

void CompViewer::cancelSortedCut(wxCommandEvent& evt) {
  printf("cancel sorted stroke");
  myUndo();
  _sortedStackWidget->Destroy ();
  _sortedStackWidget = NULL;

  if (_currNum > -1 ) {
    _stackWidget->removeLastImage ();
    _dc->removeLastImage ();
    delete _currImg;
    _currImg = NULL;
    _currNum = -1;
  }
}


void CompViewer::addStrokeToOverlay(const short* stencil) {
  int index,i,j;

  // add stroke to overlay so we can see stroke later
  //unsigned char alpha = 255;
  //CAN NO LONGER VIEW STROKES w/ menu option
  unsigned char alpha = 0;
  if (_brushType == S_ERASE)
    alpha = 0;

  // turn alpha values in _cummOverlay to 1 so they can be seen
  index=0;
  unsigned char* cummOverlay = this->m_canvas->getCummOverlay();
  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) {
      if (stencil[index]==1)
	cummOverlay[4*index+3] = alpha;
    }
}

void CompViewer::SortedInertiaChange(wxCommandEvent& evt) {
  printf("CompViewer::SortedInertiaChange\n");
  assert(_sortedStackWidget);
  partialStrokeUndo(); // undo labeling, _allStrokes effect of this stroke
  const short* stencil = _sortedStackWidget->getBrushStencil();
  float tmp = _dc->_inertia;
  _dc->_inertia = _sortedStackWidget->getInertiaValue();
  int dialNum = _sortedStackWidget->getCurrNum();
  _dc->setDialNum(dialNum);
  std::vector<CutResult> cr;
  _dc->takeStroke2(stencil, S_NUM, cr);
  _dc->_inertia = tmp;
  _sortedStackWidget->takeRefinementStroke(cr);

  int i = _sortedStackWidget->getCurrImage();
  const short* stencil2 = _sortedStackWidget->getBrushStencil();
  _dc->takeMaskAndStencil(_sortedStackWidget->getMask(i), stencil2, _sortedStackWidget->getCurrNum());
  _simpleCompImage.takeData(_dc->comp());

  _sortedStackWidget->m_canvas->Refresh();
  m_canvas->Refresh();
}

// actually, Z_CHANGE only
void CompViewer::SortedCompChange(wxCommandEvent& evt) {
  partialStrokeUndo(); // undo labeling, _allStrokes effect of this stroke
  int i = _sortedStackWidget->getCurrImage();
  const short* stencil2 = _sortedStackWidget->getBrushStencil();
  _dc->takeMaskAndStencil(_sortedStackWidget->getMask(i), stencil2, _sortedStackWidget->getCurrNum());
  _simpleCompImage.takeData(_dc->comp());
  m_canvas->Refresh();
}

/*
  void CompViewer::SortedBrightnessChange(wxCommandEvent& evt) {
  printf("CompViewer::SortedBrightnessChange\n");
  assert(_sortedStackWidget);
  partialStrokeUndo(); // undo labeling, _allStrokes effect of this stroke
  int dialNum = _sortedStackWidget->getCurrNum();
  std::vector<ImageAbs*> images = _stackWidget->getImageVector ();
  ImageAbs* newOne;
  if (dialNum == _currNum) {
  _currImg->setBrightness (_sortedStackWidget->getBrightnessValue ());
  }
  else {
  if (_currNum > -1) {
  _stackWidget->removeLastImage ();
  _dc->removeLastImage ();
  delete _currImg;
  _sortedStackWidget->revert(_currNum);
  }
  newOne = new ImageAbs (*(images [dialNum]));
  newOne->setBrightness (_sortedStackWidget->getBrightnessValue ());
  _stackWidget->addImageToStack (newOne);
  _dc->addImage (newOne);
  _currNum = _stackWidget->count() -1;
  _currImg = newOne;
  _sortedStackWidget->setCurrNum (_currNum, _currImg);
  }
  
  const short* stencil = _sortedStackWidget->getBrushStencil();
  dialNum = _sortedStackWidget->getCurrNum();
  _dc->setDialNum(dialNum);
  std::vector<CutResult> cr;
  _dc->takeStroke2(stencil, S_NUM, cr);
  _sortedStackWidget->takeRefinementStroke(cr);

  int i = _sortedStackWidget->getCurrImage();
  const short* stencil2 = _sortedStackWidget->getBrushStencil();
  _dc->takeMaskAndStencil(_sortedStackWidget->getMask(i), stencil2, _sortedStackWidget->getCurrNum());
  _simpleCompImage.takeData(_dc->comp());

  _sortedStackWidget->m_canvas->Refresh();
  m_canvas->Refresh();
  }
*/
void CompViewer::ThreadRefreshViewer(wxCommandEvent& evt) {

  SetViewLabels(_labels);    
  _simpleCompImage.takeData(_dc->comp());
  m_canvas->Refresh();

  /*
  if (dataMutex->TryLock () == wxMUTEX_NO_ERROR) {
    //_dc->externalLabellingChange();
    SetViewLabels(_labels);    
    CreateTermVisualizations(_thread);
    _simpleCompImage.takeData(_thread->comp());
    dataMutex->Unlock ();
  }
  m_canvas->Refresh();
  */
}

void CompViewer::finishGlobalCompute(wxCommandEvent& evt) {
  if (!_busyInfo) return;
  _busyInfo->Destroy(); // not calling close handler
  _busyInfo = NULL;
  assert(_disabler);
  delete _disabler; _disabler = NULL;
}

void CompViewer::interruptGlobalCompute(wxCommandEvent& evt) {
  //if (_dc->IsRunning())
  _dc->myStop(); // shouldn't delete object, since joinable
  //if (_blender->IsRunning())
  _blender->myStop(); 
  finishGlobalCompute(evt);
}

/*
// NOT USED
void CompViewer::addMaskToOverlay(const bool* mask) {
  int index,i,j;

  // add stroke to overlay so we can see stroke later
  unsigned char alpha = 1;
  index=0;
  unsigned char* cummOverlay = this->m_canvas->getCummOverlay();
  for (j=0; j<_h; ++j)
    for (i=0; i<_w; ++i, ++index) {
      if (mask[index])
	cummOverlay[4*index+3] = alpha;
    }
}
*/

void CompViewer::handleStroke(wxCommandEvent& event) {
  wxWindowDisabler disableAll; 
  wxBusyCursor wait;
  //wxBusyInfo info("Working, please wait...");

  printf("\n\n handle stroke event\n");
  saveCheckpoint();

  Brush* brush = NULL;
  if (_stackWidget->m_canvas->getCurrBrush())
    brush = _stackWidget->m_canvas->getCurrBrush();
  if (m_canvas->getCurrBrush()) {
    assert(brush == NULL);              // should only be one brush
    brush = m_canvas->getCurrBrush();
  }
  assert(brush != NULL);

  const short* stencil = brush->getStencil();

  addStrokeToOverlay(stencil);


  // handle it
  if (_brushType==S_COLOR) {
    Vec3f dialColor;
    dialColor.Set(float(_eyedropColor.r())/255.f,
		  float(_eyedropColor.g())/255.f,   
		  float(_eyedropColor.b())/255.f);
    _dc->setDialColor(dialColor);
  }
  else if (_brushType==S_NUM) 
    _dc->setDialNum(_stackWidget->getCurrNum());

  _dc->takeStroke(stencil, _brushType);

  SetViewLabels(_labels);
  CreateTermVisualizations(_dc);
  _simpleCompImage.takeData(_dc->comp());
  m_canvas->Refresh();
}


void CompViewer::saveCheckpoint() {
  _check->save(_labels, m_canvas->getCummOverlay(),  _blendedImage, _dc->getAllStrokes()); 
}


#define TIMER_LENGTH 500
void CompViewer::NewBlend(wxCommandEvent& event) {
  //wxWindowDisabler disableAll; 
  //wxBusyCursor wait;
  //wxBusyInfo info("Working, please wait...");
  double et;
  if (_ce)
    et = _ce->_edgeThres;
  else
    et = 100;

  if (_blender && _labels) {
    //popup dialog asking for coordinates of pixel to pin.
    wxString answer = wxGetTextFromUser ("Which pixel would you like to pin?", "", "0 0"); 
    if (!answer.IsEmpty ()) {
      size_t space = answer.Find(" ");
      wxString xStr = answer.Mid(0, space);
      wxString yStr = answer.Mid(space+1, answer.Len() - space);
      double x,y;
      xStr.ToDouble (&x);
      yStr.ToDouble (&y);

	  _blender->pinPixel ((int)x, (int)y);


      _disabler = new wxWindowDisabler();
      _busyInfo = new MyBusyInfo(this);
      _blender->takeComputeData(false, _labels, et);
      _busyInfo->Show();
      
      SetViewImage(&_blendedImage);
      _whichview = W_BLEND;
      _menuView->Check(ID_VIEW_BLEND, true);
      
      _timer->Start(TIMER_LENGTH);
      _blender->compute();
      finishGlobalCompute(event);
      //      _blender->compute(false, _labels, et);
      //_blender->updateFinal();

      //_blender->copyOutFinal(_blendedImage.mutableData());



      /*_blender->Create();
      _blender->Run();

      _disabler = new wxWindowDisabler();
      _busyInfo = new MyBusyInfo(this);
      _busyInfo->Show();*/


      //if (_whichview == W_BLEND)


      //m_canvas->Refresh();
    }
  }
}

void CompViewer::BlendingRefreshTimer(wxTimerEvent& evt) {
  //printf("rendering timer\n");
  wxStartTimer();
  //if (!_blender->IsRunning())
  _timer->Stop();
  //wxMutexLocker(_mutex); // done internal to _blender
  _blender->updateFinal();
  //_blender->copyOutFinal(_blendedImage.mutableData());
  //SetViewImage(&_blendedImage);
  m_canvas->Refresh();
  printf("rendering timer done %d\n", wxGetElapsedTime());
  if (!_blender->doneComputing())
  _timer->Start(TIMER_LENGTH);
}


void CompViewer::OldBlend(wxCommandEvent& event) {
  wxWindowDisabler disableAll; 
  wxBusyCursor wait;
  //wxBusyInfo info("Working, please wait...");
  double et;
  if (_ce)
    et = _ce->_edgeThres;
  else
    et = 1000;

  if (_labels && _stackWidget) {
    Adrien::blend(_w,_h,_labels,&(_stackWidget->getImageVector()),_blendedImage.mutableData(),et,stdout); 
    //    if (_whichview == W_BLEND)
    SetViewImage(&_blendedImage);
    m_canvas->Refresh();
    _menuView->Check(ID_VIEW_BLEND, true);
  }
}

/*
  void CompViewer::MaxHistFilter(wxCommandEvent& evt) {
  wxBusyCursor wait;
  if (_histo == NULL) 
  _histo = new Histogram (&(_stackWidget->getImageVector()), _w, _h);
  
  HoleFillCut* hfc = new HoleFillCut (_w, _h, &(_stackWidget->getImageVector()), _labels, stdout, _histo);
  hfc->_potts = _dc->_potts;
  hfc->_interCost = _dc->_regular_inter;
  hfc->_ft = HoleFillCut::Max;
  hfc->setCuttype (_dc->getCuttype ());
  hfc->allocateAllAuxData();
  
  if (SINGLE_THREAD) {
  hfc->compute ();
  _dc->externalLabellingChange();
  SetViewLabels(_labels);
  CreateTermVisualizations(hfc);
  _simpleCompImage.takeData(hfc->comp());
  delete hfc;
  _dc->setFilter(hfc);
  }
  else {
  _thread = hfc;
  _thread->_th_frame = this;

  _thread->Create ();
  _thread->Run ();

  }
  m_canvas->Refresh();
  //fflush (fp);
  //fclose (fp);
  }

  void CompViewer::MinHistFilter(wxCommandEvent& evt) {
  wxBusyCursor wait;
  if (_histo == NULL) {
  _histo = new Histogram (&(_stackWidget->getImageVector()), _w, _h);

  }
  HoleFillCut* hfc = new HoleFillCut (_w, _h, &(_stackWidget->getImageVector()), _labels, stdout, _histo);
  hfc->_potts = _dc->_potts;
  hfc->_interCost = _dc->_regular_inter;
  hfc->_ft = HoleFillCut::Min;
  hfc->setCuttype (_dc->getCuttype ());
  hfc->allocateAllAuxData();
  if (SINGLE_THREAD) {
  hfc->compute ();
  _dc->externalLabellingChange();
  SetViewLabels(_labels);
  CreateTermVisualizations(hfc);
  _simpleCompImage.takeData(hfc->comp());
  delete hfc;
  }
  else {
  _thread = hfc;
  _thread->_th_frame = this;

  _thread->Create ();
  _thread->Run ();

  }
  m_canvas->Refresh();
  //fflush (fp);
  //fclose (fp);
  }

  void CompViewer::MaxDiffFilter(wxCommandEvent& evt) {
  wxArrayString choices;
  for (int i=0; i < (int) _stackWidget->getImageVector().size (); i++) {
  char name[100];
  sprintf(name, "%d", i);
  choices.Add (name);
  }
  wxSingleChoiceDialog dial(this, "Select stack image", "",_stackWidget->getImageVector ().size (),&choices.Item(0));
  int res = dial.ShowModal();
  if (res==wxID_CANCEL) return;
  int sel = dial.GetSelection ();

  MaxDiffCut* mdc = new MaxDiffCut(_w,_h, &(_stackWidget->getImageVector()), _labels, sel);
  mdc->_potts = _dc->_potts;
  mdc->_interCost = _dc->_regular_inter;
  mdc->_bright = _dc->_bright;
  mdc->setCuttype (_dc->getCuttype ());
  mdc->allocateAllAuxData();

  if (SINGLE_THREAD) {
  wxBusyCursor wait;
  mdc->compute();
  _dc->externalLabellingChange();
  SetViewLabels(_labels);
  CreateTermVisualizations(mdc);
  _simpleCompImage.takeData(mdc->comp());
  _dc->setFilter(mdc);
  delete mdc;
  }
  else {
  _thread = mdc;
  _thread->_th_frame = this;

  _thread->Create ();
  _thread->Run ();
  }
	
  m_canvas->Refresh();

  }
  void CompViewer::IntrinsicFilter(wxCommandEvent& evt) {
  wxBusyCursor wait;
  PanoCut* ic = new PanoCut(_w,_h, &(_stackWidget->getImageVector()), _labels); 

  if (SINGLE_THREAD) {
  ic->compute();
  _dc->externalLabellingChange();
  SetViewLabels(_labels);
  CreateTermVisualizations(ic);
  _simpleCompImage.takeData(ic->comp());
  _dc->setFilter(ic);
  //delete ic;
  }
  else {
  _thread = ic;
  _thread->_th_frame = this;

  _thread->Create ();
  _thread->Run ();
  }

  m_canvas->Refresh();
  }

  void CompViewer::ContrastFilter(wxCommandEvent& evt) {
  printf("contrast filter\n");
  wxBusyCursor wait;
  ContrastCut* ic = new ContrastCut(_w,_h, &(_stackWidget->getImageVector()), _labels);
  ic->_potts = _dc->_potts;
  ic->_regular_inter = _dc->_regular_inter;
  ic->setCuttype(_dc->getCuttype());
  ic->allocateAllAuxData();

  if (SINGLE_THREAD) {
  ic->compute();
  _dc->externalLabellingChange();
  SetViewLabels(_labels);
  CreateTermVisualizations(ic);
  _simpleCompImage.takeData(ic->comp());
  _dc->setFilter(ic);
  //delete ic;
  }
  else {
  _thread = ic;
  _thread->_th_frame = this;

  _thread->Create ();
  _thread->Run ();
  }

  m_canvas->Refresh();
  }

  void CompViewer::BrightFilter(wxCommandEvent& evt) {
  wxBusyCursor wait;
  BrightCut* ic = new BrightCut(_w,_h, &(_stackWidget->getImageVector()), _labels);
  ic->_potts = _dc->_potts;
  ic->_regular_inter = _dc->_regular_inter;
  ic->setCuttype(_dc->getCuttype());
  ic->allocateAllAuxData();

  if (SINGLE_THREAD) {
  ic->compute();
  _dc->externalLabellingChange();
  SetViewLabels(_labels);
  CreateTermVisualizations(ic);
  _simpleCompImage.takeData(ic->comp());
  _dc->setFilter(ic);
  //delete ic;
  }
  else {
  _thread = ic;
  _thread->_th_frame = this;

  _thread->Create ();
  _thread->Run ();

  }
  m_canvas->Refresh();
  }

  void CompViewer::TimelapseFilter(wxCommandEvent& evt) {
  printf("timelapse filter\n");
  wxBusyCursor wait;
  TimelapseCut* ic = new TimelapseCut(_w,_h, &(_stackWidget->getImageVector()), _labels);
  ic->_potts = _dc->_potts;
  ic->_regular_inter = _dc->_regular_inter;
  ic->setCuttype(_dc->getCuttype());
  ic->allocateAllAuxData();

  if (SINGLE_THREAD) {
  ic->compute();
  _dc->externalLabellingChange();
  SetViewLabels(_labels);
  CreateTermVisualizations(ic);
  _simpleCompImage.takeData(ic->comp());
  _dc->setFilter(ic);
  //delete ic;
  }
  else {
  _thread = ic;
  _thread->_th_frame = this;

  _thread->Create ();
  _thread->Run ();
  }

  m_canvas->Refresh();
  }

  void CompViewer::HDRFilter(wxCommandEvent& evt) {
  wxBusyCursor wait;
  if (_histo == NULL) {
  _histo = new Histogram (&(_stackWidget->getImageVector()), _w, _h);
  }
  HDRCut* hc = new HDRCut(_w,_h, &(_stackWidget->getImageVector()), _labels, stdout, _histo);
  hc->_potts = _dc->_potts;
  hc->_interCost = _dc->_regular_inter;
  //  hc->SetCutType (_dc->getCuttype ());

  if (SINGLE_THREAD) {
  hc->compute();
  _dc->externalLabellingChange();
  SetViewLabels(_labels);
  CreateTermVisualizations(hc);
  _simpleCompImage.takeData(hc->comp());
  //_dc->setFilter(ic);
  delete hc;
  }
  else {
  _thread = hc;
  _thread->_th_frame = this;

  _thread->Create ();
  _thread->Run ();

  }
  m_canvas->Refresh();
  }
*/
void CompViewer::AddCompToStack(wxCommandEvent& evt) {
  if (!_stackWidget) return;
  ImageAbs* copy = new ImageAbs(_simpleCompImage);
  _stackWidget->addImageToStack(copy);
  _dc->addImage(copy);
}

void CompViewer::WipeLabels(wxCommandEvent& evt) {
  if (!_dc) return;
  _dc->wipeLabels(_stackWidget->getCurrNum());
 
  SetViewLabels(_labels);
  CreateTermVisualizations(_dc);
  _simpleCompImage.takeData(_dc->comp()); 
  m_canvas->Refresh();
}

void CompViewer::Recompute(wxCommandEvent& evt) {
  if (!_stackWidget || !_dc) return;
  _dc->computeMulti();
  _dc->recreateRelevant();
  SetViewLabels(_labels);
  CreateTermVisualizations(_dc);
  _dc->recreateRelevant();
  _simpleCompImage.takeData(_dc->comp()); 
  m_canvas->Refresh();
}

void CompViewer::ComputeObjectiveGlobally(wxCommandEvent& event) {

  assert(_brushType != S_COLOR && _brushType != S_NUM && validStrokeType(_brushType) && _dc);
  _dc->setGlobalObjType(_brushType);
  _dc->setGlobalCompute(true);

  if (_brushType == S_MAXHIST || _brushType == S_MINHIST) {
    if (_histo == NULL) 
      _histo = new Histogram (_dc->getImageData());
    _dc->_allObj._minhist->setHistogram(_histo);
    _dc->_allObj._maxhist->setHistogram(_histo);

  }

  _dc->allocateAllAuxData();
	
  if (SINGLE_THREAD) {
    //bwxBusyCursor wait;
    _disabler = new wxWindowDisabler();
    _busyInfo = new MyBusyInfo(this);
    _busyInfo->Show();
    _dc->_th_frame = this;
    _dc->compute();
    _dc->externalLabellingChange();
    SetViewLabels(_labels);
    CreateTermVisualizations(_dc);
    _simpleCompImage.takeData(_dc->comp());
    finishGlobalCompute(event);
  }
  /*
  else {
    //_thread = _dc;
    _dc->_th_frame = this;

    _disabler = new wxWindowDisabler();
    _busyInfo = new MyBusyInfo(this);
    _busyInfo->Show();
   _dc->Create();
    _dc->Run();
  }
  */
  m_canvas->Refresh();

}

/*
  void CompViewer::BatchProcess(wxCommandEvent& evt) {

  if (_histo == NULL) 
  _histo = new Histogram (&(_stackWidget->getImageVector()), _w, _h);

  //_potts, _interCost, _cutType, outputFile

  RunMaxDiff(0., 3., 0.5, C_NORMAL, "maxDiff/new/p0_i3_cnormal");
  RunMaxDiff(0., 4., 0.5, C_NORMAL, "maxDiff/new/p0_i4_cnormal");
  RunMaxDiff(0., 10., 0.5, C_NORMAL, "maxDiff/new/p0_i10_cnormal");

  }

  void CompViewer::RunMinHist (float potts, float interCost, CutType c, char* fileName)  {

  HoleFillCut* hfc = new HoleFillCut (_w, _h, &(_stackWidget->getImageVector()), _labels, stdout, _histo);
  hfc->_potts = potts;
  hfc->_interCost = interCost;
  hfc->_ft = HoleFillCut::Min;
  //hfc->SetCutType (c);
  hfc->setCuttype (c);
  hfc->allocateAllAuxData();
  hfc->compute ();
  SetViewLabels(_labels);
  CreateTermVisualizations(hfc);
  _simpleCompImage.takeData(hfc->comp());
  m_canvas->Refresh();
  char imageFile[300];
  sprintf(imageFile, "%s.png", fileName);
  char labelingFile[300];
  sprintf(labelingFile, "%s.lab", fileName);
  SaveImageName(imageFile);
  SaveLabelingName(labelingFile);
  delete hfc;
  }

  void CompViewer::RunMaxDiff (float potts, float interCost, float bright, CutType c, char* fileName)  {
  int sel = 0;
  MaxDiffCut* mdc = new MaxDiffCut(_w,_h, &(_stackWidget->getImageVector()), _labels, sel);
  mdc->_potts = potts;
  mdc->_interCost = interCost;
  mdc->_bright = bright;
  mdc->setCuttype (c);
  mdc->compute();
  SetViewLabels(_labels);
  CreateTermVisualizations(mdc);
  _simpleCompImage.takeData(mdc->comp());
  //  _dc->setFilter(mdc);
  delete mdc;
  m_canvas->Refresh();
  char imageFile[300];
  sprintf(imageFile, "%s.jpg", fileName);
  char labelingFile[300];
  sprintf(labelingFile, "%s.lab", fileName);
  SaveImageName(imageFile);
  SaveLabelingName(labelingFile);
  }
*/
// convert -depth 8 -size 752x566 RGBA:out000.rgba tmp.bmp

void CompViewer::SaveMichael(wxCommandEvent& event) {
  const std::vector<ImageAbs*>& images = _stackWidget->getImageVector();
  char name[100];
  unsigned char *RGBA = new unsigned char[4*_w*_h];

  int i,j,k,index4, index;

  for (k=0; k<(int)images.size(); ++k) {
    if (!_dc->relevant(k)) continue;

    for (j=0,index=0,index4=0; j<_h; ++j)
      for (i=0; i<_w; ++i, index4+=4, ++index) {
	const unsigned char* data = images[k]->data(i,j);
	RGBA[index4]=data[0]; 	RGBA[index4+1]=data[1];     RGBA[index4+2]=data[2];
	RGBA[index4+3] = (_labels[index]==k) ? 0 : 255 ;
      }

    sprintf(name,"out%.3d.rgba",k);
    FILE* fp = fopen(name,"w");
    fwrite(RGBA,sizeof(unsigned char), 4*_w*_h, fp);
    fclose(fp);
  }

  delete[] RGBA; 
}

void CompViewer::SaveSources(wxCommandEvent& event) {
  const std::vector<ImageAbs*>& images = _stackWidget->getImageVector();
  char name[100];
  std::vector<ImageAbs*>::const_iterator c;
  int k, i, j;
  int borderWidth = int(.02 * float(MAX(_w,_h)));
  //int bw=_w+2*borderWidth, bh = _h+2*borderWidth;
  for (k=0, c=images.begin(); c!=images.end(); ++c, ++k) {
    sprintf(name,"source%.3d.png",k);
    wxImage imnew(_w,_h);
    Vec3i borderColor = _labelColors[k];
    
    for (j=0; j<_h; ++j)
      for (i=0; i<_w; ++i) {
	
	if (i<borderWidth || i>=_w-borderWidth ||
	    j<borderWidth || j>=_h-borderWidth) {
	  imnew.SetRGB(i,j, borderColor.r(), borderColor.g(), borderColor.b());

	}
	else {
	  const unsigned char* sourceData = images[k]->data(i, j);
	  imnew.SetRGB(i,j, sourceData[0], sourceData[1], sourceData[2]);
	}
	
      }

    wxString st(name);
    imnew.SaveFile(st);
    
  }
}

void CompViewer::uncheckAllBrushMenus ()
{
  _menuObjective->Check(ID_IMAGE, false);
  _menuObjective->Check (ID_COLORCLONE, false);
  _menuObjective->Check (ID_HIGHLIGHT, false);
  _menuObjective->Check (ID_SHADOW, false);
  _menuObjective->Check (ID_MAXHIST, false);
  _menuObjective->Check (ID_MINHIST, false);
  _menuObjective->Check (ID_MAXCONTRAST, false);
  _menuObjective->Check (ID_MAXDIFF, false);
  _menuObjective->Check (ID_TIMELAPSE, false);
  _menuObjective->Check (ID_PANORAMA, false);
  _menuObjective->Check (ID_DIFF_BRUSH, false);

  _menuAlignment->Check (ID_TOOL_TRANSLATE, false);
  _menuAlignment->Check (ID_TOOL_ROTATE, false);
  _menuAlignment->Check (ID_TOOL_SCALE, false);
  _menuAlignment->Check (ID_TOOL_POINT, false);

  _menuEdit->Check (ID_ERASE, false);
}

void CompViewer::DefaultFamilySettings (wxCommandEvent& event) {

  if (!_dc) return;
  SetCutType (C_GRAD);
  SetInertiaConstant (0.);
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_IMAGE, true);
  _brushType = S_NUM;
  _toolType = T_BRUSH;

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
}

void CompViewer::DefaultFaceSettings (wxCommandEvent& event) {
  if (!_dc) return;
  SetCutType (C_NORMAL);
  SetInertiaConstant (0.);
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_IMAGE, true);
  _brushType = S_NUM;
  _toolType = T_BRUSH;

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);
}

void CompViewer::DefaultBackgSettings (wxCommandEvent& event) {
  if (!_dc) return;
  SetCutType (C_NORMAL);
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_MAXHIST, true);
  _brushType = S_MAXHIST;
  _toolType = T_BRUSH;
  _dc->_regular_inter = .1;

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);

}

void CompViewer::DefaultEdofSettings (wxCommandEvent& event) {
  if (!_dc) return;
  SetCutType (C_NORMAL);
  uncheckAllBrushMenus();
  _menuObjective->Check(ID_MAXCONTRAST, true);
  _brushType = S_MAXCONTRAST;
  _toolType = T_BRUSH;
  _dc->_regular_inter = .1;

  decideStackViewerBrushing();
  m_canvas->setBrushEraser(false);

}

void CompViewer::DefaultSettings (wxCommandEvent& event) {
  if (!_dc) return;
  SetCutType (C_GRAD);
  SetInertiaConstant (0.01f);

}

//----------- NON-MEMBERS

BEGIN_EVENT_TABLE(MyStatusBar, wxStatusBar)
  EVT_SIZE(MyStatusBar::OnSize)
  END_EVENT_TABLE()

  void MyStatusBar::OnSize(wxSizeEvent& event) {
  wxRect rect;
  GetFieldRect(1, rect);
  
  _colorIconStaticBmp.Move(rect.x + (rect.width - C_COLORICONSIZE) / 2,
			   rect.y + (rect.height - C_COLORICONSIZE) / 2);
  event.Skip();
}

void wxImageToImageAbs(ImageAbs& ia, const wxImage& iw) {
  if (ia.allocated())
    ia.deallocate();
  ia._size.x = iw.GetWidth();
  ia._size.y = iw.GetHeight();
  unsigned char* data = new unsigned char[3*ia._size.x*ia._size.y];  
  memcpy(data, iw.GetData(), ia._size.x*ia._size.y*3*sizeof(unsigned char));
  ia.takeData(data);
}
