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

void mexFunction(int nlhs, mxArray *plhs[], 
				 int nrhs, const mxArray *prhs[])
// rhs (input): 
// ptrs (2xn matrix)
// adj_mat (nxn matrix)
// neighbor_r (a scalar)
// min_neighbor_d (a scalar)
// lhs (output):
// four_neighbor_inds (1D cell)
{
 	if ( nrhs != 4 ) { 
		return;
	}
	
	// parameters
	const unsigned int n_ptr = (unsigned int)mxGetN(prhs[0]);
	const double neighbor_r = mxGetScalar(prhs[2]);
	const double min_neighbor_d = mxGetScalar(prhs[3]);
	const double PI = 2*acos(0.0);

	// output
	mwSize ndim[1];
	ndim[0] = n_ptr;
	plhs[0] = mxCreateCellArray(1, ndim);

	
	// distance between ptrs
	double* ptrs = mxGetPr(prhs[0]);
	double *ptr_d = (double*)mxCalloc(n_ptr*n_ptr, sizeof(double));
	for ( unsigned int i=0 ; i < n_ptr; i++) {
		double x1 = ptrs[2*i];
		double y1 = ptrs[2*i+1];
		unsigned int stride_i = n_ptr*i;
		ptr_d[stride_i + i] = 1e12;
		for ( unsigned int j=i+1 ; j < n_ptr ; j++) {
			double x2 = ptrs[2*j];
			double y2 = ptrs[2*j+1];
			double d = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
			unsigned int stride_j = n_ptr*j;

			ptr_d[stride_i + j] = d;
			ptr_d[stride_j + i] = d;
		}
	}

	// distance between neighbors
	mxLogical* adj_mat = mxGetLogicals(prhs[1]);
	vector<vector<double> > local_d(n_ptr);
	for (unsigned int i=0 ; i < n_ptr ; i++ ) {
		unsigned int stride_i = n_ptr*i;
		for ( unsigned int j=i+1 ; j < n_ptr ; j++) {
			if ( adj_mat[j+stride_i] ) {
				local_d[i].push_back(ptr_d[stride_i+j]);
				local_d[j].push_back(ptr_d[stride_i+j]);
			}
		}
	}

	// define neighbor
	vector<vector<unsigned int> > neighbor_inds(n_ptr);
	vector<vector<double> > neighbor_ptrs(n_ptr);
	vector<vector<double> > neighbor_d(n_ptr);
	for ( unsigned int i=0; i < n_ptr; i++) {
		
		double neighbor_thres_d(min_neighbor_d);
		if ( !local_d[i].empty() ) {
			neighbor_thres_d =
				max(*max_element(local_d[i].begin(), local_d[i].end())*neighbor_r, min_neighbor_d);
		}
		unsigned int stride_i = n_ptr*i;
		for ( unsigned int j=0 ; j < n_ptr ; j++) {
			if  ( ptr_d[stride_i + j] <= neighbor_thres_d ) {
				neighbor_inds[i].push_back(j);
				double x2 = ptrs[2*j];
				double y2 = ptrs[2*j+1];
				neighbor_ptrs[i].push_back(x2);
				neighbor_ptrs[i].push_back(y2);
				neighbor_d[i].push_back(ptr_d[stride_i+j]);
			}
		}
	}
		
	// pick four connected neighbors
	for ( unsigned int i=0 ; i < n_ptr ; i++) {
		double x1 = ptrs[2*i];
		double y1 = ptrs[2*i+1];
		if ( !neighbor_inds[i].empty() ) {
			size_t n_neighbor = neighbor_inds[i].size();
			vector<unsigned int> orientation(n_neighbor);
			for ( unsigned int j=0 ; j < n_neighbor; j++) {
				double offset_x = neighbor_ptrs[i][2*j] - x1;
				double offset_y = neighbor_ptrs[i][2*j+1] - y1;
				double angle = atan2(offset_y, offset_x);
				if ( angle < 0 ) angle += 2*PI;

				if ( angle >=0 && angle < PI/4) orientation[j] = 0;
				else if ( angle < 3*PI/4) orientation[j] = 1;
				else if ( angle < 5*PI/4) orientation[j] = 2;
				else if ( angle < 7*PI/4) orientation[j] = 3;
				else orientation[j] = 0;
			}
			
			vector<unsigned int> four_neighbor_inds;
			for (unsigned int c = 0; c < 4 ; c++) {
				double min_d(1e12);
				int min_idx(-1);
				for ( unsigned int j=0 ; j < n_neighbor ; j++) {
					if ( orientation[j] == c ) {
						if ( min_d > neighbor_d[i][j] ) {
							min_d = neighbor_d[i][j];
							min_idx = j;
						}
					}
				}
				if ( min_idx >= 0 ) {
					four_neighbor_inds.push_back(i);
					four_neighbor_inds.push_back(neighbor_inds[i][min_idx]);
				}
			
			}
			if ( ! four_neighbor_inds.empty() ) {
				size_t n_four_neighbors = four_neighbor_inds.size();
				mxArray* mx_four_neighbors = mxCreateDoubleMatrix(2, n_four_neighbors/2, (mxComplexity)0);
				double* four_neighbors = mxGetPr(mx_four_neighbors);
				for ( unsigned int j=0 ; j < n_four_neighbors; j++) {
					four_neighbors[j] = four_neighbor_inds[j] + 1; //one-based index
				}
				mxSetCell(plhs[0], i, mxDuplicateArray(mx_four_neighbors));
				mxDestroyArray(mx_four_neighbors);
			}
		}
	}
	mxFree(ptr_d);
}
	