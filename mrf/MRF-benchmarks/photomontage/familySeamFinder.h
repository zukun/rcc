#ifndef FAMILYSEAMFINDER_H
#define FAMILYSEAMFINDER_H

#include "seamFinder.h"

MRF::CostVal edgySmoothCostFn(int pix1, int pix2, int dp,int dq);

class FamilySeamFinder : public SeamFinder {

 public:

  FamilySeamFinder(std::vector<Image*> ims, const Image* dataCostImage);
  ~FamilySeamFinder() { delete _dataConstraints; }

  //void runStudy();

 private:

  captype data_penalty(const Coord& p, const int d);
  captype smoothness_penalty(const Coord& p, const Coord& q, const int d, const int dq);
  void initializeResult();
  EnergyFunction* getEnergyFunction(MRF::CostVal* dataCostArray);

  int* _dataConstraints;

};


#endif
