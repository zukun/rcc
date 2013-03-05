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
// overlap_score (rxn matrix)
// image_size (1x2 vector)
// region_cnt_map (2d image)
// merge_threshold (scalar)
// lhs (output): 
// merged_regions (1xn cell, each cell: 1d vector --> merged region inds, uint32)
// merged_region_size (1xn vector)
// merged_region_center (2xn matrix)
{
 	if ( nrhs != 5 ) { 
		mexPrintf("we need 5 inputs\n");
		return;
	}
	if ( nlhs == 0) {
		return;
	}	
	if ( nlhs > 3) {
		mexPrintf("1,2 or 3 outputs are only allowed\n");
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
		
	// merge
	double* overlap_score = mxGetPr(prhs[1]);
	double* region_cnt_map = mxGetPr(prhs[3]);
	double merge_threshold = mxGetScalar(prhs[4]);
	double* merged_region_size = mxGetPr(plhs[1]);
	double* merged_region_loc = mxGetPr(plhs[2]);
	
	for (size_t i=0 ; i < n_proj ; i++) {
		
		size_t stride = i*n_region;
		vector<double> accm_score(im_h*im_w,0);
		// count how many times each pixel get votes
		for (size_t j=0 ; j < n_region ; j++) {
			size_t idx = stride + j;
			double os = overlap_score[idx];
			if ( os > 0 ) {
				mxArray* mx_region_inds = mxGetCell(prhs[0],j);
				size_t n_ind = mxGetNumberOfElements(mx_region_inds);
				unsigned int* region_inds = (unsigned int*)mxGetData(mx_region_inds);
				for ( size_t k=0 ; k < n_ind ; k++) {
					unsigned int pixel_idx = region_inds[k]-1; // 0-based idx
					accm_score[pixel_idx] += os;
				}
			}
		}

		// compare to region_cnt_map and then merge
		vector<unsigned int> merge_inds;
		vector<unsigned char> bw(im_h*im_w,0);
		double accm_x(0.0), accm_y(0.0);
		for (size_t j=0 ; j < accm_score.size() ; j++) {
			double v1 = accm_score[j];
			double v2 = merge_threshold*region_cnt_map[j];
			if ( v1 > v2 ) {
				bw[j] = 1;
				merge_inds.push_back(j);
				size_t x = j/im_h;
				size_t y = j%im_h;
				accm_x += x;
				accm_y += y;
			}
		}		
		
		double merge_area = merge_inds.size();
		double center_x = accm_x/merge_area + 1; // 1-based idx
		double center_y = accm_y/merge_area + 1;
		merged_region_size[i] = merge_area;
		merged_region_loc[2*i] = center_x;
		merged_region_loc[2*i+1] = center_y;

		mxArray* mx_merge_inds = 
			mxCreateNumericMatrix(1, merge_inds.size(), mxUINT32_CLASS, mxREAL);
		unsigned int *merge_inds_ = (unsigned int*)mxGetData(mx_merge_inds);
		for ( size_t j=0 ; j < merge_inds.size(); j++) {
			merge_inds_[j] = merge_inds[j]+1; //one-based
		}
		mxSetCell(plhs[0], i, mxDuplicateArray(mx_merge_inds));
		mxDestroyArray(mx_merge_inds);
	
	}
}