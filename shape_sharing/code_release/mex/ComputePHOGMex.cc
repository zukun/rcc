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

void MakeROI(double* ori_mat, double* roi, vector<size_t>& ptr_inds, size_t n_ptr, size_t im_dim[2], size_t roi_dim[2], size_t sx, size_t sy, size_t n_bin)
{	
	size_t im_h = im_dim[0];
	size_t im_w = im_dim[1];
	size_t roi_h = roi_dim[0];
	size_t roi_w = roi_dim[1];
	size_t stride = im_h;
	size_t bin_stride = im_h*im_w;
	size_t roi_bin_stride = roi_h*roi_w;
	size_t roi_stride = roi_h;

	for (size_t b=0; b < n_bin ; b++) {
		size_t bin_stride_b = bin_stride*b;
		size_t roi_bin_stride_b = roi_bin_stride*b;
		for (size_t i=0 ; i < n_ptr ; i++) {
			size_t img_idx = ptr_inds[i];
			size_t im_x = img_idx/im_h;
			size_t im_y = img_idx%im_h;
			size_t roi_x = im_x - sx;
			size_t roi_y = im_y - sy;
			
			size_t idx =  bin_stride_b + img_idx; 
			size_t roi_idx = roi_bin_stride_b + roi_x*roi_stride + roi_y;
			roi[roi_idx] = ori_mat[idx];
		}
	}
	
}

void SubMatSum(double* mat, vector<double>::iterator mat_sum, size_t mat_dim[2], size_t n_bin, size_t sx, size_t ex, size_t sy, size_t ey)
{
	size_t h = mat_dim[0];
	size_t w = mat_dim[1];
	vector<double>::iterator it = mat_sum;
	for (size_t b=0 ; b < n_bin; b++, it++) {
		size_t stride_b = b*w*h;
		*it = 0.0; // initialize
		for (size_t x = sx; x <= ex ; x++) {
			size_t stride = x*h + stride_b;
			for (size_t y=sy ; y <= ey; y++) {
				size_t idx = stride +y;
				*it += mat[idx];
			}
		}
	}
}

void GetPHOGBBox(vector<size_t>& area_inds, size_t phog_bbox[4], size_t im_w, size_t im_h, size_t phog_L)
{
	// min/max
	int lx(im_w-1), rx(0), ty(im_h-1), dy(0);
	size_t n_ptr = area_inds.size();
	size_t stride = im_h;
	for (size_t i=0 ; i < n_ptr; i++) {
		size_t xv = area_inds[i]/stride;
		size_t yv = area_inds[i]%stride;
		if ( xv < lx ) lx = xv;
		if ( xv > rx ) rx = xv;
		if ( yv < ty ) ty = yv;
		if ( yv > dy ) dy = yv;
	}

	// margin
	size_t w = rx - lx + 1;
	size_t h = dy - ty + 1;
	size_t nbin = (size_t)pow(2.0, (int)phog_L);
	size_t margin_w = nbin - w%nbin;
	size_t margin_h = nbin - h%nbin;

	if ( margin_w != nbin ) {
		int margin_lx = floor((double)margin_w/2.0);
		int margin_rx = ceil((double)margin_w/2.0);
		lx = max(lx - margin_lx,(int)0);
	    rx = min(rx + margin_rx,(int)im_w-1);
	}
	if ( margin_h != nbin ) {
		int margin_ty = floor((double)margin_h/2);
		int margin_dy = ceil((double)margin_h/2);
		ty = max(ty - margin_ty,(int)0);
		dy = min(dy + margin_dy,(int)im_h-1);
	}

	// rectangle near the image boundary
	w = rx - lx + 1;
	h = dy - ty + 1;   
	margin_w = nbin - w%nbin;
	margin_h = nbin - h%nbin;
	if ( margin_w != nbin ) {
		int margin_lx = lx - 1;
		int margin_rx = w - rx;
		if ( margin_lx > margin_rx ) {
			lx = lx - margin_w;
		}
		else {
			rx = rx + margin_w;
		}
	}
	if ( margin_h != nbin ) {
		int margin_ty = ty - 1;
		int margin_dy = h - dy;
		if ( margin_ty > margin_dy ) {
			ty = ty - margin_h;
		}
		else {
			dy = dy + margin_h;
		}
	}

	lx = max(lx, (int)0);
	rx = min(rx, (int)im_w-1);
	ty = max(ty, (int)0);
	dy = min(dy, (int)im_h-1);
	
		
	phog_bbox[0] = lx;
	phog_bbox[1] = rx;
	phog_bbox[2] = ty;
	phog_bbox[3] = dy;
}

