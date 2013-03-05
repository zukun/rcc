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

void FindBWContour(vector<unsigned char>& bw, size_t h, size_t w, vector<size_t>& c_ptrs)
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
					c_ptrs.push_back(x);
					c_ptrs.push_back(y);
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



void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// region_bw_inds (1xn cell, each cell: 1d vector, unsigned int)
// img_size (1d vector)
// angle_bin (1d vector)
// length_bin (1d vector)
// rsz (a scalar)
// lhs (output):
// shape_context (2D matrix)
// feat_size (1xn vector)
// feat_center (2xn matrix)
{
 	if ( nrhs != 5 ) { 
		mexPrintf("we need 6 inputs\n");
		return;
	}
	
	if ( nlhs == 0 ) {
		return;
	}
	if ( nlhs > 3 ) {
		mexPrintf("too many output\n");
		return;
	}

	// parameters
	size_t n_region = mxGetNumberOfElements(prhs[0]);
	double *angle_bin = mxGetPr(prhs[2]);
	double *length_bin = mxGetPr(prhs[3]);
	size_t n_angle_bin = mxGetNumberOfElements(prhs[2]) - 1;
	size_t n_length_bin = mxGetNumberOfElements(prhs[3]) - 1;
	unsigned int feat_dim = n_angle_bin*n_length_bin;
	const double PI = 2*acos(0.0);
	double *img_size = mxGetPr(prhs[1]);
	const size_t im_h = img_size[0];
	const size_t im_w = img_size[1];
	size_t im_dim[2] = {im_h, im_w};
	double rsz = mxGetScalar(prhs[4]);

	// allocate output
	double *shape_context;
	double *feat_size;
	double *feat_centroid;
	// chordiogram
	plhs[0] = mxCreateNumericMatrix(feat_dim, n_region, mxDOUBLE_CLASS, mxREAL);
	shape_context = mxGetPr(plhs[0]);
	// scale
	plhs[1] = mxCreateDoubleMatrix(1, n_region, mxREAL);
	feat_size = mxGetPr(plhs[1]);
	// centroid
	plhs[2] = mxCreateDoubleMatrix(2, n_region, mxREAL);
	feat_centroid = mxGetPr(plhs[2]);
	
	// main loop
	for(size_t i=0; i < n_region; i++) {
		
		// area inds of a region
		mxArray* mx_area_inds = mxGetCell(prhs[0],i);
		unsigned int* area_inds = (unsigned int*)mxGetData(mx_area_inds);
		size_t area = mxGetNumberOfElements(mx_area_inds);
		vector<unsigned char> ori_mask(im_h*im_w,0); // bit-vector
		double xx(0.0), yy(0.0);
		for ( size_t j=0 ; j < area ; j++) {
			size_t idx = area_inds[j] - 1; // zero-based index
			size_t x_ = idx/im_h; 
			size_t y_ = idx%im_h;
			xx += x_;
			yy += y_;
			ori_mask[idx] = 1;
		}
		
		// resize
		size_t src_size[2];
		src_size[0] = im_h;
		src_size[1] = im_w;
		size_t dst_size[2];
		size_t mask_h = floor(rsz*im_h + 0.5);
		size_t mask_w = floor(rsz*im_w + 0.5);
		dst_size[0] = mask_h;
		dst_size[1] = mask_w;
		vector<unsigned char> mask(dst_size[0]*dst_size[1],0);
		Resize(ori_mask, mask, src_size, dst_size, rsz);

		feat_size[i] = rsz*area;
		feat_centroid[2*i] = rsz*xx/area + 1; // 1-based idx
		feat_centroid[2*i+1] = rsz*yy/area + 1;
				
		// contour ptr
		vector<size_t> ptrs;
		FindBWContour(mask, mask_h, mask_w, ptrs);
		size_t n_ptr = ptrs.size()/2;

		// distance and angle
		vector<double> dist_vec(n_ptr, 0.0);
		vector<double> angle_vec(n_ptr,0.0);
		double cx = feat_centroid[2*i];
		double cy = feat_centroid[2*i+1];
		for(unsigned int j=0; j < n_ptr; j++) {
			double ptr_xj = ptrs[2*j];
			double ptr_yj = ptrs[2*j+1];
			size_t stride = j*n_ptr;
			double dx = ptr_xj - cx;
			double dy = ptr_yj - cy;
			double dist = sqrt(dx*dx + dy*dy);
			double angle = atan2(dy,dx);
			if ( angle < 0 ) angle += 2*PI;
			dist_vec[j] = dist;
			angle_vec[j] = angle;
		}
		
		// shape context
		double mean_dist = accumulate(dist_vec.begin(), dist_vec.end(), 0.0)/(double)dist_vec.size();
		unsigned int feat_stride = feat_dim*i;
		for (unsigned int j=0 ; j < n_ptr ; j++) {
			unsigned int angle_bin_idx = histBinning(angle_vec[j], angle_bin, n_angle_bin);
			unsigned int dist_bin_idx = histBinning(dist_vec[j]/mean_dist, length_bin, n_length_bin);
			unsigned int bin_idx = n_length_bin*angle_bin_idx + dist_bin_idx;
			shape_context[feat_stride + bin_idx]++;
		}

	}
		
}



