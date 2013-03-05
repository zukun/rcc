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

void ComputeGY(vector<size_t>& pgy_inds, vector<size_t>& ngy_inds, vector<unsigned char>& proj_bw, int bbox[4], size_t proj_h)  
{
	int min_x = bbox[0];
	int max_x = bbox[1];
	int min_y = bbox[2];
	int max_y = bbox[3];
	size_t proj_stride = proj_h;
	
	if ( min_x > max_x || min_y > max_y )
		return;
	

	if ( min_y > 0 && max_y < proj_h-1 ) { // normal case
		for ( size_t x= min_x ; x <= max_x ; x++) {
			for (size_t y= min_y; y <= max_y ; y++) {
				size_t proj_idx = proj_stride*x + y;
				if ( proj_bw[proj_idx] ) {
					size_t ty_idx = proj_stride*x + y-1;
					size_t dy_idx = proj_stride*x + y+1;
					int gy = proj_bw[dy_idx] - proj_bw[ty_idx];
					
					if ( gy > 0 ) {
						pgy_inds.push_back(proj_idx);
					}
					else if ( gy < 0 ) {
						ngy_inds.push_back(proj_idx);
					}
				}
			}
		}
	}
	else if ( min_y == 0 && max_y < proj_h -1) {
		for ( size_t x= min_x ; x <= max_x ; x++) {
			size_t y = min_y;
			size_t proj_idx = proj_stride*x + y;
			if ( proj_bw[proj_idx] ) {
				size_t dy_idx = proj_stride*x + y+1;
				int gy = proj_bw[dy_idx];
				if ( gy > 0 ) {
					pgy_inds.push_back(proj_idx);
				}
			}
			for (y= min_y+1; y <= max_y ; y++) {
				size_t proj_idx = proj_stride*x + y;
				if ( proj_bw[proj_idx] ) {
					size_t ty_idx = proj_stride*x + y-1;
					size_t dy_idx = proj_stride*x + y+1;
					int gy = proj_bw[dy_idx] - proj_bw[ty_idx];
					
					if ( gy > 0 ) {
						pgy_inds.push_back(proj_idx);
					}
					else if ( gy < 0 ) {
						ngy_inds.push_back(proj_idx);
					}
				}
			}
		}
	}
	else if ( min_y > 0 && max_y == proj_h -1) {
		for ( size_t x= min_x ; x <= max_x ; x++) {
			
			for (size_t y= min_y; y <= max_y-1 ; y++) {
				size_t proj_idx = proj_stride*x + y;
				if ( proj_bw[proj_idx] ) {
					size_t ty_idx = proj_stride*x + y-1;
					size_t dy_idx = proj_stride*x + y+1;
					int gy = proj_bw[dy_idx] - proj_bw[ty_idx];
					
					if ( gy > 0 ) {
						pgy_inds.push_back(proj_idx);
					}
					else if ( gy < 0 ) {
						ngy_inds.push_back(proj_idx);
					}
				}
			}
			size_t y = max_y;
			size_t proj_idx = proj_stride*x + y;
			if ( proj_bw[proj_idx] ) {
				size_t ty_idx = proj_stride*x + y-1;
				int gy = proj_bw[ty_idx];
				if ( gy > 0 ) {
					ngy_inds.push_back(proj_idx);
				}
			}
		}
	}
	else {
		for ( size_t x= min_x ; x <= max_x ; x++) {
			size_t y = min_y;
			size_t proj_idx = proj_stride*x + y;
			if ( proj_bw[proj_idx] ) {
				size_t dy_idx = proj_stride*x + y+1;
				int gy = proj_bw[dy_idx];
				if ( gy > 0 ) {
					pgy_inds.push_back(proj_idx);
				}
			}
			for (size_t y= min_y+1; y <= max_y-1 ; y++) {
				size_t proj_idx = proj_stride*x + y;
				if ( proj_bw[proj_idx] ) {
					size_t ty_idx = proj_stride*x + y-1;
					size_t dy_idx = proj_stride*x + y+1;
					int gy = proj_bw[dy_idx] - proj_bw[ty_idx];
					
					if ( gy > 0 ) {
						pgy_inds.push_back(proj_idx);
					}
					else if ( gy < 0 ) {
						ngy_inds.push_back(proj_idx);
					}
				}
			}

			y = max_y;
			proj_idx = proj_stride*x + y;
			if ( proj_bw[proj_idx] ) {
				size_t ty_idx = proj_stride*x + y-1;
				int gy = proj_bw[ty_idx];
				if ( gy > 0 ) {
					ngy_inds.push_back(proj_idx);
				}
			}
		}
	}

}
void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// bw (1xn cell, each cell: 2d binary image)
// scale (1xn vector)
// translation (2xn matrix)
// out_size (1x2 vector)
// lhs (output):
// pgy_inds (1xn cell, each cell: 1d vector)
// ngy_inds (1xn cell, each cell: 1d vector)
// proj_area (1xn vector)
// proj_bw_inds (1xn cell, each cell: 1d vector)
{
 	if ( nrhs != 4 ) { 
		mexPrintf("we need 4 inputs\n");
		return;
	}
	if ( nlhs < 2 || nlhs > 4) {
		mexPrintf("2, 3 or 4 outputs are only allowed\n");
		return;
	}	
	
	// parameters
	size_t n_proj = mxGetNumberOfElements(prhs[0]);
	double* out_size = mxGetPr(prhs[3]);
	size_t out_h = out_size[0];
	size_t out_w = out_size[1];
	
	// allocate output
	if ( nlhs == 2) {
		mwSize dim[1];
		dim[0] = n_proj;
		plhs[0] = mxCreateCellArray(1, dim);
		plhs[1] = mxCreateCellArray(1, dim);
	}
	else if ( nlhs == 3 ) {
		mwSize dim[1];
		dim[0] = n_proj;
		plhs[0] = mxCreateCellArray(1, dim);
		plhs[1] = mxCreateCellArray(1, dim);
		plhs[2] = mxCreateDoubleMatrix(1, n_proj, mxREAL);
	}
	else {
		mwSize dim[1];
		dim[0] = n_proj;
		plhs[0] = mxCreateCellArray(1, dim);
		plhs[1] = mxCreateCellArray(1, dim);
		plhs[2] = mxCreateDoubleMatrix(1, n_proj, mxREAL);
		plhs[3] = mxCreateCellArray(1, dim);
	}

	// projection
	double *scale = mxGetPr(prhs[1]);
	double *trans = mxGetPr(prhs[2]);
	for (size_t i=0 ; i < n_proj ; i++ ) {
		
		const mxArray *mx_bw = mxGetCell(prhs[0],i);
		mxLogical *bw = mxGetLogicals(mx_bw);
		size_t im_h = mxGetM(mx_bw);
		size_t im_w = mxGetN(mx_bw);
		size_t im_stride = im_h;
		
		// boundbox
		size_t min_x = im_w-1;
		size_t max_x = 0;
		size_t min_y = im_h-1;
		size_t max_y = 0;
		for (size_t x=0; x < im_w ; x++) {
			for (size_t y=0; y < im_h ; y++) {
				size_t idx = x*im_stride + y;
				if ( bw[idx] ) {
					if ( min_x > x) min_x = x;
					if ( max_x < x) max_x = x;
					if ( min_y > y) min_y = y;
					if ( max_y < y) max_y = y;
				}
			}
		}

        vector<size_t> proj_inds;
		vector<unsigned char> proj_bw(out_h*out_w,0);
        double s = scale[i];
        int proj_min_x(out_h-1); 
        int proj_max_x(0); 
        int proj_min_y(out_w-1); 
        int proj_max_y(0); 
        if  ( s > 0 ) {
            double tx = trans[2*i];
            double ty = trans[2*i+1];

            // back-projection
            proj_min_x = max(0, (int)(s*min_x + tx + 0.5));
            proj_max_x = min((int)out_w-1, (int)(s*max_x + tx + 0.5));
            proj_min_y = max(0, (int)(s*min_y + ty + 0.5));
            proj_max_y = min((int)out_h-1, (int)(s*max_y+ty + 0.5));
            size_t out_stride = out_h;

            for ( int x=proj_min_x; x <= proj_max_x ; x++) {
                for (int y=proj_min_y; y <= proj_max_y ; y++) {
                    // back-project: nearest neighbor
                    int x_ori = (int)floor(((double)x-tx)/s + 0.5);
                    int y_ori = (int)floor(((double)y-ty)/s + 0.5);
                    int im_idx = x_ori*im_stride + y_ori;
                    if ( im_idx >= 0 && im_idx < im_h*im_w) {
                        if ( bw[im_idx] ) {
                            size_t proj_idx = out_stride*x + y;
                            proj_inds.push_back(proj_idx);
                            proj_bw[proj_idx] = 1;
                        }
                    }
                }
            }
        }
		// gradient along y
		vector<size_t> pgy_inds;
		vector<size_t> ngy_inds;
		int bbox[4];
		bbox[0] = proj_min_x;
		bbox[1] = proj_max_x;
		bbox[2] = proj_min_y;
		bbox[3] = proj_max_y;
		ComputeGY(pgy_inds, ngy_inds, proj_bw, bbox, out_h);

		if ( nlhs == 2 ) {
			mxArray* mx_pgy_inds = mxCreateNumericMatrix(1, pgy_inds.size(), mxUINT32_CLASS, mxREAL);
			mxArray* mx_ngy_inds = mxCreateNumericMatrix(1, ngy_inds.size(), mxUINT32_CLASS, mxREAL);
			unsigned int* pgy_inds_ = (unsigned int*)mxGetData(mx_pgy_inds);
			unsigned int* ngy_inds_ = (unsigned int*)mxGetData(mx_ngy_inds);
			
			for ( size_t j=0 ; j < pgy_inds.size() ; j++ ) {
				pgy_inds_[j] = pgy_inds[j] + 1; // 1-based
			}
			for ( size_t j=0 ; j < ngy_inds.size() ; j++ ) {
				ngy_inds_[j] = ngy_inds[j] + 1; // 1-based
			}
			mxSetCell(plhs[0], i, mxDuplicateArray(mx_pgy_inds));
			mxSetCell(plhs[1], i, mxDuplicateArray(mx_ngy_inds));
			mxDestroyArray(mx_pgy_inds);
			mxDestroyArray(mx_ngy_inds);
		}	
		else if ( nlhs == 3) {
			mxArray* mx_pgy_inds = mxCreateNumericMatrix(1, pgy_inds.size(), mxUINT32_CLASS, mxREAL);
			mxArray* mx_ngy_inds = mxCreateNumericMatrix(1, ngy_inds.size(), mxUINT32_CLASS, mxREAL);
			unsigned int* pgy_inds_ = (unsigned int*)mxGetData(mx_pgy_inds);
			unsigned int* ngy_inds_ = (unsigned int*)mxGetData(mx_ngy_inds);
			
			for ( size_t j=0 ; j < pgy_inds.size() ; j++ ) {
				pgy_inds_[j] = pgy_inds[j] + 1; // 1-based
			}
			for ( size_t j=0 ; j < ngy_inds.size() ; j++ ) {
				ngy_inds_[j] = ngy_inds[j] + 1; // 1-based
			}
			mxSetCell(plhs[0], i, mxDuplicateArray(mx_pgy_inds));
			mxSetCell(plhs[1], i, mxDuplicateArray(mx_ngy_inds));
			double* area = mxGetPr(plhs[2]);
			area[i] = proj_inds.size();

			mxDestroyArray(mx_pgy_inds);
			mxDestroyArray(mx_ngy_inds);
		}	

		else if ( nlhs == 4) {
			mxArray* mx_pgy_inds = mxCreateNumericMatrix(1, pgy_inds.size(), mxUINT32_CLASS, mxREAL);
			mxArray* mx_ngy_inds = mxCreateNumericMatrix(1, ngy_inds.size(), mxUINT32_CLASS, mxREAL);
			mxArray* mx_proj_inds = mxCreateNumericMatrix(1, proj_inds.size(), mxUINT32_CLASS, mxREAL);
			unsigned int* pgy_inds_ = (unsigned int*)mxGetData(mx_pgy_inds);
			unsigned int* ngy_inds_ = (unsigned int*)mxGetData(mx_ngy_inds);
			unsigned int* proj_inds_ = (unsigned int*)mxGetData(mx_proj_inds);
			
			for ( size_t j=0 ; j < pgy_inds.size() ; j++ ) {
				pgy_inds_[j] = pgy_inds[j] + 1; // 1-based
			}
			for ( size_t j=0 ; j < ngy_inds.size() ; j++ ) {
				ngy_inds_[j] = ngy_inds[j] + 1; // 1-based
			}
			for ( size_t j=0 ; j < proj_inds.size() ; j++ ) {
				proj_inds_[j] = proj_inds[j] + 1; // 1-based
			}
		
			mxSetCell(plhs[0], i, mxDuplicateArray(mx_pgy_inds));
			mxSetCell(plhs[1], i, mxDuplicateArray(mx_ngy_inds));
			mxSetCell(plhs[3], i, mxDuplicateArray(mx_proj_inds));
			
			double* area = mxGetPr(plhs[2]);
			area[i] = proj_inds.size();

			mxDestroyArray(mx_pgy_inds);
			mxDestroyArray(mx_ngy_inds);
			mxDestroyArray(mx_proj_inds);
		}	
	}
}