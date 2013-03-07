//#undef _DEBUG
#include <wx/image.h>
#include <wx/string.h>
#include <fstream>
#include "stackViewer.h"


BEGIN_EVENT_TABLE(StackViewer, wxFrame)
  EVT_SPINCTRL(ID_ZCHANGE, StackViewer::zChange)
  EVT_CUSTOM(wxEVT_MYMOUSE_MOVE, -1, StackViewer::handleMouseMove)
  //  EVT_CUSTOM(wxEVT_MYRMOUSE_DOWN, -1, StackViewer::handleRMouseDown)
  EVT_CUSTOM(wxEVT_MYMOUSE_DOWN, -1, StackViewer::handleMouseDown)
END_EVENT_TABLE()



StackViewer::StackViewer(int w, int h, const wxString& baseName, wxFrame *frame, const wxString& title, 
			 const wxPoint& pos, const wxSize& size, const wxArrayString& names)
  : BaseGLViewer(frame, title, pos, size), _w(w), _h(h)
{
  _filenames = names;
  
  wxImage im(baseName);
  assert(im.Ok());
  ImageAbs* ia = new ImageAbs();
  _images.push_back(ia);
  wxImageToImageAbs(*ia, im);

  uint i=0;
  while (i<_filenames.Count()) {
    if (baseName.Cmp(_filenames[i])==0)   // this image is same as base image, lo0se it
      _filenames.Remove(i);
  
    else {
      wxImage im(_filenames[i]);
      assert(im.Ok());
      ImageAbs* ia = new ImageAbs();
      _images.push_back(ia);
      wxImageToImageAbs(*ia, im);
      ++i;      
    }
  }

  _count = _images.size();
  _corrs.resize(_count);
  _createPoint = false;
  //  _distPlot = NULL;

  CreateStatusBar();
  //  SetStatusText( "Aseem needs to relax!" );
}

/*
StackViewer::StackViewer(int w, int h, char* stackDescrip, wxFrame *frame, const wxString& title, 
			 const wxPoint& pos, const wxSize& size, const wxArrayString& names)
  : BaseGLViewer(frame, title, pos, size), _w(w), _h(h) {

  std::ifstream fp(stackDescrip);
  assert(fp);
  
  fp >> _count;
  char cstring[300];
  for (int i=0; i<_count; ++i) {
    //wxString str;
    fp >> cstring;
    _filenames.Add(wxString(cstring));
    wxImage im(_filenames[i]);
    assert(im.Ok());
    ImageAbs* ia = new ImageAbs();
    _images.push_back(ia);
    wxImageToImageAbs(*ia, im);
    fp >> ia->_displacement;
  }

  CreateStatusBar();
  SetStatusText( "Mira kicks ass!" );
}
*/

void StackViewer::finishInit() {
  _spin = new wxSpinCtrl(this, ID_ZCHANGE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS,
			 0, _count-1, 0);
  
  wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
  sizer->Add(_spin,0,wxALIGN_LEFT,5);
  sizer->Add(m_canvas,1,wxGROW, 5);
  SetSizer( sizer );      // use the sizer for layout

  _spin->SetValue(0);
  setCurrImage(0);
  m_canvas->initStep();
  //m_canvas->enableBrushing();
  resize();
}

StackViewer::~StackViewer() {
  for (int i=0; i<_count; ++i) {
    delete _images[i];
  }
  //delete[] _images;
}


void StackViewer::zChange(wxSpinEvent& evt) {
  //printf("non-sorted spin event\n");
  setCurrImage(evt.GetPosition());
  //m_canvas->redrawRendered();
  m_canvas->Refresh();
  evt.Skip();
}

void StackViewer::setCurrImage(const int i) {
  assert(i>=0 && i<_count);
  _currImage = i;
  SetViewImage(_images[i]);
}

void StackViewer::externalSetImage(const int i) {
  setCurrImage(i);
  _spin->SetValue(i);
  m_canvas->Refresh();
}

void StackViewer::resize() {
    SetClientSize(_images[0]->getWidth(), _images[0]->getHeight() + _spin->GetSize().GetHeight() + 2);
}

void StackViewer::handleMouseDown(wxCommandEvent& event) {
  if (_createPoint) {
    int x = m_canvas->mx(), y = m_canvas->my();
    getCurrCorrVec().back().second = Coord(x,y);
    printf("added second %d %d\n",x,y);
    _createPoint = false;
    handlePointTransform();
    m_canvas->Refresh();
  }
}

void StackViewer::handleMouseMove(wxCommandEvent& evt) {
  wxString caption;
  int x=m_canvas->mx(), 
    y=m_canvas->my();
  caption << "(" << x << ", " << y << ")";
  SetStatusText(caption);
}
/*
void StackViewer::handleRMouseDown(wxCommandEvent& event) {
	
  //int x = m_canvas->mx(), y = m_canvas->my();
  
  if (m_canvas->rightMDown () && _distPlot) {
    
        if (_distPlot)
         _distPlot->handleRMouseDown (event);
     }
}
*/

void StackViewer::handlePointTransform() {
  const CorrVec& cv = getCurrCorrVec();
  if (cv.size() < 3)
    return; 

  BestFitAffine bfa(cv, float(_w)/2., float(_h)/2.);
  if (!bfa.successful())
    return;
  const float* M = bfa.getTransform();
  _images[_currImage]->setTrans(M);
  
}

void StackViewer::enableCreatePoint() {
  _createPoint = true;
}

void StackViewer::disableCreatePoint() {
  _createPoint = false;
}



const ImageAbs* StackViewer::getCurrImage() const {
  return (_images[_currImage]);
}


void StackViewer::translateCurrImage(const int x, const int y) {
  _images[_currImage]->translate(x,y);
}

void StackViewer::scaleCurrImage(const float s) {
  _images[_currImage]->scale(s);
}

/*
void StackViewer::setCurrImageScale(const float s) {
  _images[_currImage]->setScale(s);
}
*/

void StackViewer::rotateCurrImage(const float a) {
  _images[_currImage]->rotate(a);
}


/*
void StackViewer::save(char* name) {
  std::ofstream fp(name);
  assert(fp);
  fp << _count << std::endl;
  for (int i=0; i<_count; ++i)
	  fp << _filenames[i] << " " << _images[i]->_displacement << std::endl;
}
*/

void StackViewer::addImageToStack(ImageAbs* im) {
  ++_count;
  _filenames.Add(wxString());
  _images.push_back(im);    
  _spin->SetRange(0,_count-1);
}

void StackViewer::removeLastImage() {
	--_count;
	_filenames.Remove(_filenames.Last());
	_images.pop_back (); 
    _spin->SetRange(0,_count-1);
}


const CorrVec& StackViewer::getCurrCorrVec() const {
  assert(_currImage <(int)_corrs.size());
  return _corrs[_currImage];
}

CorrVec& StackViewer::getCurrCorrVec() {
  assert(_currImage <(int)_corrs.size());
  return _corrs[_currImage];
}
