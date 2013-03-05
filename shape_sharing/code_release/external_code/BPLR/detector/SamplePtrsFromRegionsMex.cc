// matlab include
#include "mex.h"
#include "matrix.h"

#include <stdlib.h>
#include <functional>
#include <numeric>
#include <algorithm>
#include <vector>
#include <math.h>
#include <list>

using namespace std;

void myMax(double *img, unsigned int bbox[4], unsigned int stride, double& max_val, double& max_x, double& max_y)
{
	unsigned int sx = bbox[0];
	unsigned int ex = bbox[1];
	unsigned int sy = bbox[2];
	unsigned int ey = bbox[3];
	unsigned int n_elem = (ex-sx+1)*(ey-sy+1);
	vector<double> img_vec(n_elem);

	// find max
	max_val = -1e6;
	unsigned int cnt(0);
	for (unsigned int x = sx; x <= ex ; x++ ) {
		unsigned stride_x = x*stride;
		for ( unsigned int y = sy; y <= ey; y++) {
			unsigned int idx = stride_x + y;
			double val = img[idx];
			img_vec[cnt] = val;
			cnt++;
			if ( val > max_val ) {
				max_val = val;
			}
		}
	}

	// find duplicate
	double dup_x(0.0);
	double dup_y(0.0);
	unsigned int dup_cnt(0);
	for (unsigned int x = sx; x <= ex ; x++ ) {
		unsigned stride_x = x*stride;
		for ( unsigned int y = sy; y <= ey; y++) {
			unsigned int idx = stride_x + y;
			if ( max_val == img[idx] ) {
				dup_x += x;
				dup_y += y;
				dup_cnt++;
			}
		}
	}
	max_x = floor(dup_x/dup_cnt + 0.5);
	max_y = floor(dup_y/dup_cnt + 0.5);
}

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// dt (1D cell, each cell: 2D image)
// boundbox (1D cell, each cell: 1D vector(lx,rx,ty,dy))
// grid_space (a scalar)
// lhs (output):
// dt_max_vals (1D cell, each cell: 1xn vector, n: number of cells in the region)
// max_locs (1D cell, each cell: 2xn matrix)
{
 	if ( nrhs != 3 ) { 
		return;
	}
	
	// parameters
	const unsigned int n_region = (unsigned int)mxGetNumberOfElements(prhs[0]);
	const double grid_space = mxGetScalar(prhs[2]);

	// output
	mwSize ndim[1];
	ndim[0] = n_region;
	plhs[0] = mxCreateCellArray(1, ndim);
	plhs[1] = mxCreateCellArray(1, ndim);

	// sample points
	for ( unsigned int i=0 ; i < n_region ; i++ ) {
		const mxArray* mx_dt = mxGetCell(prhs[0],i);
		const mxArray* mx_bbox = mxGetCell(prhs[1],i);
		double* dt = mxGetPr(mx_dt);
		double* bbox = mxGetPr(mx_bbox);
		double lx = bbox[0]-1; //zero-based index
		double rx = bbox[1]-1;
		double ty = bbox[2]-1;
		double dy = bbox[3]-1;
		unsigned int stride = mxGetM(mx_dt);
		
		vector<double> v_max_val;
		vector<double> v_max_loc;
		for ( unsigned int x = lx; x <= rx-grid_space+1; x += grid_space) {
			unsigned int sx = x;
			unsigned int ex = x + grid_space - 1;
			for ( unsigned int y = ty; y <= dy-grid_space+1; y+= grid_space) {
				unsigned int sy = y;
				unsigned int ey = y + grid_space -1;
				unsigned int local_bbox[4];
				local_bbox[0] = sx;
				local_bbox[1] = ex;
				local_bbox[2] = sy;
				local_bbox[3] = ey;
				double max_val_(-1e6);
				double max_x_(0);
				double max_y_(0);
				myMax(dt, local_bbox, stride, max_val_, max_x_, max_y_);
				v_max_val.push_back(max_val_);
				v_max_loc.push_back(max_x_);
				v_max_loc.push_back(max_y_);
			}
		}
		size_t n_cell = v_max_val.size();
		mxArray* mx_max_val = mxCreateDoubleMatrix(1, n_cell, (mxComplexity)0);
		mxArray* mx_max_loc = mxCreateDoubleMatrix(2, n_cell, (mxComplexity)0);
		double* max_val = mxGetPr(mx_max_val);
		double* max_loc = mxGetPr(mx_max_loc);
		for ( unsigned int j=0 ; j < n_cell ; j++ ) {
			max_val[j] = v_max_val[j];
			max_loc[2*j] = v_max_loc[2*j]+1; //one-based index
			max_loc[2*j+1] = v_max_loc[2*j+1]+1;
		}
		mxSetCell(plhs[0], i, mxDuplicateArray(mx_max_val));
		mxSetCell(plhs[1], i, mxDuplicateArray(mx_max_loc));
		mxDestroyArray(mx_max_val);
		mxDestroyArray(mx_max_loc);
	}
}