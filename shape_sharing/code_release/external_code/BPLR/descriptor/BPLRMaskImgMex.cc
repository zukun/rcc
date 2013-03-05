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

void GetCircleInds(vector<unsigned int>& inds, double cx, double cy, double r, double w, double h, unsigned int stride)
{
	unsigned int lx = (unsigned int)max(ceil(cx - r), 0.0);
	unsigned int rx = (unsigned int)min(floor(cx + r),w-1);
	for (unsigned int x = lx ; x <= rx ; x++ ) {
		double sqrt_part = sqrt(r*r - (x-cx)*(x-cx));
		unsigned int ty = (unsigned int)max(ceil(cy - sqrt_part),0.0);
		unsigned int dy = (unsigned int)min(floor(cy + sqrt_part),h-1);
		for ( unsigned int y = ty ; y <= dy ; y++) {
			inds.push_back(x*stride + y);
		}
	}
}


void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// elements ptrs(2xn matrix)
// elements scales (1xn vector), 
// BPLRs' elements memebers (1D cell)
// img size (1x2 vector)
// lhs (output):
// BPLRs' mask image (1D cell, each cell contain a binary image)
{
 	if ( nrhs != 4 ) { 
		return;
	}
	
	// parameters
	double *img_size = mxGetPr(prhs[3]);
	unsigned int w = (unsigned int)img_size[1];
	unsigned int h = (unsigned int)img_size[0];
	double ww = img_size[1];
	double hh = img_size[0];
	unsigned int n_bplr = mxGetNumberOfElements(prhs[2]);
	unsigned int n_element = mxGetNumberOfElements(prhs[1]);
	
	// element inds
	double* element_ptrs = mxGetPr(prhs[0]);
	double* element_scales = mxGetPr(prhs[1]);
	unsigned int stride = h;
	vector<vector<unsigned int> > element_inds(n_element);
	for ( unsigned int i=0 ; i < n_element ; i++) {
		// -1: zero-based indexing
		GetCircleInds(element_inds[i], element_ptrs[2*i]-1, element_ptrs[2*i+1]-1, element_scales[i], ww, hh, stride);
	}
	
	// output: bplr mask image
	mwSize dim[1];
	dim[0] = n_bplr;
	plhs[0] = mxCreateCellArray(1, dim);
	for ( unsigned int i=0; i < n_bplr ; i++) {
		
		// create binary image
		mxArray *mask_img = mxCreateLogicalMatrix(h,w);
		mxLogical *mask = mxGetLogicals(mask_img);

		// element members of a bplr
		const mxArray *member_element_inds = mxGetCell(prhs[2],i);
		double *member_inds = mxGetPr(member_element_inds);
		unsigned int n_member = mxGetNumberOfElements(member_element_inds);

		// fill the mask
		for ( unsigned int j=0 ; j < n_member ; j++) {
			if ( member_inds[j] < 0.0) break;
			unsigned int m_idx = (unsigned int)member_inds[j];
			m_idx = m_idx - 1; // zero-based index
			for ( vector<unsigned int>::iterator it = element_inds[m_idx].begin();
				it != element_inds[m_idx].end(); it++) {
				mask[*it] = true;
			}
		}
		mxSetCell(plhs[0], i, mxDuplicateArray(mask_img));
		mxDestroyArray(mask_img);
	}

}



