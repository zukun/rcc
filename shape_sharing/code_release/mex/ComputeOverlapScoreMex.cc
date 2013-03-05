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
// bw1_area(1xn vector)
// bw2_inds (1xn cell, each cell: 1d vector)
// img_size (1x2 vector)
// lhs (output): 
// overlap_score (nx1 vector)
{
 	if ( nrhs != 5 ) { 
		mexPrintf("we need 5 inputs\n");
		return;
	}
	if ( nlhs == 0) {
		return;
	}	
	if ( nlhs > 1) {
		mexPrintf("1 output is only allowed\n");
	}

	// parameters
	size_t n_region = mxGetNumberOfElements(prhs[0]);
	double* im_dim = mxGetPr(prhs[4]);
	size_t im_h = im_dim[0];
	size_t im_w = im_dim[1];

	// allocate output
	plhs[0] = mxCreateDoubleMatrix(n_region, 1, mxREAL);
	double* overlap_score = mxGetPr(plhs[0]);

	// overlap
	const double* bw1_area = mxGetPr(prhs[2]);
	for (size_t i=0 ; i < n_region; i++) {


		// bw2
		const mxArray *mx_bw2_inds = mxGetCell(prhs[3],i);
		unsigned int *bw2_inds = (unsigned int*)mxGetData(mx_bw2_inds);
		size_t n_bw2 = mxGetNumberOfElements(mx_bw2_inds);

		if ( n_bw2 ) {
			vector<unsigned char> bw2(im_h*im_w,0);

			for (size_t j=0 ; j < n_bw2; j++) {
				unsigned int idx = bw2_inds[j] -1; // zero-based idx
				bw2[idx] = 1;
			}		

			// integral
			vector<double> integral_bw2(im_h*im_w, 0.0);
			for (size_t x=0; x < im_w ; x++) {
				size_t stride = x*im_h;
				integral_bw2[stride] = bw2[stride]; 
				for (size_t y =1; y < im_h ; y++) {
					size_t idx = stride + y;
					integral_bw2[idx] = integral_bw2[idx-1] + bw2[idx];
				}
			}
			bw2.clear();

			// overlap
			const mxArray *mx_pgy = mxGetCell(prhs[0],i);
			const mxArray *mx_ngy = mxGetCell(prhs[1],i);
			unsigned int *pgy = (unsigned int*)mxGetData(mx_pgy);
			size_t n_pgy = mxGetNumberOfElements(mx_pgy);
			unsigned int *ngy = (unsigned int*)mxGetData(mx_ngy);
			size_t n_ngy = mxGetNumberOfElements(mx_ngy);

			// pgy
			double sum_pgy(0.0);
			for ( size_t j=0 ; j < n_pgy ; j++) {
				double pgy_val = integral_bw2[pgy[j]-1] == 0 ? 0.0 : integral_bw2[pgy[j]-1] -1.0;
				sum_pgy += pgy_val; // zero-based
			}
			// ngy
			double sum_ngy(0.0);
			for ( size_t j=0 ; j < n_ngy ; j++) {
				sum_ngy += integral_bw2[ngy[j]-1]; // zero-based
			}
			
			double overlap = (sum_ngy - sum_pgy);
			double a1 = bw1_area[i];
			double a2 = n_bw2;
			overlap_score[i] = overlap/(a1+a2-overlap+1e-12);
			integral_bw2.clear();
		}
	}
}