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

void MakeROI(vector<double>& ori_mat, double* roi, vector<size_t>& ptr_inds, size_t n_ptr, size_t im_dim[2], size_t roi_dim[2], size_t sx, size_t sy, size_t n_bin)
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

void ComputePHOG(vector<double>& pb_orient, vector<size_t>& area_inds, size_t im_dim[2], size_t bbox[4], size_t n_bin, size_t phog_L, vector<double>& phog)
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

void FindBWContour(vector<unsigned char>& bw, size_t h, size_t w, vector<size_t>& c_inds)
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
}

void Resize(vector<unsigned char>& src, vector<unsigned char>& dst, size_t src_size[2], size_t dst_size[2], double rsz)
{
	// parameter
	size_t im_h = src_size[0];
	size_t im_w = src_size[1];
	size_t im_stride = im_h;
	size_t out_h = dst_size[0];
	size_t out_w = dst_size[1];
	size_t out_stride = out_h;

	// boundbox
	size_t min_x = im_w;
	size_t max_x = 0;
	size_t min_y = im_h;
	size_t max_y = 0;
	for (size_t x=0; x < im_w ; x++) {
		for (size_t y=0; y < im_h ; y++) {
			size_t idx = x*im_stride + y;
			if ( src[idx] ) {
				if ( min_x > x) min_x = x;
				if ( max_x < x) max_x = x;
				if ( min_y > y) min_y = y;
				if ( max_y < y) max_y = y;
			}
		}
	}

	double s = rsz;	
	// back-projection
	int proj_min_x = max(0, (int)(s*min_x + 0.5));
	int proj_max_x = min((int)out_w-1, (int)(s*max_x + 0.5));
	int proj_min_y = max(0, (int)(s*min_y + 0.5));
	int proj_max_y = min((int)out_h-1, (int)(s*max_y + 0.5));
	for ( int x=proj_min_x; x <= proj_max_x ; x++) {
		for (int y=proj_min_y; y <= proj_max_y ; y++) {
			// back-project: nearest neighbor
			int x_ori = (int)floor(((double)x)/s + 0.5);
			int y_ori = (int)floor(((double)y)/s + 0.5);
			int im_idx = x_ori*im_stride + y_ori;
			if ( src[im_idx] ) {
				size_t proj_idx = out_stride*x + y;
				dst[proj_idx] = 1;
			}
		}
	}
}

void Correlation(vector<unsigned char>& im, double* filter, vector<size_t>& ptr_inds, vector<double>& f_out, int im_h, int im_w, int f_h, int f_w)
{
	size_t n_ptr = ptr_inds.size();
	for ( size_t i=0 ; i < n_ptr ; i++ ) {
		size_t idx = ptr_inds[i];
		int x = idx%im_h;
		int y = idx/im_h;
		int lx = max(x - f_w, 0);
		int rx = min(x + f_w, im_w-1);
		int ty = max(y - f_h, 0);
		int dy = min(y + f_h, im_h-1);
		
		int filter_idx(0);
		double filter_val(0);
		for (size_t xx = lx ; xx <= rx ; xx++) {
			size_t stride = im_h*xx;
			for (size_t yy = ty ; yy <= dy ; yy++, filter_idx++) {
				size_t ptr_idx = stride + yy;
				double f = filter[filter_idx];
				filter_val += f*im[ptr_idx];
			}
		}
		f_out[i] = filter_val;
	}
}



