#ifndef INTRINSICCUT_H
#define INTRINSICCUT_H

#include "portraitcut.h"

//typedef int[3]

class IntrinsicCut : public PortraitCut {

 public:

  IntrinsicCut(int oWidth, int oHeight, vector<ImageAbs*>* images, ushort* result);
  ~IntrinsicCut();

  float BVZ_data_penalty(Coord p, ushort d);
  float BVZ_interaction_penalty(Coord p, Coord np, ushort l, ushort nl);


 protected:

  void computeMedians();


  CGrad* _medians;
  Vec3i* _medianColors;
};


#endif
