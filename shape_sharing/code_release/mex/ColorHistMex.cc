// matlab include
#include "mex.h"
#include "matrix.h"

#include "DT.h"

#include <stdlib.h>
#include <cmath>

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// input image (Lab)
// bbox (4 x n_bbox) -- zero-indexed
// lhs (output):
// color_hist (2D matrix)
{
 	if ( nlhs == 0 ) {
		return;
	}
	if ( nlhs > 1 ) {
		mexPrintf("too many output\n");
		return;
	}
	
	if ( nrhs != 3 ) { 
		mexPrintf("we need 3 inputs\n");
		return;
	}

	// parameters
	double min_L = 0.0;
	double max_L = 100.0;
	double min_a = -128.0;
	double max_a = 128.0;
	double min_b = -128.0;
	double max_b = 128.0;
	int n_color_bin = 22;
	int hist_dim = n_color_bin*3;
	double L_interval = (max_L-min_L)/n_color_bin + 1e-6;
	double A_interval = (max_a-min_a)/n_color_bin + 1e-6;
	double B_interval = (max_b-min_a)/n_color_bin + 1e-6;

	int im_h = mxGetM(prhs[0]);
	int im_w = mxGetN(prhs[0]);
	int im_dim[2] = {im_h, im_w};
	int n_bbox = mxGetN(prhs[1]);
	
	// output
	plhs[0] = mxCreateNumericMatrix(hist_dim, n_bbox, mxSINGLE_CLASS, mxREAL);
	float* color_hist = (float*)mxGetData(plhs[0]);
	
	// hist
	double* Lab = mxGetPr(prhs[0]);
	double* bbox = mxGetPr(prhs[1]); 
	for ( int i=0 ; i < n_bbox; i++) {

		float* color_hist_i = color_hist + i*hist_dim;
		int lx = (int)bbox[4*i];
		int rx = (int)bbox[4*i+1];
		int ty = (int)bbox[4*i+2];
		int dy = (int)bbox[4*i+3];

		for ( int x=lx ; x <= rx ; x++) {
			for ( int y=ty ; y <= dy; y++) {
				int idx = y + x*im_h;
				double L = Lab[idx];
				double A = Lab[idx + im_h*im_w] + min_a;
				double B = Lab[idx + 2*im_h*im_w] + min_b;
				int L_idx = (int)(L/L_interval);
				int A_idx = (int)(A/A_interval);
				int B_idx = (int)(B/B_interval);
				
				color_hist_i[L_idx]++;
				color_hist_i[A_idx + n_color_bin]++;
				color_hist_i[B_idx + 2*n_color_bin]++;
			}
		}
	}	

}



