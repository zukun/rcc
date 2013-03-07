#ifndef BRIGHTCUT_H
#define BRIGHTCUT_H

#include "portraitcut.h"

//#pragma unmanaged

class BrightCut : public PortraitCut {

 public:

  BrightCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result);

  float BVZ_data_penalty(Coord p, ushort d);
  float BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl);
  float getDataCost (const struct StrokeData* s, Coord p, ushort d) { return BVZ_data_penalty (p, d); }

  ~BrightCut() {}

  // weights
//  float _potts, _regular_inter;

 protected:


};

//#pragma managed

#endif