void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// region_bw_inds (1xn cell, each cell: 1d vector, unsigned int)
// phog_L (a scalar)
// rsz (a scalar)
// img_size (1x2 vector)
// n_bin (a scalar)
// dx_filter (2D matrix)
// dy_filter (2D matrix)
// lhs (output):
// phog (2D matrix)
{
 	if ( nrhs != 7 ) { 
		mexPrintf("we need 7 inputs\n");
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
	double* img_size = mxGetPr(prhs[3]);
	const size_t im_h = img_size[0];
	const size_t im_w = img_size[1];
	size_t im_dim[2] = {im_h, im_w};
	const size_t n_bin = (size_t)mxGetScalar(prhs[4]);
	const size_t n_region = mxGetNumberOfElements(prhs[0]);
	const size_t phog_L = (size_t)mxGetScalar(prhs[1]);
	size_t phog_dim = n_bin*(pow(4.0,(int)phog_L+1)-1)/3;
	double rsz = mxGetScalar(prhs[2]);
	const size_t dxw = mxGetN(prhs[5])/2;
	const size_t dxh = mxGetM(prhs[5])/2;
	const size_t dyw = mxGetN(prhs[6])/2;
	const size_t dyh = mxGetM(prhs[6])/2;
	const double PI = 2*acos(0.0);
	double angle_size = 2*PI/n_bin;

	// allocate output
	double *phog;
	// phog
	plhs[0] = mxCreateNumericMatrix(phog_dim, n_region, mxDOUBLE_CLASS, mxREAL);
	phog = mxGetPr(plhs[0]);
	
	// main loop
	double* dx_filter = mxGetPr(prhs[5]);
	double* dy_filter = mxGetPr(prhs[6]);
	for(size_t i=0; i < n_region; i++) {
		
		// area inds of a region
		mxArray* mx_area_inds = mxGetCell(prhs[0],i);
		unsigned int* area_inds = (unsigned int*)mxGetData(mx_area_inds);
		size_t area = mxGetNumberOfElements(mx_area_inds);
		vector<unsigned char> ori_mask(im_h*im_w,0); // bit-vector
		for ( size_t j=0 ; j < area ; j++) {
			size_t idx = area_inds[j] - 1; // zero-based index
			ori_mask[idx] = 1;
		}
		
		// resize
		size_t src_size[2];
		src_size[0] = im_h;
		src_size[1] = im_w;
		size_t dst_size[2];
		dst_size[0] = im_h;
		dst_size[1] = im_w;
		size_t mask_h = im_h;
		size_t mask_w = im_w;
			
		if ( rsz != 1.0 ) {
			mask_h = floor(rsz*im_h + 0.5);
			mask_w = floor(rsz*im_w + 0.5);
			dst_size[0] = mask_h;
			dst_size[1] = mask_w;
		}
		vector<unsigned char> mask(dst_size[0]*dst_size[1],0);
		if ( rsz != 1.0 ) { 
			Resize(ori_mask, mask, src_size, dst_size, rsz);
		}
		else {
			mask = ori_mask;
		}
				
		// contour ptr inds
		vector<size_t> c_inds;
		FindBWContour(mask, mask_h, mask_w, c_inds);

		// gradient
		vector<double> dx(c_inds.size(),0.0);
		vector<double> dy(c_inds.size(),0.0);
		Correlation(mask, dx_filter, c_inds, dx, mask_h, mask_w, dxh, dxw);
		Correlation(mask, dy_filter, c_inds, dy, mask_h, mask_w, dyh, dyw);

		// orientation
		vector<double> orientation(mask_h*mask_w*n_bin,0.0);
		size_t bin_stride = mask_h*mask_w;
		for ( size_t j=0 ; j < c_inds.size() ; j++) {
			double angle = atan2(dy[j],dx[j]);
			if ( angle < 0 ) angle += 2*PI;
			size_t bin_idx = floor(angle/angle_size);
			if ( bin_idx == n_bin ) bin_idx = 0;
			size_t im_idx = c_inds[j];
			orientation[bin_idx*bin_stride + im_idx] += 1.0;
		}
		
		// bbox of a bplr
		size_t phog_bbox[4];
		GetPHOGBBox(c_inds, phog_bbox, mask_w, mask_h, phog_L);
		
		// phog
		vector<double> phog_i(phog_dim,0.0);
		ComputePHOG(orientation, c_inds, dst_size, phog_bbox, n_bin, phog_L, phog_i);
		double hist_sum = accumulate(phog_i.begin(), phog_i.end(), 0.0);
		size_t phog_offset = phog_dim*i;
		for ( size_t j=0 ; j < phog_dim; j++ ) {
			phog[phog_offset+j] = phog_i[j]/hist_sum; // normalize
		}
	}
}



