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
// regions (1xr cell, each cell: 1d vector --> region_inds, uint32)
// merge_mat (rxn matrix, binary)
// image_size (1x2 vector)
// lhs (output): 
// merged_regions (1xn cell, each cell: 1d vector --> merged region inds, uint32)
// merged_region_size (1xn vector)
// merged_region_center (2xn matrix)
// merged_region_bbox (4xn matrix)
{
 	if ( nrhs != 3 ) { 
		mexPrintf("we need 3 inputs\n");
		return;
	}
	if ( nlhs == 0) {
		return;
	}	
	if ( nlhs > 4) {
		mexPrintf("1,2,3 or 4 outputs are only allowed\n");
		return;
	}

	// parameters
	size_t n_region = mxGetNumberOfElements(prhs[0]);
	size_t n_proj = mxGetN(prhs[1]);
	double* im_dim = mxGetPr(prhs[2]);
	size_t im_h = im_dim[0];
	size_t im_w = im_dim[1];

	// allocate output
	mwSize dim[1];
	dim[0] = n_proj;
	plhs[0] = mxCreateCellArray(1, dim);
	plhs[1] = mxCreateDoubleMatrix(1, n_proj, mxREAL);
	plhs[2] = mxCreateDoubleMatrix(2, n_proj, mxREAL);
	plhs[3] = mxCreateDoubleMatrix(4, n_proj, mxREAL);
		
	// merge
	mxLogical* merge_mat = mxGetLogicals(prhs[1]);
	double* merged_region_size = mxGetPr(plhs[1]);
	double* merged_region_loc = mxGetPr(plhs[2]);
	double* merged_region_bbox = mxGetPr(plhs[3]);

	for (size_t i=0 ; i < n_proj ; i++) {
		vector<unsigned int> merge_inds;
		size_t stride = i*n_region;
		vector<unsigned char> bw(im_h*im_w,0);
		double accm_x(0.0), accm_y(0.0);
		double min_x(im_w-1), max_x(0.0), min_y(im_h-1), max_y(0);
		for (size_t j=0 ; j < n_region ; j++) {
			size_t idx = stride + j;
			if ( merge_mat[idx] ) {
				mxArray* mx_region_inds = mxGetCell(prhs[0],j);
				size_t n_ind = mxGetNumberOfElements(mx_region_inds);
				unsigned int* region_inds = (unsigned int*)mxGetData(mx_region_inds);
				for ( size_t k=0 ; k < n_ind ; k++) {
					unsigned int pixel_idx = region_inds[k]-1; // 0-based idx
					if ( !bw[pixel_idx] ) {
						bw[pixel_idx] = 1;
						merge_inds.push_back(region_inds[k]);
						size_t x = pixel_idx/im_h;
						size_t y = pixel_idx%im_h;
						accm_x += x;
						accm_y += y;
						if ( min_x > x ) min_x = x;
						if ( max_x < x ) max_x = x;
						if ( min_y > y ) min_y = y;
						if ( max_y < y ) max_y = y;
					}
				}
			}
		}
		double merge_area = merge_inds.size();
		double center_x = accm_x/merge_area + 1; // 1-based idx
		double center_y = accm_y/merge_area + 1;
		double lx = min_x + 1;
		double rx = max_x + 1;
		double ty = min_y + 1;
		double dy = max_y + 1;
		merged_region_size[i] = merge_area;
		merged_region_loc[2*i] = center_x;
		merged_region_loc[2*i+1] = center_y;
		merged_region_bbox[4*i] = lx;
		merged_region_bbox[4*i+1] = rx;
		merged_region_bbox[4*i+2] = ty;
		merged_region_bbox[4*i+3] = dy;

		mxArray* mx_merge_inds = 
			mxCreateNumericMatrix(1, merge_inds.size(), mxUINT32_CLASS, mxREAL);
		unsigned int *merge_inds_ = (unsigned int*)mxGetData(mx_merge_inds);
		for ( size_t j=0 ; j < merge_inds.size(); j++) {
			merge_inds_[j] = merge_inds[j]; 
		}
		mxSetCell(plhs[0], i, mxDuplicateArray(mx_merge_inds));
		mxDestroyArray(mx_merge_inds);
	
	}
}