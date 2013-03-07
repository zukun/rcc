#ifndef INCREDIBLECUT_H
#define INCREDIBLECUT_H

#include "portraitcut.h"


class IncredibleCut : public PortraitCut {

 public:

  IncredibleCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result);

  ~IncredibleCut();

  

  float BVZ_data_penalty(Coord p, ushort d);
  float BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl);

 protected:

  void setupTolerances();
  float *_low, *_high;
};

#endif 
