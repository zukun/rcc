#ifndef _DEFS_H_
#define _DEFS_H_

#include "../jl_vectors.h"

typedef unsigned short ushort;

#define A_INFINITY 100000.		/* infinite capacity */

// MAXDIFF is from a specific image, DIFF is from a color
enum StrokeType {S_HIGHLIGHT, S_SHADOW, S_COLOR, S_NUM, S_MAXHIST, S_MINHIST, S_MAXCONTRAST, 
		 S_MINCONTRAST, S_MAXDIFF, S_TIMELAPSE, S_PANO, S_NONE, S_ERASE, S_DIFF};

static int _numValidStrokeTypes = 11;

static inline bool validStrokeType(const int i) {
  if (i<_numValidStrokeTypes) return true;
  return false;
}

struct Ranker {
  Ranker(const int which, const float score) : _which(which), _score(score) {}
  bool operator< (const Ranker& other) const { return _score < other._score; }
  short _which;
  float _score;
};

struct RowSpan {
  RowSpan(int x, int y, int num) : _x(x), _y(y), _num(num) {}
  int _x, _y, _num; // span from _x,_y to _x+_num-1,_y
};



struct GradMag {
  double _x, _y; // L2 norm of all color channels
};

struct CGrad {
  float _x[3], _y[3]; // 3 are r,g,b
  float normFrom(const CGrad& o) const;  // square-rooted
  float val(int c) const;
  void setVal(int c, const float d);
};

struct StrokeData {
  StrokeData() { _st=S_NONE;}
  StrokeType _st;
  int _num; // if S_NUM
  Vec3f _dialColor; // if S_COLOR
  CGrad _cgrad; // if S_INTRINSIC
  Vec3i _medianColor; // if S_INTRINSIC
  Vec3i _avoidColor; // if S_NEG
};

#endif
