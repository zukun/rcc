#include "linearSolver.h"

using namespace std;

Areal ConjGrad(int n, implicitMatrix *A, Areal x[], Areal b[], 
		Areal epsilon,	// how low should we go?
		int    *steps, FILE *fp)
{
  int		i, iMax;
  Areal	alpha, beta, rSqrLen, rSqrLenOld, u;

  Areal *r = (Areal *) malloc(sizeof(Areal) * n);
  Areal *d = (Areal *) malloc(sizeof(Areal) * n);
  Areal *t = (Areal *) malloc(sizeof(Areal) * n);
  Areal *temp = (Areal *) malloc(sizeof(Areal) * n);

  //vecAssign(n, x, b);
  fprintf(fp,"starting conjugate gradient with %d variables\n",n);

  vecAssign(n, r, b);
  A->matVecMult(x, temp);
  vecDiffEqual(n, r, temp);

  // constrain r here, if wished
  //r[0] = 0; 

  rSqrLen = vecSqrLen(n, r);
  printf("initial error %f\n",rSqrLen);
  vecAssign(n, d, r);

  i = 0;
  if (*steps)
    iMax = *steps;
  else
    iMax = MAX_STEPS;
		
  if (rSqrLen > epsilon)
    while (i < iMax) {	
      ++i;
      A->matVecMult(d, t);
      u = vecDot(n, d, t);
      
      if (u == 0) {
	printf("(SolveConjGrad) d'Ad = 0\n");
	break;
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
      
      // constrain r here, if wished
      //r[0] = 0; 

      rSqrLenOld = rSqrLen;
      rSqrLen = vecSqrLen(n, r);
      
      // Converged! Let's get out of here
      if (rSqrLen <= epsilon)
	break;		
      else {
	fprintf(fp,"Iteration %d, error %f\n",i, rSqrLen);
	fflush(fp);
      }
      
      // Change direction: d = r + beta * d
      beta = rSqrLen/rSqrLenOld;
      vecTimesScalar(n, d, beta);
      vecAddEqual(n, d, r);
    }
  
  // free memory

  free(r);
  free(d);
  free(t);
  free(temp);
		
  *steps = i;
  return(rSqrLen);
}


//---------------------------


Areal PrecConjGrad(int n, implicitMatrix *A, const HB_Precond* P, Areal x[], Areal b[], 
		Areal epsilon,	// how low should we go?
		int    *steps, FILE *fp)
{
  int		i, iMax;
  Areal	alpha, beta, rSqrLen, rSqrLenOld, u;

  Areal *r = (Areal *) malloc(sizeof(Areal) * n);
  Areal *d = (Areal *) malloc(sizeof(Areal) * n);
  Areal *t = (Areal *) malloc(sizeof(Areal) * n);
  Areal *temp = (Areal *) malloc(sizeof(Areal) * n);

  //vecAssign(n, x, b);
  fprintf(fp,"starting conjugate gradient with %d variables\n",n);

  vecAssign(n, r, b);
  A->matVecMult(x, temp);
  vecDiffEqual(n, r, temp);
  printf("initial error %f\n",vecSqrLen(n,r));

  vecAssign(n, d, r);
  P->precondVec(d);
  rSqrLen = vecDot(n,r,d);


  i = 0;
  if (*steps)
    iMax = *steps;
  else
    iMax = MAX_STEPS;
		
  if (rSqrLen > epsilon)
    while (i < iMax) {	
      ++i;
      A->matVecMult(d, t);
      u = vecDot(n, d, t);
      
      if (u == 0) {
	printf("(SolveConjGrad) d'Ad = 0\n");
	break;
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
      
      // constrain r here, if wished
      //r[0] = 0; 

      rSqrLenOld = rSqrLen;
      rSqrLen = vecSqrLen(n,r);

      // Converged! Let's get out of here
      if (rSqrLen <= epsilon)
	break;		
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
  
  // free memory

  free(r);
  free(d);
  free(t);
  free(temp);
		
  *steps = i;
  return(rSqrLen);
}

/*

  // Tester function for implicitMatrixWithTrans. Takes an implicit
  // matrix of m rows and n columns (transpose is n*m), and makes sure
  // that it is really equal to its transpose.
  void testImplicitMatrixWithTrans(implicitMatrixWithTrans *mat, int m, int n) {
  const Areal ERROR_BOUND = 0.000001;

  cout << "Testing " << mat << " of size (" << m << ", " << n << ")" << endl;

  // these are the explicit representations of the matrices we will store
  Areal explicitMat1[m][n];
  Areal explicitMat2[n][m];
  
  // these are the basis vectors we will use to reconstruct
  Areal vec1[n];
  Areal vec2[m];
  cout << "Allocated the explicit arrays." << endl;

  // construct the explicit matrix by multiplying with basis vecors
  cout << "Constructing explicit matrix...";
  for (int j = 0 ; j < n ; j++) {
  cout << "[" << j << "]";

  // make vec1 into basis vector
  for (int elt = 0 ; elt < n ; elt++) {
  if (j == elt)
  vec1[elt] = 1.0;
  else
  vec1[elt] = 0.0;
  }

  // get the result
  mat->matVecMult(vec1, vec2);

  // load into matrix
  for (int i = 0 ; i < m ; i++)
  explicitMat1[i][j] = vec2[i];
  }
  cout << endl;

  // now do the same, but with the transpose
  cout << "Constructing explicit transpose matrix...";
  for (int j = 0 ; j < m ; j++) {
  cout << "[" << j << "]";

  // make vec2 into basis vector
  for (int elt = 0 ; elt < m ; elt++) {
  if (j == elt)
  vec2[elt] = 1.0;
  else
  vec2[elt] = 0.0;
  }

  // get the result
  mat->matTransVecMult(vec2, vec1);

  // load into matrix
  for (int i = 0 ; i < n ; i++)
  explicitMat2[i][j] = vec1[i];
  }
  cout << endl;

  // print out the matrix
  for (int i = 0 ; i < m ; i++) {
  for (int j = 0 ; j < n ; j++) {
  cout << explicitMat1[i][j] << " ";
  }
  cout << endl;
  }
  cout << endl;
  // print out the matrix
  for (int i = 0 ; i < n ; i++) {
  for (int j = 0 ; j < m ; j++) {
  cout << explicitMat2[i][j] << " ";
  }
  cout << endl;
  }

  // now check that they really are the transpose of each other!
  for (int i = 0 ; i < m ; i++) {
  for (int j = 0 ; j < n ; j++) {
  if (fabs(explicitMat1[i][j] - explicitMat2[j][i]) > ERROR_BOUND) {
  cout << endl << "FAILURE: ";
  cout << explicitMat1[i][j] << " == ";
  cout << "m[" << i << "][" << j << "] != ";
  cout << "trans(m)[" << j << "][" << i << "] == ";
  cout << explicitMat2[j][i] << endl;
  return;
  }
  }
  }
  cout << "SUCCESS: The matrix is equal to its transpose!" << endl;
  }
*/
