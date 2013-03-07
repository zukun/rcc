#include "steppableLinearSolver.h"


SteppableLinearSolver::SteppableLinearSolver(int n_, implicitMatrix *A_, const HB_Precond* P_, Areal x_[], Areal b_[], 
					     Areal epsilon_) :
  n(n_), A(A_), P(P_), x(x_), b(b_), epsilon(epsilon_) {

  r = (Areal *) malloc(sizeof(Areal) * n);
  d = (Areal *) malloc(sizeof(Areal) * n);
  t = (Areal *) malloc(sizeof(Areal) * n);
  temp = (Areal *) malloc(sizeof(Areal) * n);
  
  //vecAssign(n, x, b);
  printf("starting conjugate gradient with %d variables\n",n);
  
  vecAssign(n, r, b);
  A->matVecMult(x, temp);
  vecDiffEqual(n, r, temp);
  
  rSqrLen = vecSqrLen(n, r);
  printf("initial error %f\n",rSqrLen);
  vecAssign(n, d, r);
  
  i = 0;
  
  if (rSqrLen > epsilon) 
    _done = false;
  else 
    _done = true;
  
  
}

SteppableLinearSolver::~SteppableLinearSolver() {
  free(r);
  free(d);
  free(t);
  free(temp);
}


void SteppableLinearSolver::takeOneStep() {
  if (_done) return;

  ++i;
  A->matVecMult(d, t);
  u = vecDot(n, d, t);
      
  if (u == 0) {
    printf("(SolveConjGrad) d'Ad = 0\n");
    _done = true;
    return;
  }
      
  // How far should we go?
  alpha = rSqrLen / u;
  //printf("here %f %f\n",rSqrLen, u);
      
  // Take a step along direction d
  vecAssign(n, temp, d);
  vecTimesScalar(n, temp, alpha);
  vecAddEqual(n, x, temp);
  //printf("next length %f\n",vecSqrLen(n,x));
      
  if (i & 0x3F) {
    vecAssign(n, temp, t);
    vecTimesScalar(n, temp, alpha);
    vecDiffEqual(n, r, temp);
  } else {
    // For stability, correct r every 64th iteration
    vecAssign(n, r, b);
    A->matVecMult(x, temp);
    vecDiffEqual(n, r, temp);
  }

  rSqrLenOld = rSqrLen;
  rSqrLen = vecSqrLen(n,r);

  // Converged! Let's get out of here
  if (rSqrLen <= epsilon) {
    _done = true;
    return;
  }
  else {
    //printf("Iteration %d, error %f\n",i, rSqrLen);      
    //fflush(fp);
  }

  vecAssign(n,temp,r);
  P->precondVec(temp);      
  rSqrLen = vecDot(n,temp,r);

  // Change direction: d = r + beta * d
  beta = rSqrLen/rSqrLenOld;
  vecTimesScalar(n, d, beta);
  vecAddEqual(n, d, temp);
}

