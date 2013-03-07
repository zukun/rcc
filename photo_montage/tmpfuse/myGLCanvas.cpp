#include <wx/wx.h>
#include <wx/image.h>

#include "myGLCanvas.h"


DEFINE_EVENT_TYPE(wxEVT_STROKE)
DEFINE_EVENT_TYPE(wxEVT_MYMOUSE_MOVE)
DEFINE_EVENT_TYPE(wxEVT_MYMOUSE_DOWN)
DEFINE_EVENT_TYPE(wxEVT_MYRMOUSE_DOWN)

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
  EVT_SIZE(MyGLCanvas::OnSize)
  EVT_PAINT(MyGLCanvas::OnPaint)
  EVT_ERASE_BACKGROUND(MyGLCanvas::OnEraseBackground)
  EVT_LEFT_DOWN(MyGLCanvas::MouseDown)
  EVT_LEFT_UP(MyGLCanvas::MouseUp)
  EVT_MOTION(MyGLCanvas::MouseMotion)
  EVT_RIGHT_DOWN(MyGLCanvas::RightMouseDown)
END_EVENT_TABLE()


MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id,
		       const wxPoint& pos, const wxSize& size, long style, const wxString& name, int* attribList):
  wxGLCanvas(parent, id, pos, size, style, name, attribList ) {
  _viewImage = _overlayImage = NULL;
  _viewOverlay = false;
  SetCurrent();
  _currBrush = NULL;
  _brushingEnabled = false; 
  _myRefresh = false;
  //_rendered = NULL;
  _initLevel = 1;
  _inited = 0;
  _constructed = 72;
  _cummOverlay = NULL;
  _viewCummOverlay = false;
  _compw = _comph = 0;
  _mouseDown = _altDown = _ctrlDown = _rightMDown = false;
  //_brushType = S_NUM;
  //InitGL(); 
  _initGL = false;
  //_blendCummOverlay = false;
  _brushColor = Vec3f(0, 1., 1.);
  SetSizeHints(50,50);
  //printf("Finished construction %x\n", this);
  _currPoints = NULL;
  parent->Show(TRUE);
  _brushIsEraser = false;
  _whichCorr = 0;
}

MyGLCanvas::~MyGLCanvas()
{
  //if (_rendered)
  //delete _rendered;
  if (_cummOverlay) delete[] _cummOverlay;
}

void MyGLCanvas::redrawRendered() {
  assert(!glGetError());
  SetCurrent();
  glClear(GL_COLOR_BUFFER_BIT); 
  if (_viewImage) {
    displayError();
    drawImage(_viewImage);    
    displayError();
  }

  else 
    glClear(GL_COLOR_BUFFER_BIT);

  glPushAttrib(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);

  // for frankenface viewing
  if (_viewOverlay && _overlayImage && _overlayImage->allocated()) {
    displayError();
    drawImage(_overlayImage,.5);   

    displayError();  
  }

  // for strokes
  if (_viewCummOverlay && _cummOverlay) {
    glPixelZoom(1.f,-1.f);
    glRasterPos2i(0,0);
    if (_brushIsEraser) {
      glEnable(GL_STENCIL_TEST);
      glStencilFunc(GL_EQUAL, 0, 0x1);
      glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    }
    
    drawCummOverlay();

    if (_brushIsEraser)
      glDisable(GL_STENCIL_TEST);
    glPixelZoom(1.f,1.f);
  }

  displayError();
  glPopAttrib();

  // for corr points
  if (_currPoints) {
    glPointSize(5.0);
    glColor3f(1., 1., 1.);
    glBegin(GL_POINTS);
    CorrVec::const_iterator c = _currPoints->begin();
    for ( ; c!= _currPoints->end(); ++c) {
      Coord cd;
      if (_whichCorr == 0)
	cd = c->first;
      else
	cd = c->second;
      glVertex2i(cd.x, cd.y);
      //printf("Rendering poing %d %d\n",cd.x, cd.y);
    }
    glEnd();
  }
  
  displayError();
}


