#ifndef BESTFITAFFINE_H
#define BESTFITAFFINE_H

#include <vector>
#include "coord.h"

typedef std::vector <std::pair <Coord,Coord> > CorrVec;

class BestFitAffine {


 public:

  BestFitAffine(const CorrVec& cv, float offx, float offy);
  ~BestFitAffine();

  const float* getTransform() const;

  bool successful() const { return _success; };

 private:

  int solvru(double *a,double *b,int n);
  double qrlsq(double *a,double *b,int m,int n,int *f);

  float* _M;
  bool _success;
};



#endif
