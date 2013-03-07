#include "imageData.h"

ImageData::ImageData(std::vector<ImageAbs*>* images, const int w, const int h) : _w(w), _h(h) {
  _wh = _w*_h;
  _n = images->size();
  _images = *images;

  for (unsigned int i=0; i<_n; ++i) {
    _gradmags.push_back(NULL);
    _cgrads.push_back(NULL);
  }
}

ImageData::~ImageData() {
  for (int i=0; i<_n; ++i) {
    if (_gradmags[i])
      delete[] _gradmags[i];
    if (_cgrads[i])
      delete[] _cgrads[i];
  }
}

//--------------------------------------------


void ImageData::allocateComputeGradMag(ushort l) {
  if (_gradmags[l] == NULL) {
    GradMag* gm = new GradMag[_w*_h];
    _gradmags[l] = gm;
    computeGradMag(l,gm);  
  }
}

void ImageData::allocateComputeAllGradMags() {
  printf("COmputing all grad mags\n");
  for (int i=0; i<_n; ++i) {
    allocateComputeGradMag(i);
  }
  printf("Finished computing all grad mags\n");
}

void ImageData::computeGradMag(ushort l, GradMag* grad) {
  int i,  j,index=0;
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index) {
      Coord loc(i,j);
      grad[index]._x = horizGradMag(l,loc);
      grad[index]._y = vertGradMag(l,loc);
    }
}

float ImageData::vertGradMagLookup(ushort l, const Coord& loc) {
  if (!_gradmags[l]) {
    assert(0);
    printf("VertGradMagLookup %d failed!\n", l); exit(0);
    return vertGradMag(l,loc);
  }
  float res = (float) ((_gradmags[l])[loc.y*_w+loc.x])._y;
  //assert(_finite(res) && !_isnan(res));
  return res;
}

float ImageData::vertGradMag(ushort l, const Coord& loc) {
  float sum=0, d;
  for (int c=0; c<3; ++c) {  // iterate over colors
    d = float(_imptr(l,Coord(loc.x,loc.y-1))[c]) + 2.f*float(_imptr(l,loc)[c]) + float(_imptr(l,Coord(loc.x,loc.y+1))[c]) -
      float(_imptr(l,Coord(loc.x+1,loc.y-1))[c]) - 2.f*float(_imptr(l,Coord(loc.x+1,loc.y))[c]) - float(_imptr(l,Coord(loc.x+1,loc.y+1))[c]);
    d /= 3.f;
    sum += d*d;
  }
  d = sqrt(sum); 
  //assert(_finite(d) && !_isnan(d));
  return d;
}

float ImageData::horizGradMagLookup(ushort l, const Coord& loc) {
  if (!_gradmags[l]) {
    assert(0);
    printf("HorizGradMagLookup %d failed!\n", l);  exit(0);
    return horizGradMag(l,loc);
  }
  float res = (float) ((_gradmags[l])[loc.y*_w+loc.x])._x;
  //assert(_finite(res) && !_isnan(res));
  return res;
}

float ImageData::horizGradMag(ushort l, const Coord& loc) {
  float sum=0, d;
  for (int c=0; c<3; ++c) {  // iterate over colors
    d = float(_imptr(l,Coord(loc.x-1,loc.y))[c]) + 2.f*float(_imptr(l,loc)[c]) + float(_imptr(l,Coord(loc.x+1,loc.y))[c]) -
      float(_imptr(l,Coord(loc.x-1,loc.y+1))[c]) - 2.f*float(_imptr(l,Coord(loc.x,loc.y+1))[c]) - float(_imptr(l,Coord(loc.x+1,loc.y+1))[c]);
    d /= 3.f;
    sum += d*d;
  }
  d =  sqrt(sum);
  //assert(_finite(d) && !_isnan(d));
  return d;
}

//--------------------------------------------

void ImageData::allocateComputeCGrad(ushort l) {
  if (_cgrads[l] == NULL) {
    CGrad* gm = new CGrad[_w*_h];
    _cgrads[l] = gm;
    computeCGrad(l,gm);  
  }
}

void ImageData::allocateComputeAllCGrads() {
  printf("COmputing all cgrads\n");
  for (int i=0; i<_n; ++i) {
    allocateComputeCGrad(i);
  }
  printf("Finished computing all cgrads\n");

}



void ImageData::computeCGrad(ushort l, CGrad* grad) {
  int i,  j,index=0;
  for (j=0; j<_h; j++)
    for (i=0; i<_w; i++, ++index) {
      Coord loc(i,j);
      horizCGrad(l,loc, grad[index]._x);
      vertCGrad(l,loc, grad[index]._y);
    }
}

void ImageData::computeCGrad(const ushort l, const int i, const int j, CGrad& here) {
  Coord loc(i,j);
  horizCGrad(l,loc, here._x);
  vertCGrad(l,loc, here._y);
}

void ImageData::vertCGrad(ushort l, const Coord& loc, float y[3]) {
  for (int c=0; c<3; ++c) {  // iterate over colors
    //y[c] = float(_imptr(l,Coord(loc.x,loc.y-1))[c]) + 2.f*float(_imptr(l,loc)[c]) + float(_imptr(l,Coord(loc.x,loc.y+1))[c]) -
    //float(_imptr(l,Coord(loc.x+1,loc.y-1))[c]) - 2.f*float(_imptr(l,Coord(loc.x+1,loc.y))[c]) - float(_imptr(l,Coord(loc.x+1,loc.y+1))[c]);
    y[c] = float(_imptr(l,Coord(loc.x,loc.y+1))[c]) - float(_imptr(l,loc)[c]);
  }
}

void ImageData::horizCGrad(ushort l, const Coord& loc, float x[3]) {
  for (int c=0; c<3; ++c) {  // iterate over colors
    //x[c] = float(_imptr(l,Coord(loc.x-1,loc.y))[c]) + 2.f*float(_imptr(l,loc)[c]) + float(_imptr(l,Coord(loc.x+1,loc.y))[c]) -
    //float(_imptr(l,Coord(loc.x-1,loc.y+1))[c]) - 2.f*float(_imptr(l,Coord(loc.x,loc.y+1))[c]) - float(_imptr(l,Coord(loc.x+1,loc.y+1))[c]);
    x[c] = float(_imptr(l,Coord(loc.x+1,loc.y))[c]) - float(_imptr(l,loc)[c]);
  }
}

const CGrad& ImageData::cgradLookup(ushort l, const Coord& loc) const {
  if (!_cgrads[l]) {
    printf("cgradLookup for %d failed\n", l); 
    assert(0);
    exit(0);
  }
  return ((_cgrads[l])[loc.y*_w+loc.x]);
}

void ImageData::addImage(ImageAbs* im) {
  _images.push_back(im);
  _gradmags.push_back(NULL);
  _cgrads.push_back(NULL);  
  ++_n;
}

void ImageData::removeLastImage() {
  _images.pop_back();
  if (_gradmags[_gradmags.size()-1] != NULL)
    delete _gradmags[_gradmags.size ()-1];
  _gradmags.pop_back();
  if (_cgrads[_cgrads.size()-1] != NULL)
    delete _cgrads[_cgrads.size ()-1];
  _cgrads.pop_back();  
  --_n;
}



float ImageData::getLum(const Coord& p, const ushort d) const {
  const unsigned char* I = _imptr(d,p);
  return (.3086f * (float)I[0] + .6094f * (float)I[1] + .082f * (float)I[2]);
}