void MyGLCanvas::OnPaint( wxPaintEvent& event )
{

  //printf("%x paint\n", this);
  wxPaintDC dc(this);
  if (!GetContext()) return;

  //printf("%x paint 2\n", this);
  SetCurrent();


  if (!_initGL) {
    InitGL();
    _initGL = TRUE;
  }

 
  if (_constructed==72 && _inited >= _initLevel) {
    //assert(_rendered && (*_rendered==0 || *_rendered != 0));
    if (!_myRefresh) {

      redrawRendered(); 
      glFlush();
      SwapBuffers();
    }
    else {
      _myRefresh = false;
      glFlush();
    }
    //glDrawBuffer(GL_FRONT);

    /*
    glRasterPos2i( 0, 0 );
    glPixelStorei(GL_UNPACK_ROW_LENGTH, _w);
    glDrawPixels(_w, _h, GL_RGB, GL_UNSIGNED_BYTE, _rendered);*/ 
    
  
    //glFlush();
    //SwapBuffers();
  }
  else
    glClear(GL_COLOR_BUFFER_BIT);

  assert(!glGetError());
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
{
  //printf("On Size %x\n", this);
  // this is also necessary to update the context on some platforms
  wxGLCanvas::OnSize(event);
  SetCurrent();

  // set GL viewport (not called by wxGLCanvas::OnSize on all platforms...)
  GetClientSize(&_w, &_h);
  assert(_w>0 && _h>0);

  if (_constructed==72 && GetContext()) {
    
    if (!_initGL) {
      InitGL();
      _initGL = TRUE;
    }

    assert(!glGetError());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, (GLfloat) _w, 0.0f, 
	       (GLfloat) _h);
    glMatrixMode(GL_MODELVIEW);
    myGlLoadIdentity();    	
    glViewport(0, 0, (GLint) _w, (GLint) _h);
    //if (_rendered) delete[] _rendered;
    //_rendered = new unsigned char[3*_w*_h];
    //glPixelStorei(GL_PACK_ROW_LENGTH, _w);
	assert(!glGetError());
    ++_inited;
    redrawRendered();
  }
  //printf("On Size %x 2\n", this);
}

void MyGLCanvas::OnEraseBackground(wxEraseEvent& event)
{
  // Do nothing, to avoid flashing.
}

void MyGLCanvas::InitGL()
{
  SetCurrent();
  assert(GetContext()); 
  glClearColor( 1.0, 1.0, 1.0, 0.0 );
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glClearDepth(1.0);
  glClear(GL_DEPTH_BUFFER_BIT);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glDrawBuffer(GL_BACK);
  displayError();

  glClearStencil(0x0);
  glDisable(GL_STENCIL_TEST);
  glDisable(GL_ALPHA_TEST);

  glDisable( GL_BLEND ) ;
  glAlphaFunc(GL_GREATER, 0.0);
  
  int bits;
  glGetIntegerv(GL_STENCIL_BITS, &bits);
  assert(bits>0);
  GLboolean mode;
  glGetBooleanv(GL_RGBA_MODE, &mode);
  assert(mode);
  
  //glGetIntegerv(GL_MAX_TEXTURE_SIZE, &bits);
  //printf("%d\n",bits); exit(0);

  assert(!glGetError());
}


void MyGLCanvas::SetOverlayImage(const ImageAbs* ia) {
  _overlayImage = ia;
}

void MyGLCanvas::SetViewImage(const ImageAbs* ia) {
  assert(ia);
  _viewImage = ia;
  assert(_viewImage->valid());
}

void MyGLCanvas::myGlLoadIdentity() {
  glLoadIdentity();
  glTranslatef(0,_h,0);
  glScalef(1.,-1.,1.);
}


