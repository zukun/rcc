#include "distanceMap.h"
#include <float.h>

DistanceMap::DistanceMap(int w, int h, const std::vector<RowSpan> spans) : _w(w), _h(h) {
  _wh = _w*_h;
  _data = new float[_wh];
  _xDist = new float[_wh];
  _yDist = new float[_wh];

  int i, x, y;
  for (i=0; i<_wh; i++) {
    _data[i] = FLT_MAX;
    _xDist[i] = FLT_MAX;
    _yDist[i] = FLT_MAX;
  }

  // initialize stroke points
  for (unsigned int j=0; j<spans.size(); ++j) {
    x = spans[j]._x; y = spans[j]._y;
    for (int s=0; s<spans[j]._num; ++s) {
      int index = y*_w + x + s;
      _data[index] = _xDist[index] = _yDist[index] = 0;
    }
    
    //setDist(x+s,y, 0);
  }

  SSED();
  delete[] _xDist; _xDist = NULL;
  delete[] _yDist; _yDist = NULL;

}

void DistanceMap::setDist(const int x, const int y, const float val) {
  assert(x>=0 && x<_w && y>=0 && y<_h);
  _data[y*_w+x] = val;
}

DistanceMap::~DistanceMap() {
  if (_xDist) delete[] _xDist;
  if (_yDist) delete[] _yDist;
  if (_data) delete[] _data;
}


void DistanceMap::propagate(const int loc, const int prop_loc, const int dx, const int dy) {
  assert(loc>-1);
  assert(prop_loc>-1);
  float tmpx = _xDist[prop_loc] + dx;
  float tmpy = _yDist[prop_loc] + dy;
  float  newVal = tmpx*tmpx + tmpy*tmpy;
  if (newVal < _data[loc] || (newVal==_data[loc] && fabs(tmpx)>fabs(_xDist[loc]))) {
    _xDist[loc] = tmpx;
    _yDist[loc] = tmpy;
    _data[loc] = newVal;
  }
}

void DistanceMap::SSED() {
  int scanline, point, x, y;
  
  // first picture scan, forward
  int prev_scanline = 0, prev_point;
  int w_limit = _w-1;
  for (y=1; y<_h; y++) {
    scanline = y*_w;
    
    for (x=0; x<_w; x++) {
      point = scanline+x;
      prev_point = prev_scanline+x;      

      if (x!=0) {
	propagate(point,prev_point-1,1,1);
	propagate(point,point-1,1,0);	
      }
      propagate(point,prev_point,0,1);	
      if (x != w_limit) 
	propagate(point,prev_point+1,1,1);
      
    } // end loop over scanline

    // then backwards propagate
    for (x=_w-2; x>-1; x--) {
      point = scanline+x;
      propagate(point,point+1,1,0);	
    }

    prev_scanline = scanline;
  } // end loop over scanlines
  
  // second picture scan, backwards
  int next_scanline, next_point;
  next_scanline =(_h-1)*_w;
  for (y=_h-2; y>-1; y--) {
    scanline = y * _w;
    
    for (x=w_limit; x>-1; x--) {
      point = scanline+x;
      next_point = next_scanline+x;

      if (x!=w_limit) {
	propagate(point,next_point+1,1,1);
	propagate(point,point+1,1,0);
      }
      propagate(point,next_point,0,1);
      if (x!=0)
	propagate(point,next_point-1,1,1);
      
    } // end loop over scanline

    // then forward propagate
    for (x=1; x<_w; x++) {
      point = scanline+x;
      propagate(point,point-1,1,0);
    }
    
    next_scanline = scanline;
  } // end loop over scanlines
  
  // take square roots
  for (int j=0; j<_h; j++) 
    for (int i=0; i<_w; i++)
      _data[j*_w+i] = sqrt(_data[j*_w+i]);

  /*
  FILE* fp = fopen("shit.dat", "w");
  for (y=0; y<_h; ++y) {
    for (x=0; x<_w; ++x)
      fprintf(fp, "%.5f ", dist(x,y));
    fprintf(fp,"\n");
  }
  fclose(fp);
   
  std::exit(0);*/
}
