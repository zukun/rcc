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
// input image (h x w x c), pre-computed gradient image (c >1)
// bbox (4 x n_bbox) -- zero-indexed
// phog_L (a scalar)
// lhs (output):
// PHOG (2D matrix)
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
	
	if ( ! mxIsSingle(prhs[0]) ) {
		mexPrintf("wrong image format: must be single precision\n");
		return;
	}
	

	// parameters
	const mwSize* img_dim = mxGetDimensions(prhs[0]);
	const size_t im_h = img_dim[0];
	const size_t im_w = img_dim[1];
	const size_t n_orient = img_dim[2];
	size_t im_dim[2] = {im_h, im_w};

	const mwSize* bbox_dim = mxGetDimensions(prhs[1]);
	const size_t n_bbox = bbox_dim[1];

	const size_t phog_L = (size_t)mxGetScalar(prhs[2]);
	size_t phog_dim = n_orient*((size_t)((pow(4.0,(int)phog_L+1)-1)/3));
		
	// output
	plhs[0] = mxCreateNumericMatrix(phog_dim, n_bbox, mxSINGLE_CLASS, mxREAL);
	float* phog = (float*)mxGetData(plhs[0]);
	
	// integral image
	float* gradient = (float*)mxGetData(prhs[0]);
	float* integral = new float[im_h*im_w*n_orient];
	for ( size_t i=0 ; i < n_orient ; i++) {
		float* gradient_i = gradient + i*im_h*im_w;
		float* integral_i = integral + i*im_h*im_w;
		Integral2D(gradient_i, integral_i, im_w, im_h);
	}

	// phog
	double* bbox = mxGetPr(prhs[1]); 
	// each channel
	for (int k=0 ; k < n_orient; k++) {
		float* integral_k = integral + k*im_w*im_h;

		// each bbox
		for (int i=0 ; i < n_bbox; i++) {

			float* phog_i = phog + i*phog_dim;

			int lx = (int)bbox[4*i];
			int rx = (int)bbox[4*i+1];
			int ty = (int)bbox[4*i+2];
			int dy = (int)bbox[4*i+3];
			int w = rx - lx + 1;
			int h = dy - ty + 1;
			
			// each level
			for ( int j=0 ; j < phog_L ; j++) {

				int n_bin_1d = (int)pow(2.0,j);
				int cell_idx_offset = ((int)pow(4.0,j)-1)/3;
				int x_bin_size = (int)((double)w/(double)n_bin_1d);			
				int y_bin_size = (int)((double)h/(double)n_bin_1d);	


				// for each cell of the level
				for (int bx=0 ; bx < n_bin_1d; bx++) {
					int cell_lx = lx + bx*x_bin_size;
					int cell_rx = lx +(bx+1)*x_bin_size-1;
					for (int by=0 ; by < n_bin_1d ; by++) {
						
						int cell_ty = ty + by*y_bin_size;
						int cell_dy = ty + (by+1)*y_bin_size-1;
						
						int idx1 = cell_ty + cell_lx*im_h;
						int idx2 = cell_ty + cell_rx*im_h;
						int idx3 = cell_dy + cell_lx*im_h;
						int idx4 = cell_dy + cell_rx*im_h;
						
						int cell_idx = cell_idx_offset + by + bx*n_bin_1d;

						float val = integral_k[idx1] + integral_k[idx4] - integral_k[idx2] - integral_k[idx3];
						phog_i[cell_idx*n_orient + k] = val;
					
					}
				}
			}
		}	
	}
	delete []integral;
}



