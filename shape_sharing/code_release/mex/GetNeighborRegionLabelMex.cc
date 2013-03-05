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
// labels (1xL cell, each cell 2D double image)
// n_max_label (1xL vector)
// border (1xn cell, each cell 2D logical image)
// lhs (output):
// neighbor(Lxn cell)
{
 	if ( nrhs != 3 ) { 
		mexPrintf("we need 3 inputs\n");
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
	size_t n_level = mxGetNumberOfElements(prhs[0]);
	size_t n_region = mxGetNumberOfElements(prhs[2]);
	double* n_max_label = mxGetPr(prhs[1]);
	size_t n_min_elem = 3;

	// output
	mwSize dim[2];
	dim[0] = n_level;
    dim[1] = n_region;
	plhs[0] = mxCreateCellArray(2, dim);

	// neighbor
	for (size_t i=0 ; i < n_region ; i++ ) {
		mxArray* mx_border = mxGetCell(prhs[2],i);
		mxLogical* border = mxGetLogicals(mx_border);
		size_t im_h = mxGetM(mx_border);
		size_t im_w = mxGetN(mx_border);
		vector<size_t> border_inds;
		for ( size_t x=0; x < im_w; x++) {
			for ( size_t y=0; y < im_h ; y++) {
				size_t idx = x*im_h + y;
				if ( border[idx] ) border_inds.push_back(idx);
			}
		}

		for ( size_t j=0 ; j < n_level ; j++) {
			mxArray* mx_label = mxGetCell(prhs[0],j);
			double* label = mxGetPr(mx_label);
			size_t max_label = n_max_label[j];
			vector<size_t> neighbor_label_count(max_label+1,0);
			vector<double> neighbor_label;				
			size_t n_border = border_inds.size();
			for (size_t k=0 ; k < n_border ; k++) {
				size_t idx = border_inds[k];
				size_t label_idx = (size_t)label[idx];
				if ( neighbor_label_count[label_idx] == 0 ) {
					neighbor_label.push_back(label_idx);
				}
				neighbor_label_count[label_idx]++;
			}
			size_t n_neighbor = neighbor_label.size();
			vector<size_t> neighbor;
			for (size_t k=0 ; k < n_neighbor ; k++) {
				size_t label_idx = neighbor_label[k];
				if ( label_idx > 0 && neighbor_label_count[label_idx] >= n_min_elem ) {
					neighbor.push_back(label_idx);
				}
			}
			n_neighbor = neighbor.size();
			mxArray *mx_neighbor = mxCreateDoubleMatrix(1,n_neighbor,mxREAL);
			double* neighbor_ = mxGetPr(mx_neighbor);
			for (size_t k=0 ; k < n_neighbor ; k++) {
				neighbor_[k] = neighbor[k];
			}
            size_t cell_idx = i*n_level + j;
			mxSetCell(plhs[0], cell_idx, mxDuplicateArray(mx_neighbor));
			mxDestroyArray(mx_neighbor);
		}
	}
		
}



