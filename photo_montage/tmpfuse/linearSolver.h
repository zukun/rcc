#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H


#include <math.h> 
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
//#include <values.h>
#include <iostream>
//#include "HB_OneCurve.h"
#include "hb.h"
//#pragma unmanaged

// Karen's CGD

#define MAX_STEPS 100

// Matrix class the solver will accept
class implicitMatrix
{
 public:
  virtual void matVecMult(const Areal x[], Areal r[]) const = 0;
  virtual ~implicitMatrix() {}
};

// Matrix class the solver will accept
// Ax=r, so x is data
class implicitMatrixWithTrans : public implicitMatrix
{
 public:
  virtual void matVecMult(const Areal x[], Areal r[]) const = 0;
  virtual void matTransVecMult(const Areal x[], Areal r[]) const = 0;
  virtual ~implicitMatrixWithTrans() {};
};

// Solve Ax = b for a symmetric, positive definite matrix A
// A is represented implicitely by the function "matVecMult"
// which performs a matrix vector multiple Av and places result in r
// "n" is the length of the vectors x and b
// "epsilon" is the error tolerance
// "steps", as passed, is the maximum number of steps, or 0 (implying MAX_STEPS)
// Upon completion, "steps" contains the number of iterations taken
Areal ConjGrad(int n, implicitMatrix *A, Areal x[], Areal b[], 
		Areal epsilon,	// how low should we go?
		int    *steps, FILE* fp);


Areal PrecConjGrad(int n, implicitMatrix *A, const HB_Precond* P, Areal x[], Areal b[], 
		Areal epsilon,	// how low should we go?
		    int    *steps, FILE *fp);

// Tester function for implicitMatrixWithTrans. Takes an implicit
// matrix of m rows and n columns (transpose is n*m), and makes sure
// that it is really equal to its transpose.
void testImplicitMatrixWithTrans(implicitMatrixWithTrans *mat, int m, int n);

// Some vector helper functions
void vecAddEqual(const int n, Areal r[], const Areal v[]);
void vecDiffEqual(const int n, Areal r[], const Areal v[]);
void vecAssign(const int n, Areal v1[], const Areal v2[]);
void vecTimesScalar(const int n, Areal v[], const Areal s);
Areal vecDot(const int n, const Areal v1[], const Areal v2[]);
Areal vecSqrLen(const int n, const Areal v[]);
Areal vecMax(const int n, const Areal v[]);

//----------------------------------------------------------------------


inline void vecAddEqual(const int n, Areal r[], const Areal v[])
{
  for (int i = 0; i < n; ++i)
    r[i] = r[i] + v[i];
}

inline void vecDiffEqual(const int n, Areal r[], const Areal v[])
{
  for (int i = 0; i < n; ++i)
    r[i] = r[i] - v[i];
}

inline void vecAssign(const int n, Areal v1[], const Areal v2[])
{
  //for (int i = 0; i < n; ++i)
  //v1[i] = v2[i];
  memcpy(v1,v2,n*sizeof(Areal));
}

inline void vecTimesScalar(const int n, Areal v[], const Areal s)
{
  for (int i = 0; i < n; ++i)
    v[i] *= s;
}

inline Areal vecDot(int n, const Areal v1[], const Areal v2[])
{
  Areal dot = 0;
  for (int i = 0; i < n; ++i)
    dot += v1[i] * v2[i];
  return dot;
}

inline Areal vecSqrLen(const int n, const Areal v[])
{
  return vecDot(n, v, v);
}

inline Areal vecMax(const int n, const Areal v[]) {
  Areal max = v[0];
  for (int i=1; i<n; ++i)
    if (v[i] > max) max = v[i];
  return max;
}

//#pragma managed

#endif