void ComputePHOG(double* pb_orient, vector<size_t>& area_inds, size_t im_dim[2], size_t bbox[4], size_t n_bin, size_t phog_L, vector<double>& phog)
// phog is not L1 normalized. (just summation)
{
	// phog parameter
	size_t phog_dim = phog.size();
    size_t lx = bbox[0]; 
	size_t rx = bbox[1];
	size_t ty = bbox[2];
	size_t dy = bbox[3];
	size_t w = rx-lx+1;
	size_t h = dy-ty+1;		
	size_t n_area = area_inds.size(); 

	// make ROI
	size_t roi_dim[2];
	roi_dim[0] = h;
	roi_dim[1] = w;
	double* roi = (double*)mxCalloc(h*w*n_bin, sizeof(double));
	MakeROI(pb_orient, roi, area_inds, n_area, im_dim, roi_dim, lx, ty, n_bin);	
	// phog computation
	size_t cell_offset(0);
	for (size_t l=0 ; l <= phog_L; l++) {
		size_t n_spatial_bin = pow(2.0,(int)l);
		size_t grid_x = w/n_spatial_bin;
		size_t grid_y = h/n_spatial_bin;
		size_t x_bin(0);
		for (size_t x=0; x_bin < n_spatial_bin; x += grid_x, x_bin++) {
			size_t sx = x;
			size_t ex = x + grid_x - 1;
			
			size_t y_bin(0);
			for ( size_t y=0; y_bin < n_spatial_bin; y += grid_y, y_bin++) {
				size_t sy = y;
				size_t ey = y + grid_y -1;
				
				SubMatSum(roi, phog.begin() + cell_offset , roi_dim, n_bin, sx, ex, sy, ey);
				cell_offset += n_bin;
			}
		}
	}
	mxFree(roi);		
}

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// pb_orient(3D matrix)
// bplr_area_inds (1D cell, unsigned int)
// phog_L (a scalar)
// lhs (output):
// PHOG (2D matrix)
{
 	if ( nrhs != 3 ) { 
		mexPrintf("we need 3 inputs\n");
		return;
	}
	
	if ( mxGetNumberOfDimensions(prhs[0]) != 3 ) {
		mexPrintf("wrong input\n");
		return;
	}
	
	if ( nlhs == 0 ) {
		return;
	}
	if ( nlhs > 1 ) {
		mexPrintf("too many output\n");
		return;
	}

	// parameters
	const mwSize* img_dim = mxGetDimensions(prhs[0]);
	const size_t im_h = img_dim[0];
	const size_t im_w = img_dim[1];
	size_t im_dim[2] = {im_h, im_w};
	const size_t n_bin = img_dim[2];
	const size_t n_bplr = mxGetNumberOfElements(prhs[1]);
	const size_t phog_L = (size_t)mxGetScalar(prhs[2]);
	size_t phog_dim = n_bin*(pow(4.0,(int)phog_L+1)-1)/3;
		
	// output
	double *phog;
	plhs[0] = mxCreateDoubleMatrix(phog_dim, n_bplr, mxREAL);
	phog = mxGetPr(plhs[0]);
	
	// phog 
	for ( size_t i=0; i < n_bplr ; i++) {
		// area inds of a bplr
		mxArray* mx_area_inds = mxGetCell(prhs[1], i);
		size_t n_area = mxGetNumberOfElements(mx_area_inds);
		vector<size_t> area_inds(n_area);
		unsigned int *area_inds_ = (unsigned int*)mxGetData(mx_area_inds);
		for (size_t j=0 ; j < n_area; j++) {
			area_inds[j] = area_inds_[j] - 1; //zero-based 
		}
		
		// bbox of a bplr
		size_t phog_bbox[4];
		GetPHOGBBox(area_inds, phog_bbox, im_w, im_h, phog_L);
		
		// phog
		double* pb_orient = mxGetPr(prhs[0]);
		vector<double> phog_i(phog_dim,0.0);
		ComputePHOG(pb_orient, area_inds, im_dim, phog_bbox, n_bin, phog_L, phog_i);
		double hist_sum = accumulate(phog_i.begin(), phog_i.end(), 0.0);
		size_t phog_offset = phog_dim*i;
		for ( size_t j=0 ; j < phog_dim; j++ ) {
			phog[phog_offset+j] = phog_i[j]/hist_sum; // normalize
		}
		
	}
		
}



