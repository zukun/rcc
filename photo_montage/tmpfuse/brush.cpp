#include <stdio.h>
#include <iostream>
#include "brush.h"

Brush::Brush(const bool tp, const int x, const int y) : _isEraser(tp) {
  _locs.push_back(Coord(x,y));
  _quadric = gluNewQuadric();
  assert(_quadric);
  _stencil = NULL;
  _sw = _sh = 0;
}

Brush::~Brush() {
  if (_stencil) delete[] _stencil;
}

void Brush::move(const int x, const int y) {
  //_cx += dx; _cy +=dy;
  _locs.push_back(Coord(x,y));
}

void Brush::render() {
  displayError();
  if (_isEraser) return;
  glColor3f(0,0,1);
  for (unsigned int i=0; i<_locs.size(); ++i) {
    glPushMatrix();
    glTranslatef(_locs[i].x, _locs[i].y, 0);
    //printf("rendering disk %d %d\n",_locs[i].x, _locs[i].y);
    gluDisk(_quadric, 0, B_RADIUS, 10, 1);
    glPopMatrix();
  }
  displayError();
}

void Brush::renderRecent() {
  assert(!glGetError());
  glEnable(GL_STENCIL_TEST);
  glStencilFunc(GL_ALWAYS,1,0x1);
  glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

  glPushAttrib(GL_ENABLE_BIT); // very important for erase brush, speedup for others
  glDisable(GL_ALPHA_TEST);

  if (!_isEraser)
    glColor3f(_color.r(), _color.g(), _color.b());
  else
    glColor4f(0,0,0.,0); 


  bool _justOne = false; double dist;
  int i = _locs.size()-1;
  if (_locs.size()>=2) {
    dist=_locs[i].distTo(_locs[i-1]);
    if (dist <= 3)
      _justOne = true;
  }
  else _justOne = true;
  
  int myRad = B_RADIUS;
  if (_isEraser)
    myRad +=5;

  if (_justOne) {
    glPushMatrix();
    glTranslatef(_locs[i].x, _locs[i].y, 0);
    gluDisk(_quadric, 0, myRad, 10, 1);
    glPopMatrix();
  }

  else {
    int steps = (int) ceil( dist / 3. );
    //double stepLength = double(dist) / double(steps);
    double dx = double(_locs[i].x - _locs[i-1].x) / double(steps);
    double dy = double(_locs[i].y - _locs[i-1].y) / double(steps);
    double x = _locs[i-1].x, y = _locs[i-1].y;
    for (int j=1; j<=steps; ++j) {
      x += dx; y += dy;
      glPushMatrix();
      glTranslatef(x,y,0);
      gluDisk(_quadric, 0, myRad, 10, 1);
      glPopMatrix();
    }
  }

  glPopAttrib();
  glDisable(GL_STENCIL_TEST);
  assert(!glGetError());
}

/*
//#define A_swap(a,b) c=a; a=b; b=c;
void A_swap(short& a, short& b) {
  short c = a;
  a = b;
  b = c;
  }*/


void Brush::orStencil(const short* s) {
  assert(_stencil && _sw>0 && _sh>0);
  int i,j,index=0;
  for (j=0; j<_sh; ++j)
    for (i=0; i<_sw; ++i, ++index)
      _stencil[index] = _stencil[index] || s[index];
}

void Brush::grabStencil(int sw, int sh, int hoffset) {
  assert(!glGetError());
  _sw = sw; _sh = sh;
  _stencil = new short[_sw*_sh];    

  glEnable(GL_STENCIL_TEST);
  glReadPixels(0,hoffset,_sw,_sh, GL_STENCIL_INDEX, GL_SHORT, _stencil);
  glDisable(GL_STENCIL_TEST);
  //assert(!glGetError());
  displayError();

  /*
    int count=0;
    for (int i=0; i<_sw*_sh; ++i) {
      if (_stencil[i] == 1)
	++count;
    }
    printf("Stencil count %d\n",count);
  */

  // flip the stencil upside down
  int i, j=0, bj=_sh-1;
  while (j<bj) {
    for (i=0; i<_sw; ++i) 
      std::swap(_stencil[j*_sw + i], _stencil[bj*_sw + i]);
    ++j; --bj;
  }
}


void displayError(){

	GLenum error = glGetError();

	switch(error){

	case GL_NO_ERROR:
	  //printf("clean\n");
		break;
	case GL_INVALID_ENUM:
	  std::cout<<"invalid enum"<<std::endl;  assert(0);
		break;
	case GL_INVALID_VALUE:
		std::cout<<"invalid value"<<std::endl;assert(0);
		break;
	case GL_INVALID_OPERATION:
		std::cout<<"invalid operation"<<std::endl;assert(0);
		break;
	case GL_STACK_OVERFLOW:
		std::cout<<"stack overflow"<<std::endl;assert(0);
		break;
	case GL_STACK_UNDERFLOW:
		std::cout<<"stack underflow"<<std::endl;assert(0);
		break;
	case GL_OUT_OF_MEMORY:
		std::cout<<"out of memory"<<std::endl;assert(0);
		break;
	}

}
