#ifndef DISTANCEMAP_H
#define DISTANCEMAP_H

#include <assert.h>
#include <stdlib.h>
#include <vector>
#include "defs.h"


class DistanceMap {

 public:

  DistanceMap(int w, int h, const std::vector<RowSpan> spans);
  ~DistanceMap();
  
  float dist(const int x, const int y) {
    assert(x>=0 && x<_w && y>=0 && y<_h);
    return _data[y*_w + x];
  }

 private:

  void propagate(const int loc, const int prop_loc, const int dx, const int dy);
  void SSED();
  void setDist(const int x, const int y, const float val);

  int _w, _h, _wh;
  float *_data, *_xDist, *_yDist;
};


#endif
