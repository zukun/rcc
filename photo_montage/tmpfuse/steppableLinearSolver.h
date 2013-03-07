#ifndef STEPPABLELINEARSOLVER_H
#define STEPPABLELINEARSOLVER_H

#include "linearSolver.h"

class SteppableLinearSolver {

 public:

  SteppableLinearSolver(int n_, implicitMatrix *A_, const HB_Precond* P_, Areal x_[], Areal b_[], 
			Areal epsilon_);

  ~SteppableLinearSolver();

  void takeOneStep();

  bool done() const { return _done; }

  int numSteps() const { return i;}

  Areal error() const { return rSqrLen; }

 private:
  int n;
  implicitMatrix *A;
  const HB_Precond *P;
  Areal *x, *b;
  Areal epsilon;


  int i, iMax;
  Areal	alpha, beta, rSqrLen, rSqrLenOld, u;
  Areal *r, *d, *t, *temp;

  bool _done;
};

#endif
