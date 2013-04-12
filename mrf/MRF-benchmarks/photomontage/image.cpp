#include "image.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

/*
Image::Image() {
  _w=-1; _h=-1;
  _data = NULL;
}
*/

Image::~Image() {
  if (_data != NULL) delete[] _data;
  if (_gradmags != NULL) delete[] _gradmags;
}

Image::Image(CType* data, const int w, const int h) {
  assert(data != NULL);
  _w = w; _h = h;
  _data = data;
  _gradmags = NULL;
}

Image::Image(const int w, const int h) {
  _w = w; _h = h;
  _data = new CType[_NUMC_*_w*_h];
  _gradmags = NULL;
}

Image::Image(char* name) {
  _data = NULL;
  int numc;
   read_png(name, &_w, &_h, &_data, &numc);
  assert(_data && numc==_NUMC_);
  _gradmags  = NULL;
}



void Image::createAuxData() {
  _gradmags = new GradMag[_w*_h];
  computeAllGradMags();
}

bool Image::trans(const int x, const int y) {
  //return (_data[_NUMC_*REF(x,y) + 3] == 0); // _NUMC_=4
  int ind = _NUMC_*REF(x,y);
  return (_data[ind]==0 && _data[ind+1]==0 && _data[ind+2]==0);
}

float Image::distTo(const Coord& p, const Image* other) {
  float dist = 0, a;
  int ind = _NUMC_*REF(p.x,p.y);
  for (int c=0; c<3; ++c, ++ind) {
    a = float(_data[ind]) - float(other->_data[ind]);
    dist += a*a;
  }

  dist = sqrt(dist);
  return dist;
}

void Image::setPix(const int i, const int j, const Image* o) {
  int ind = _NUMC_*REF(i,j);
  memcpy(_data+ind,o->_data+ind,_NUMC_*sizeof(CType));
}


void Image::setChPix(const CType datum, const int c, const int i, const int j) {
  assert(c>=0 && c<_NUMC_);
  CType* mpix = getPix(i,j);
  assert(mpix);
  mpix[c] = datum;
}


void Image::computeAllGradMags() {
  //printf("computing grad mags\n");
  assert(_gradmags);
  int i, j, index=0;
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index) {
      Coord loc(i,j);
      _gradmags[index]._x = calcHorizGradMag(loc);
      _gradmags[index]._y = calcVertGradMag(loc);
    }
}

float Image::calcHorizGradMag(const Coord& loc) {
  if (loc.x==0 || loc.x==_w-1 || loc.y==_h-1)
    return -1.f;
  if (trans(loc.x-1,loc.y) || trans(loc.x,loc.y) || trans(loc.x+1,loc.y) ||
      trans(loc.x-1,loc.y+1) || trans(loc.x,loc.y+1) || trans(loc.x+1,loc.y+1))
    return -1.f;
  float sum=0, d;
  for (int c=0; c<3; ++c) {  // iterate over colors
    d = float(_data[REFC(loc.x-1,loc.y)+c]) +   2.f*float(_data[REFC(loc.x,loc.y)+c]) +   float(_data[REFC(loc.x+1,loc.y)+c]) -
        float(_data[REFC(loc.x-1,loc.y+1)+c]) - 2.f*float(_data[REFC(loc.x,loc.y+1)+c]) - float(_data[REFC(loc.x+1,loc.y+1)+c]);
    d /= 3.f;
    sum += d*d;
  }
  d =  sqrt(sum);
  assert(finite(d) && !isnan(d));
  //if (d!=0) printf("horiz %f\n",d);
  return d;
}

float Image::calcVertGradMag(const Coord& loc) {
  if (loc.y==0 || loc.y==_h-1 || loc.x==_w-1)
    return -1.f;
  if (trans(loc.x,loc.y-1) || trans(loc.x,loc.y) || trans(loc.x,loc.y+1) ||
      trans(loc.x+1,loc.y-1) || trans(loc.x+1,loc.y) || trans(loc.x+1,loc.y+1))
    return -1.f;
  float sum=0, d;
  for (int c=0; c<3; ++c) {  // iterate over colors
    d = float(_data[REFC(loc.x,loc.y-1)+c]) +   2.f*float(_data[REFC(loc.x,loc.y)+c]) +   float(_data[REFC(loc.x,loc.y+1)+c]) -
        float(_data[REFC(loc.x+1,loc.y-1)+c]) - 2.f*float(_data[REFC(loc.x+1,loc.y)+c]) - float(_data[REFC(loc.x+1,loc.y+1)+c]);
    d /= 3.f;
    sum += d*d;
  }
  d =  sqrt(sum);
  assert(finite(d) && !isnan(d));
  //if (d!=0) printf("vert %f\n",d);
  return d;
}