void MyGLCanvas::MouseDown(wxMouseEvent& event) {
  _mouseDown = true;
  _altDown = event.ShiftDown();
  _ctrlDown = event.ControlDown();

  _mx = event.GetX(), _my = event.GetY();

//  _currColor.set_r(rand()%256);
//  _currColor.set_g(rand()%256);
//  _currColor.set_b(rand()%256);
  // send event to parent viewer
  wxCommandEvent eventCustom(wxEVT_MYMOUSE_DOWN);
  ProcessEvent(eventCustom);

  if (_brushingEnabled && !_ctrlDown && !_altDown) {
    //if (_brushType == S_COLOR && _altDown) return; 
    //if (_brushType == S_NUM && _altDown) return; 
    //printf("Mouse down %d %d\n", event.GetX(), event.GetY());
    assert(!_currBrush);
    SetCurrent();
    glClear(GL_STENCIL_BUFFER_BIT);
    _currBrush = new Brush(_brushIsEraser, event.GetX(), event.GetY());    
    _currBrush->setColor(_brushColor);
    if (!_brushIsEraser) {
      glDrawBuffer(GL_FRONT);
      _currBrush->renderRecent();
      glDrawBuffer(GL_BACK);
    }    
    else
      _currBrush->renderRecent();
    MyRefresh();
  }
}

void MyGLCanvas::RightMouseDown(wxMouseEvent& event) {

	if (event.RightDown ())
	  _rightMDown = true;

  _mx = event.GetX(), _my = event.GetY();

  // send event to parent viewer
  wxCommandEvent eventCustom(wxEVT_MYRMOUSE_DOWN);
  ProcessEvent(eventCustom);
}
void MyGLCanvas::MouseUp(wxMouseEvent& event) {
  _mouseDown = false;
  _altDown = event.ShiftDown();
  _ctrlDown = event.ControlDown();

  if (_brushingEnabled && _currBrush && !_ctrlDown && !_altDown) {

    //if (_brushType == S_COLOR && _altDown) return; 

    //printf("Mouse up %d %d\n", event.GetX(), event.GetY());
    assert(_currBrush);
    SetCurrent();

    // handle new brush stroke

    assert(_compw > 0 && _comph > 0);
    _currBrush->grabStencil(_compw, _comph, _h-_comph);
    glClear(GL_STENCIL_BUFFER_BIT);

    wxCommandEvent eventCustom(wxEVT_STROKE);
    ProcessEvent(eventCustom);

    delete _currBrush; _currBrush = NULL;
    //redrawRendered();
    Refresh();
  }

 
}

void MyGLCanvas::MouseMotion(wxMouseEvent& event) {
  _mx = event.GetX(), _my = event.GetY();
  _altDown = event.ShiftDown();
  _ctrlDown = event.ControlDown();

  // send event to parent viewer
  wxCommandEvent eventCustom(wxEVT_MYMOUSE_MOVE);
  ProcessEvent(eventCustom);

  if (_brushingEnabled && _currBrush && event.Dragging() && event.LeftIsDown() && !_ctrlDown && !_altDown) {

    //if (_brushType == S_COLOR && _altDown) return; 

    //printf("Mouse move %d %d\n", event.GetX(), event.GetY());
    _currBrush->move(_mx, _my);

    if (!_brushIsEraser) {
      glDrawBuffer(GL_FRONT);
      _currBrush->renderRecent();
      glDrawBuffer(GL_BACK);
    }    
    else
      _currBrush->renderRecent();

    MyRefresh();
  }
}

void MyGLCanvas::MyRefresh() {
  if (!_brushIsEraser) 
    _myRefresh = true;
  Refresh();
}


void MyGLCanvas::buildCummOverlay() {
  assert(_compw > 0 && _comph > 0);
  _cummOverlay = new unsigned char[4*_compw*_comph];
  int index,i,j;
  index=0;
  for (j=0; j<_comph; ++j)
    for (i=0; i<_compw; ++i, index+=4) {
      _cummOverlay[index] = _cummOverlay[index+1] = 0;
      _cummOverlay[index+2] = 255;
      _cummOverlay[index+3] = 0; 
    }
}


 void MyGLCanvas::viewCummOverlay(const bool b) { 
   _viewCummOverlay = b; 
   
   //if (_viewCummOverlay)
   //glEnable(GL_BLEND);
   //else
   // glDisable(GL_BLEND);
 }


