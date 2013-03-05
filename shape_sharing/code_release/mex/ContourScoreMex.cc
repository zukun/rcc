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

void FindBWContour(vector<unsigned char>& bw, size_t h, size_t w, vector<size_t>& c_inds, size_t& boundary_cnt)
{	
	for ( size_t x=1; x < w-1 ; x++ ) {
		size_t lx = x-1;
		size_t rx = x+1;
		size_t stride_lx = h*lx;
		size_t stride_rx = h*rx;
		size_t stride_x = h*x;
		for ( size_t y=1 ; y < h-1 ; y++) {
			
			size_t idx = stride_x + y;
			if ( bw[idx] ) {
				
				size_t ty = y-1;
				size_t dy = y+1;
				
				size_t lx_idx = stride_lx + y;
				size_t rx_idx = stride_rx + y;
				size_t ty_idx = stride_x + ty;
				size_t dy_idx = stride_x + dy;

				int gx = bw[rx_idx] - bw[lx_idx];
				int gy = bw[dy_idx] - bw[ty_idx];
				
				if ( gx != 0 || gy != 0 ) {
					c_inds.push_back(idx);
				}
			}
		}
	}
	boundary_cnt = c_inds.size();

	// image boundary 
	// --> counted, but not included in the c_inds 
	// --> penalize the image boundary
	size_t x = 0;
	for (size_t y=0; y < h ; y++ ){
		size_t idx = y;
		if ( bw[idx] ) {
			//c_inds.push_back(idx);
			boundary_cnt++;
		}
	}
	x = w-1;
	size_t stride = x*h;
	for (size_t y=0; y < h ; y++ ){
		size_t idx = stride + y;
		if ( bw[idx] ) {
			//c_inds.push_back(idx);
			boundary_cnt++;
		}
	}
	size_t y =0;
	for (size_t x=1; x < w-1; x++) {
		size_t idx = x*h;
		if ( bw[idx] ) {
			//c_inds.push_back(idx);
			boundary_cnt++;
		}
	}
	y =h-1;
	for (size_t x=1; x < w-1; x++) {
		size_t idx = x*h + y;
		if ( bw[idx] ) {
			//c_inds.push_back(idx);
			boundary_cnt++;
		}
	}

}

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// region_area_inds (1xr cell, each cell: 1d vector)
// contour_img (1xk cell, each cell: 2d (double) image)
// img_size (1x2 vector)
// lhs (output):
// contour_score (kxr vector)
{
 	if ( nrhs != 3 ) { 
		mexPrintf("we need 3 inputs\n");
		return;
	}
	
	if ( nlhs == 0 ) {
		return;
	}

	if ( nlhs > 3) {
		mexPrintf("1 output is only allowed\n");
		return;
	}

	// parameters
	size_t n_region = mxGetNumberOfElements(prhs[0]);
	size_t n_contour_img = mxGetNumberOfElements(prhs[1]);
	double* im_dim = mxGetPr(prhs[2]);
	size_t im_h = im_dim[0];
	size_t im_w = im_dim[1];

	// output
	plhs[0] = mxCreateDoubleMatrix(n_contour_img, n_region, mxREAL);
	double* score = mxGetPr(plhs[0]);
	
	// score
	for ( size_t i=0 ; i < n_region ; i++) {
		
		// region bw
		vector<unsigned char> bw(im_h*im_w,0);
		mxArray* mx_bw_inds = mxGetCell(prhs[0],i);
		unsigned int* bw_inds = (unsigned int*)mxGetData(mx_bw_inds);
		size_t n_bw_inds = mxGetNumberOfElements(mx_bw_inds);
		for ( size_t ii=0 ; ii < n_bw_inds ; ii++) {
			unsigned int idx = bw_inds[ii] -1; // zero-based
			bw[idx] = 1;
		}
		// region bw boundary
		vector<size_t> c_inds;
		size_t n_boundary(0);
		FindBWContour(bw, im_h, im_w, c_inds, n_boundary);
		
		size_t score_stride = i*n_contour_img;
		for ( size_t j=0 ; j < n_contour_img; j++) {
			
			// bottom-up contour of region
			mxArray* mx_cimg = mxGetCell(prhs[1],j);
			double* cimg = mxGetPr(mx_cimg);

			double boundary_sum(0.0);
			// boundary
			for ( size_t k=0 ; k < c_inds.size() ; k++) {
				size_t idx = c_inds[k];
				boundary_sum += cimg[idx];
			}

			// score
			if ( n_boundary ) {
				score[score_stride + j] = boundary_sum/n_boundary;
			}

		}
	}
		
}



