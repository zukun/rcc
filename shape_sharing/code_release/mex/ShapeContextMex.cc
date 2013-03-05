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

unsigned int histBinning(double val, double *bin, unsigned int n_bin)
{
	unsigned int bin_idx(0);
	for ( int i=0 ; i < n_bin ; i++ ) {
		if ( val >= bin[i] && val < bin[i+1] ) {
			bin_idx = i;
			break;
		}
	}
	return bin_idx;
}

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// canny edge image (binary)
// bbox : 4 x n_bbox
// params : angle_bin (1d vector), length_bin (1d vector)
// lhs (output):
// shape_context (2D matrix)
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
	if ( !mxIsLogical(prhs[0]) ) {
		mexPrintf("First input must be logical\n");
		return;
	}

	// parameters
	mxArray *tmp;
	tmp = mxGetField(prhs[2], 0, "angle_bin");
	double* angle_bin = mxGetPr(tmp);
	int n_angle_bin = mxGetNumberOfElements(tmp) - 1;	
	tmp = mxGetField(prhs[2], 0, "length_bin");
	double* length_bin = mxGetPr(tmp);
	int n_length_bin = mxGetNumberOfElements(tmp) - 1;

	int n_bbox = mxGetN(prhs[1]);
	int feat_dim = n_angle_bin*n_length_bin;
	const double PI = 2*acos(0.0);
	
	int im_h = mxGetM(prhs[0]);
	int im_w = mxGetN(prhs[0]);
	int im_dim[2] = {im_h, im_w};

	// allocate output
	plhs[0] = mxCreateNumericMatrix(feat_dim, n_bbox, mxSINGLE_CLASS, mxREAL);
	float* shape_context = (float*)mxGetData(plhs[0]);

	mxLogical* canny = mxGetLogicals(prhs[0]);
	double* bbox = mxGetPr(prhs[1]);
	for ( int i=0 ; i < n_bbox ; i++) {
		double lx = bbox[4*i];
		double rx = bbox[4*i+1];
		double ty = bbox[4*i+2];
		double dy = bbox[4*i+3];
		double w = rx-lx+1;
		double h = dy-ty+1;
		double cx = (bbox[4*i] + bbox[4*i+1])/2;
		double cy = (bbox[4*i+2] + bbox[4*i+3])/2;
		double diag = sqrt(w/2 + h/2);

		float* shape_context_i = shape_context + i*feat_dim;

		for ( int x = lx ; x <= rx ; x++) {
			for  ( int y = ty ; y <= dy ; y++) {
				int idx = y + x*im_h;
				if ( canny[idx] ) {
					double dx = x - cx;
					double dy = y - cy;
					double dist = sqrt(dx*dx + dy*dy)/diag;
					double angle = atan2(dy,dx);
					if ( angle < 0 ) angle += 2*PI;

					unsigned int angle_idx =
						histBinning(angle, angle_bin, n_angle_bin);
					unsigned int length_idx = 
						histBinning(dist, length_bin, n_length_bin);

					unsigned int bin_idx = n_length_bin*angle_idx + length_idx;
					shape_context_i[bin_idx]++;
				}
			}
		}
	}
}