void MyGLCanvas::viewOverlay() {
  _viewOverlay = true;
  Refresh();
}

void MyGLCanvas::dontViewOverlay() {
  _viewOverlay = false;
  Refresh();
}


void MyGLCanvas::drawCummOverlay() {
  int w=_compw, h=_comph;
  myGlLoadIdentity();
  glShadeModel(GL_FLAT);
  const unsigned char* d = _cummOverlay;
  for (int j=0; j<h; ++j) {
    glBegin(GL_QUAD_STRIP);
    for (int i=0; i<w; ++i, d+=4) {
      glVertex2s(i,j);
      glVertex2s(i,j+1);
      glColor4ubv(d);
    }
    glEnd();
  }
  
  myGlLoadIdentity();
  displayError();
}

void MyGLCanvas::drawImage(const ImageAbs* ia, const float a) {
  displayError();
  if (a==0) return;
  
  /*
  if (0 && !ia->rotated()) { 
    //glPixelZoom(1.f,-1.f);
    glPixelZoom(ia->scale(), -ia->scale());
    if (a!=1.)
      glPixelTransferf(GL_ALPHA_SCALE, a);
    
    glRasterPos2i(MAX(0,ia->displacement().x), MAX(0,ia->displacement().y));
    int offx = -MIN(0,ia->displacement().x), offy = -MIN(0, ia->displacement().y);
    int dataOffset = 3*(offy*ia->_size.x + offx);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, _viewImage->_size.x);
    glDrawPixels(ia->_size.x - offx, ia->_size.y - offy,
		 GL_RGB,GL_UNSIGNED_BYTE,ia->_data + dataOffset);
    
    glPixelZoom(1.f,1.f);
    if (a!=1.)
      glPixelTransferf(GL_ALPHA_SCALE, 1.);

  }
  
  else {
  */
  int w=ia->_size.x, h=ia->_size.y;
  myGlLoadIdentity();
  /*
  glTranslatef(w/2, h/2,0);
  glRotatef(ia->angle(), 0,0,1);
  glTranslatef(-w/2, -h/2,0);
  glTranslatef(ia->displacement().x, ia->displacement().y, 0);
  glScalef(ia->scale(), ia->scale(), ia->scale());
  */

  glTranslatef(w/2, h/2,0);  
  float M[16];
  ia->getGLTrans(M);
  glMultMatrixf(M);
  glTranslatef(-w/2, -h/2,0); 

  glPushAttrib(GL_COLOR_BUFFER_BIT);
  if (a==1)
    glDisable(GL_BLEND);
  else 
    glEnable(GL_BLEND);

  short sh=(short)h, sw=(short)w;
  glShadeModel(GL_FLAT);
  const unsigned char* d = ia->data();
  unsigned char ab = (unsigned char)(a * float(255));
  if (a==1) {
    for (short j=0; j<sh; ++j) {
      glBegin(GL_QUAD_STRIP);
      for (short i=0; i<sw; ++i, d+=3) {
	glVertex2s(i,j);
	glVertex2s(i,j+1);
	glColor3ubv(d);
      }
      glEnd();
    }
  }

  else {
    for (short j=0; j<sh; ++j) {
      glBegin(GL_QUAD_STRIP);
      for (short i=0; i<sw; ++i, d+=3) {
	glVertex2s(i,j);
	glVertex2s(i,j+1);
	glColor4ub(d[0], d[1], d[2], ab);
      }
      glEnd();
    }
  }
   
  myGlLoadIdentity();
  glPopAttrib();


  displayError();
}

void MyGLCanvas::renderCorrPoints (const CorrVec* cps, int which) {
  _currPoints = cps;
  _whichCorr = which;
}

void MyGLCanvas::dontRenderCorrPoints() {
  _currPoints = NULL;
}
