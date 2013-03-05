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
// angle_bin1 (1d vector)
// angle_bin2 (1d vector)
// length_bin (1d vector)
// rsz (a scalar)
// lhs (output):
// chordiogram (2D matrix)
// feat_size (1xn vector)
// feat_center (2xn matrix)
{
 	if ( nrhs != 6 ) { 
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
	double *chord_bin1 = mxGetPr(prhs[2]);
	double *chord_bin2 = mxGetPr(prhs[2]);
	double *chord_bin3 = mxGetPr(prhs[3]);
	double *chord_bin4 = mxGetPr(prhs[4]);
	size_t n_angle_bin1 = mxGetNumberOfElements(prhs[2]) - 1;
	size_t n_angle_bin2 = mxGetNumberOfElements(prhs[3]) - 1;
	size_t n_length_bin = mxGetNumberOfElements(prhs[4]) - 1;
	size_t chord_dim1 = n_angle_bin1;
	size_t chord_dim2 = n_angle_bin1;
	size_t chord_dim3 = n_angle_bin2;
	size_t chord_dim4 = n_length_bin;
	size_t chord_dim1_offset = 0;
	size_t chord_dim2_offset = chord_dim1;
	size_t chord_dim3_offset = chord_dim1 + chord_dim2;
	size_t chord_dim4_offset = chord_dim1 + chord_dim2 + chord_dim3;
	unsigned int chordiogram_dim = chord_dim1 + chord_dim2 + chord_dim3 + chord_dim4;
	const double PI = 2*acos(0.0);
	double *img_size = mxGetPr(prhs[1]);
	const size_t im_h = img_size[0];
	const size_t im_w = img_size[1];
	size_t im_dim[2] = {im_h, im_w};
	double rsz = mxGetScalar(prhs[5]);

	// allocate output
	double *chordiogram;
	double *chordiogram_size;
	double *chordiogram_centroid;
	// chordiogram
	plhs[0] = mxCreateNumericMatrix(chordiogram_dim, n_region, mxDOUBLE_CLASS, mxREAL);
	chordiogram = mxGetPr(plhs[0]);
	// scale
	plhs[1] = mxCreateDoubleMatrix(1, n_region, mxREAL);
	chordiogram_size = mxGetPr(plhs[1]);
	// centroid
	plhs[2] = mxCreateDoubleMatrix(2, n_region, mxREAL);
	chordiogram_centroid = mxGetPr(plhs[2]);
	
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

		chordiogram_size[i] = rsz*area;
		chordiogram_centroid[2*i] = rsz*xx/area + 1; // 1-based idx
		chordiogram_centroid[2*i+1] = rsz*yy/area + 1;
				
		// contour ptr
		vector<size_t> ptrs;
		FindBWContour(mask, mask_h, mask_w, ptrs);
		size_t n_ptr = ptrs.size()/2;

		// pairwise distance and neighbor
		vector<vector<size_t> > neighbor(n_ptr);
		double neighbor_thresh = sqrt(2.0) + 1e-6;
		vector<double> dist_mat(n_ptr*n_ptr, 0);
		vector<pair<double, double> > ptr_diff_mat(n_ptr*n_ptr);
		for(unsigned int j=0; j < n_ptr; j++) {
			double ptr_xj = ptrs[2*j];
			double ptr_yj = ptrs[2*j+1];
			size_t stride = j*n_ptr;
			for (unsigned int k=0; k < n_ptr ; k++) {
				double ptr_xk = ptrs[2*k];
				double ptr_yk = ptrs[2*k+1];
				double dx = ptr_xk-ptr_xj;
				double dy = ptr_yk-ptr_yj;
				double d = sqrt(dx*dx + dy*dy);
				dist_mat[stride+k] = d;
				dist_mat[stride+k] = d;
				ptr_diff_mat[stride+k].first = dx;
				ptr_diff_mat[stride+k].second = dy;
				if ( d < neighbor_thresh && j != k) {
					neighbor[j].push_back(k);
				}
			}
		}
		
		// normal vector
		size_t height = mask_h;
		size_t width = mask_w;
		vector<double> normal_angle(n_ptr);
		vector<unsigned int> tf(n_ptr, 0);
		for (unsigned int j=0; j < n_ptr; j++) {
			size_t n_neighbor = neighbor[j].size();
			if (n_neighbor == 2) {
				tf[j] = 1;
				double ptr_xj = ptrs[2*j];
				double ptr_yj = ptrs[2*j+1];
				int x = (int)ptr_xj;
				int y = (int)ptr_yj;
				int lx = (int)(x-1);
				int rx = (int)(x+1);
				int ty = (int)(y-1);
				int dy = (int)(y+1);
				
				int lxv(0), rxv(0), tyv(0), dyv(0);
				if ( lx >= 0 ) lxv = mask[height*lx + y];
				if ( rx < width ) rxv = mask[height*rx + y];
				if ( ty >= 0 ) tyv = mask[height*x + ty];
				if ( dy < height ) dyv = mask[height*x + dy];
				int gx = rxv - lxv;
				int gy = dyv - tyv;
				if ( gx != 0 ) gx = gx > 0 ? 1 : -1;
				if ( gy != 0 ) gy = gy > 0 ? 1 : -1;
				
				// normal
				unsigned int n_idx1 = neighbor[j][0];
				unsigned int n_idx2 = neighbor[j][1];
				double ptr_x1 = ptrs[2*n_idx1];
				double ptr_y1 = ptrs[2*n_idx1+1];
				double ptr_x2 = ptrs[2*n_idx2];
				double ptr_y2 = ptrs[2*n_idx2+1];

				double tan_x = ptr_x2 - ptr_x1;
				double tan_y = ptr_y2 - ptr_y1;
				double normal_x = gx*fabs(tan_y);
				double normal_y = gy*fabs(tan_x);
				normal_angle[j] = atan2(normal_y, normal_x);
				if ( normal_angle[j] < 0 ) {
					normal_angle[j] = normal_angle[j] + 2*PI;
				}
			}
		}

		// chordiogram
		double mean_dist = accumulate(dist_mat.begin(), dist_mat.end(), 0.0)/(double)dist_mat.size();
		unsigned int chordiogram_stride = chordiogram_dim*i;
		for (unsigned int j=0 ; j < n_ptr ; j++) {
			
			if ( tf[j] ) {

				size_t stride = j*n_ptr;
				double normal_angle_j = normal_angle[j];
				for (unsigned int k=0; k < n_ptr; k++) {
					if ( j != k ) {
						double normal_angle_k = normal_angle[k];					
						double chord_angle = 
							atan2(ptr_diff_mat[stride+k].second, ptr_diff_mat[stride+k].first);
						if ( chord_angle < 0 ) {
							chord_angle = chord_angle + 2*PI;
						}
						// chord 
						double chord1 = normal_angle_k - chord_angle;
						double chord2 = normal_angle_j - chord_angle;
						double chord3 = chord_angle;
						double chord4 = dist_mat[stride+k]/mean_dist;
						// binning --> chordiogram
						unsigned int bin1 = histBinning(chord1, chord_bin1, chord_dim1);
						unsigned int bin2 = histBinning(chord2, chord_bin2, chord_dim2) + chord_dim2_offset;
						unsigned int bin3 = histBinning(chord3, chord_bin3, chord_dim3) + chord_dim3_offset;
						unsigned int bin4 = histBinning(chord4, chord_bin4, chord_dim4) + chord_dim4_offset;
						// increment chordiogram	
						chordiogram[chordiogram_stride + bin1]++;
						chordiogram[chordiogram_stride + bin2]++;
						chordiogram[chordiogram_stride + bin3]++;
						chordiogram[chordiogram_stride + bin4]++;
					}
				}
			}
		}

	}
		
}



