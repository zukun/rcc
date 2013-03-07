#include "brightCut.h"

BrightCut::BrightCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result) :
  PortraitCut(oWidth, oHeight, images, result, stdout) {
  memset(_labels, 0, _w*_h*sizeof(ushort));
  memset(_required, (short)-1, _w*_h*sizeof(short));
}

float BrightCut::BVZ_data_penalty(Coord p, ushort d) {
  unsigned char* I = _imptr(d,p);
  float lum = .3086f * (float)I[0] + .6094f * (float)I[1] + .082f * (float)I[2];
  return .01f * (255.f -lum);
}

float BrightCut::BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl) {
  if (l==nl) return 0;
  return (_potts +  _regular_inter * (PortraitCut::BVZ_interaction_penalty(p,np,l,nl))); 
}

