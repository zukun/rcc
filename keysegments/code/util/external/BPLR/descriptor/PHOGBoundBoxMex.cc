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
// bplr_area_inds (1D cell)
// img size (1x2 vector)
// phog_L (a scalar)
// lhs (output):
// bplr_lx (1xn vector)
// bplr_rx (1xn vector)
// bplr_ty (1xn vector)
// bplr_dy (1xn vector)
{
 	if ( nrhs != 3 ) { 
		return;
	}
	
	// parameters
	double *img_size = mxGetPr(prhs[1]);
	double im_w = img_size[1];
	double im_h = img_size[0];
	int stride = (int)im_h;
	unsigned int n_bplr = mxGetNumberOfElements(prhs[0]);
	int phog_L = (int)mxGetScalar(prhs[2]);
	
	// bound box
	double *bplr_lx = (double*)mxCalloc(n_bplr, sizeof(double));
	double *bplr_rx = (double*)mxCalloc(n_bplr, sizeof(double));
	double *bplr_ty = (double*)mxCalloc(n_bplr, sizeof(double));
	double *bplr_dy = (double*)mxCalloc(n_bplr, sizeof(double));
	
	for ( unsigned int i=0; i < n_bplr ; i++ ) {
	
		const mxArray *ptr_inds = mxGetCell(prhs[0],i);
		unsigned int n_ptr = mxGetNumberOfElements(ptr_inds);
		double *inds = mxGetPr(ptr_inds);
		// min/max
		double lx(im_w), rx(1.0), ty(im_h), dy(1.0);
		for (unsigned int j=0 ; j < n_ptr; j++) {
			double xv = (int)(inds[j]-1)/stride;
			double yv = (int)(inds[j]-1)%stride;
			xv++;
			yv++;
			if ( xv < lx ) lx = xv;
			if ( xv > rx ) rx = xv;
			if ( yv < ty ) ty = yv;
			if ( yv > dy ) dy = yv;
		}
		// margin
		double w = rx - lx + 1;
		int i_w = (int)w;
		double h = dy - ty + 1;
		int i_h = (int)h;
		double nbin = pow(2.0, phog_L);
		int i_nbin = nbin;
		double margin_w = nbin - i_w%i_nbin;
		double margin_h = nbin - i_h%i_nbin;

		if ( margin_w != nbin ) {
			double margin_lx = floor(margin_w/2.0);
			double margin_rx = ceil(margin_w/2.0);
			lx = max(lx - margin_lx,1.0);
		    rx = min(rx + margin_rx,im_w);
		}
		if ( margin_h != nbin ) {
			double margin_ty = floor(margin_h/2);
			double margin_dy = ceil(margin_h/2);
			ty = max(ty - margin_ty,1.0);
			dy = min(dy + margin_dy,im_h);
		}

		// rectangle near the image boundary
		w = rx - lx + 1;
		h = dy - ty + 1;   
		i_w = (int)w;
		i_h = (int)h;
		margin_w = nbin - i_w%i_nbin;
		margin_h = nbin - i_h%i_nbin;
		if ( margin_w != nbin ) {
			double margin_lx = lx - 1;
			double margin_rx = w - rx;
			if ( margin_lx > margin_rx ) {
				lx = lx - margin_w;
			}
			else {
				rx = rx + margin_w;
			}
		}
		if ( margin_h != nbin ) {
			double margin_ty = ty - 1;
			double margin_dy = h - dy;
			if ( margin_ty > margin_dy ) {
				ty = ty - margin_h;
			}
			else {
				dy = dy + margin_h;
			}
		}

		// rectangle of entire image size
		lx = max(lx, 1.0);
		rx = min(rx, im_w);
		ty = max(ty, 1.0);
		dy = min(dy, im_h);

		bplr_lx[i] = lx;
		bplr_rx[i] = rx;
		bplr_ty[i] = ty;
		bplr_dy[i] = dy;
	}
	// output
	plhs[0] = mxCreateNumericMatrix(1, n_bplr, mxDOUBLE_CLASS, (mxComplexity)0);
	plhs[1] = mxCreateNumericMatrix(1, n_bplr, mxDOUBLE_CLASS, (mxComplexity)0);
	plhs[2] = mxCreateNumericMatrix(1, n_bplr, mxDOUBLE_CLASS, (mxComplexity)0);
	plhs[3] = mxCreateNumericMatrix(1, n_bplr, mxDOUBLE_CLASS, (mxComplexity)0);
	
	mxSetPr(plhs[0], bplr_lx);
	mxSetPr(plhs[1], bplr_rx);
	mxSetPr(plhs[2], bplr_ty);
	mxSetPr(plhs[3], bplr_dy);
}