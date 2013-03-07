#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <stdio.h>
#include <vector>
#include "../ImageAbs.h"
#include "defs.h"

class ImageData {

 public:

  ImageData(std::vector<ImageAbs*>* images, const int w, const int h);
  ~ImageData();

  void computeGradMag(ushort l, GradMag* grad);
  void allocateComputeAllGradMags();
  void allocateComputeGradMag(ushort l);
  
  void computeCGrad(ushort l, CGrad* grad);
  void computeCGrad(const ushort l, const int i, const int j, CGrad& here);
  void allocateComputeAllCGrads();
  void allocateComputeCGrad(ushort l);

  float vertGradMag(ushort l, const Coord& loc);
  float vertGradMagLookup(ushort l, const Coord& loc);
  float horizGradMag(ushort l, const Coord& loc);
  float horizGradMagLookup(ushort l, const Coord& loc);

  void vertCGrad(ushort l, const Coord& loc, float y[3]);
  void horizCGrad(ushort l, const Coord& loc, float x[3]);
  const CGrad& cgradLookup(ushort l, const Coord& loc) const;

  void addImage(ImageAbs* im);
  void removeLastImage();

  ImageAbs* images(const int i) { return _images[i]; }
  const ImageAbs* images(const int i) const { return _images[i]; }
  GradMag* gradmags(const int i) { return _gradmags[i]; }
  const GradMag* gradmags(const int i) const { return _gradmags[i]; }
  CGrad* cgrads(const int i) { return _cgrads[i]; }
  const CGrad* cgrads(const int i) const {return _cgrads[i]; }

  unsigned char* _imptr(short l, Coord p) {
    return _images[l]->transformedPixel(p);
  }

  const unsigned char* _imptr(short l, Coord p) const {
    return _images[l]->transformedPixel(p);
  }

  float getLum(const Coord& p, const ushort d) const;

  int w() const { return _w; }
  int h() const { return _h; }
  int size() const { return _n; }

 private:
  
  std::vector<ImageAbs*> _images;
  std::vector<GradMag*> _gradmags;
  std::vector<CGrad*> _cgrads;

  int _n,_w,_h,_wh;  
};


#endif
