// matlab include
#include "mex.h"
#include "matrix.h"

#include <stdlib.h>
#include <functional>
#include <numeric>
#include <algorithm>
#include <vector>
#include <math.h>


using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// pgy_inds (1xn cell, each cell: 1d vector)
// ngy_inds (1xn cell, each cell: 1d vector)
// integral_y (1xr cell, each cell: 2D matrix)
// region_size(1xr vector)
// lhs (output): 
// overlap (rxn matrix)
{
 	if ( nrhs != 4 ) { 
		mexPrintf("we need 4 inputs\n");
		return;
	}
	if ( nlhs == 0) {
		return;
	}	
	if ( nlhs > 1) {
		mexPrintf("1 output is only allowed\n");
	}

	// parameters
	size_t n_proj = mxGetNumberOfElements(prhs[0]);
	size_t n_region = mxGetNumberOfElements(prhs[2]);

	// allocate output
	plhs[0] = mxCreateDoubleMatrix(n_region, n_proj, mxREAL);
	double* overlap = mxGetPr(plhs[0]);

	// overlap
	const double* region_size = mxGetPr(prhs[3]);

	for (size_t i=0 ; i < n_proj; i++) {
		const mxArray *mx_pgy = mxGetCell(prhs[0],i);
		const mxArray *mx_ngy = mxGetCell(prhs[1],i);

        size_t n_pgy = mxGetNumberOfElements(mx_pgy);
        size_t n_ngy = mxGetNumberOfElements(mx_ngy);
        unsigned int *pgy = (unsigned int*)mxGetData(mx_pgy);
        unsigned int *ngy = (unsigned int*)mxGetData(mx_ngy);
        size_t stride = n_region*i;

        // each region
        for ( size_t r=0 ; r < n_region ; r++) {
            const mxArray *mx_integral = mxGetCell(prhs[2],r);
            double* integral = mxGetPr(mx_integral);

            // pgy
            double sum_pgy(0.0);
            for ( size_t j=0 ; j < n_pgy ; j++) {
                double pgy_val = integral[pgy[j]-1];
                sum_pgy += pgy_val; // zero-based
            }
            // ngy
            double sum_ngy(0.0);
            for ( size_t j=0 ; j < n_ngy ; j++) {
                sum_ngy += integral[ngy[j]-1]; // zero-based
            }
            overlap[stride+r] = (sum_ngy - sum_pgy)/region_size[r];
        }
    }
}