/*=================================================================
* createSparseAdjMatrix.c
*=================================================================*/

#include <math.h>
#include "mex.h"

#include <stdlib.h>
#include <vector>

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// inds (1xn vector)
// vals (1xn vector) 
// n_row (a scalar)
// n_col (a scalar)
// lhs (output):
// sparse_mat (n_row x n_col sparse matrix)
{
        
    /* Check for proper number of input and output arguments */    
    if (nrhs != 4) {
		mexErrMsgTxt("Four input argument required.");
    } 
    if(nlhs > 1){
		mexErrMsgTxt("Too many output arguments.");
    }
    
	// paramemters
	size_t nz = mxGetNumberOfElements(prhs[0]);
	size_t nr = mxGetScalar(prhs[2]);
	size_t nc = mxGetScalar(prhs[3]);
     
	// output
	plhs[0] = mxCreateSparse(nr, nc, nz, mxREAL);
	
	// set sparse matrix
	double* mat = mxGetPr(plhs[0]);
	mwIndex* ir = mxGetIr(plhs[0]);
	mwIndex* jc = mxGetJc(plhs[0]);
	double* inds = mxGetPr(prhs[0]);
	double* vals = mxGetPr(prhs[1]);
	vector<size_t> nz_in_col(nc,0);
	
	for ( size_t i=0; i < nz ; i++ ) {
		size_t idx = inds[i];
		size_t r_idx = idx%nr;
		size_t c_idx = idx/nr;
		mat[i] = vals[i];
		ir[i] = r_idx;
		nz_in_col[c_idx]++;		
	}
	jc[0] = 0;
	for ( size_t j=1; j <= nc; j++) {
		jc[j] = jc[j-1] + nz_in_col[j-1];		
	}    
  
}
 
