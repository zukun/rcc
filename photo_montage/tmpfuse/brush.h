#ifndef BRUSH_H
#define BRUSH_H

#include "jl_vectors.h"
#include "coord.h"
#include <vector>
#include <wx/glcanvas.h>
#include <GL/glu.h>
#include "glerror.h"


#define B_RADIUS 5

class Brush {

 public:

  
  Brush(const bool isEraser, const int x, const int y);
  ~Brush();

  void move(const int x, const int y);

  void render();
  void renderRecent();

  void grabStencil(int sw, int sh, int hoffset);
  const short* getStencil() const { return _stencil; }

  void orStencil(const short* s);
  void setColor(Vec3f col) { _color = col; }
 private:
  //Coord _loc;
  std::vector<Coord> _locs;
  GLUquadric* _quadric;

  short* _stencil; // 1 in the brush, not 1 elsewhere
  int _sw, _sh;
  //StrokeType _type;
  bool _isEraser;
  Vec3f _color;
};



#endif
