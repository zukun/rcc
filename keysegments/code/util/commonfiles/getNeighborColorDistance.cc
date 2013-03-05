/*=================================================================
* getNeighborColorDistance.cc
*=================================================================*/

#include <math.h>
#include "mex.h"


double squareDist( double *data_, double *nextData_, size_t ndx1, size_t ndx2, size_t N )
{
    double val = 0.0;
    for (size_t i=0; i<3; i++) {
	    val += (data_[ndx1+i*N] - nextData_[ndx2+i*N]) * (data_[ndx1+i*N] - nextData_[ndx2+i*N]);
    }
    return val;
}

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input):
// opticalflow_map (m x n matrix) 
// Data (mxn x 3 matrix)
// nextData (mxn x 3 matrix)
// lhs (output):
// colordist (m x n matrix)
{
        
    /* Check for proper number of input and output arguments */    
    if (nrhs != 3) {
		mexErrMsgTxt("Three input arguments required.");
    } 
    if (nlhs > 1){
		mexErrMsgTxt("Too many output arguments.");
    }
    
	// parameters
    mwSize m = mxGetM(prhs[0]);
    mwSize n = mxGetN(prhs[0]);	
    size_t N = m*n;

	// output
    plhs[0] = mxCreateDoubleMatrix(m, n, mxREAL);
	
	// set output and input pointers
	double* mat = mxGetPr(plhs[0]);
    double* opticalFlowMap = mxGetPr(prhs[0]);
	double* data = mxGetPr(prhs[1]);
	double* nextData = mxGetPr(prhs[2]);

    size_t count = 0;
    for (size_t j=0; j<n; j++) {
        for (size_t i=0; i<m; i++) {        
            mat[count] = squareDist(data, nextData, i+m*j, opticalFlowMap[i+m*j]-1, N);           
            count++;
        }
	}   
  
}
 
