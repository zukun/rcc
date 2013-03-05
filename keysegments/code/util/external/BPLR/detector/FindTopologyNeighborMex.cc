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
// edges (2xn matrix)
// n_ptr (a scalar)
// topology_k (a scale)
// lhs (output):
// neighbor_inds (1D (1xn_ptr) cell)
// topology_dist (1D (1xn_ptr) cell)
{
 	if ( nrhs != 3 ) { 
		return;
	}
	
	// parameters
	const unsigned int n_ptr = (unsigned int)mxGetScalar(prhs[1]);
	const unsigned int n_edge = (unsigned int)mxGetN(prhs[0]);
	const unsigned int topology_k = (unsigned int)mxGetScalar(prhs[2]);

	// adj list
	double *edges = mxGetPr(prhs[0]);
	vector<vector<unsigned int> > adj_list(n_ptr);
	for ( unsigned int i=0 ; i < n_edge ; i++ ) {
		unsigned int p1 = (unsigned int)edges[2*i] - 1;
		unsigned int p2 = (unsigned int)edges[2*i+1] - 1;
		adj_list[p1].push_back(p2);
		adj_list[p2].push_back(p1);
	}

	// find neighbor
	mwSize ndim[1];
	ndim[0] = n_ptr;
	plhs[0] = mxCreateCellArray(1, ndim);
	plhs[1] = mxCreateCellArray(1, ndim);
	for ( unsigned int i =0; i < n_ptr; i++ ) {
		
		// initialize
		vector<unsigned int> neighbor;
		vector<unsigned int> prev_neighbor(n_ptr); //bit-vector
		vector<unsigned int> tD(n_ptr); //topology dist
		list<unsigned int> newly_neighbor_list;
		neighbor.push_back(i);
		prev_neighbor[i] = 1;
		newly_neighbor_list.push_back(i);
		tD[i] = 0;
		// search for neighbors up to topology_k
		for ( unsigned int j=1 ; j <= topology_k ; j++ ) {
			// for each newly neighbor --> add its neighbor
			list<unsigned int> newly_neighbor_list_next;
			for ( list<unsigned int>::iterator it = newly_neighbor_list.begin();
				it != newly_neighbor_list.end();
				it++) {
					for ( vector<unsigned int>::iterator it2 = adj_list[*it].begin();
						it2 != adj_list[*it].end(); it2++) {

						neighbor.push_back(*it2);
						if ( !prev_neighbor[*it2] ) {
							newly_neighbor_list_next.push_back(*it2);
							prev_neighbor[*it2] = 1;
							tD[*it2] = j;
						}
					}
			}
			if ( newly_neighbor_list_next.empty() ) break;

			newly_neighbor_list.swap(newly_neighbor_list_next);
		}

		// assign neighbor
		sort(neighbor.begin(), neighbor.end());
		vector<unsigned int>::iterator it_end = unique(neighbor.begin(), neighbor.end());
		neighbor.resize( it_end - neighbor.begin() );    
		unsigned int n_neighbor = neighbor.size();
		mxArray* mx_neighbor_inds = mxCreateDoubleMatrix(1, n_neighbor, (mxComplexity)0);
		mxArray* mx_topology_dist = mxCreateDoubleMatrix(1, n_neighbor, (mxComplexity)0);
		double* neighbor_inds = mxGetPr(mx_neighbor_inds);
		double* topology_dist = mxGetPr(mx_topology_dist);
		unsigned int n_cnt = 0;
		for ( vector<unsigned int>::iterator it = neighbor.begin() ; 
			it != neighbor.end() ; it++, n_cnt++) {
			neighbor_inds[n_cnt] = *it + 1; // retunr to 1-based index
			topology_dist[n_cnt] = tD[*it];
		}
		mxSetCell(plhs[0], i, mxDuplicateArray(mx_neighbor_inds));
		mxSetCell(plhs[1], i, mxDuplicateArray(mx_topology_dist));
		mxDestroyArray(mx_neighbor_inds);
		mxDestroyArray(mx_topology_dist);
	}
}