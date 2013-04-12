#ifndef IMAGE_H
#define IMAGE_H

#include <assert.h>
#include "coord.h"
#include "pngio.h"
#define CType unsigned char
#define _NUMC_ 3
#define REF(x,y) ((y)*_w+(x))
#define REFC(x,y) (_NUMC_*((y)*_w+(x)))

// Next two lines no longer true!
// four channels R,G,B,A
// 4th channel, 0, transparent

struct GradMag {
  float _x, _y; // L2 norm of all color channels
};


class Image {
  
 public:
  //Image();
  Image(CType* data, const int w, const int h);
  Image(char* name);
  Image(const int w, const int h);
  ~Image();

  void write(char* name) { write_png(name,_w,_h,_data,_NUMC_); }

  //void allocate(CType* data, const int w, const int h);
  CType* data() { return _data; }
  int width() const { return _w;}
  int height() const { return _h;}
  
  // transparent?
  bool trans(const int x, const int y);

  float distTo(const Coord& p, const Image* other);

  void setPix(const int i, const int j, const Image* o);

  void setChPix(const CType datum, const int c, const int i, const int j);

  const CType* getPixC(const int i, const int j) const { return _data + _NUMC_*REF(i,j); }
  const CType* getPixC(const int ind) const { return _data + _NUMC_*ind; }

  CType* getPix(const int i, const int j) { return _data + _NUMC_*REF(i,j); }

  void createAuxData();
  void computeAllGradMags();
  inline float horizGradMag(const Coord& loc);
  inline float vertGradMag(const Coord& loc);

 private:
  float calcHorizGradMag(const Coord& loc);
  float calcVertGradMag(const Coord& loc);

  CType* _data;
  int _w, _h;
  GradMag* _gradmags;
};

inline float Image::horizGradMag(const Coord& loc) {
  assert(_gradmags);
  return _gradmags[REF(loc.x,loc.y)]._x;
}

inline float Image::vertGradMag(const Coord& loc) {
  assert(_gradmags);
  return _gradmags[REF(loc.x,loc.y)]._y;
}



#endif
