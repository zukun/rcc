#include <assert.h>
#include <math.h>
#include "mex.h"

#include <vector>
using namespace std;

// parameters
static const double INFCOST(1e40);

static double deformation_weight;
static double max_allowable_deform;
static double deformation_range;
static double ordering_penalty;
static double ordering_added_penalty;
static double ordering_range;
static double location_weight;
static double location_threshold;
static double no_match_cost1;
static double no_match_cost2;
static double match_distance_weight;

double computeOrderingCost(double *refptrdeform, double* matchptr, double* matchptr_prev, double refdeformsize)
{
	const double eps = 1e-10;

	// check if ordering constraint holds
	double matchptrdeform[2];
	double orderingcheck[2];
	for ( size_t ii=0 ; ii < 2 ; ii++ )
	{
		matchptrdeform[ii] = matchptr[ii] - matchptr_prev[ii];
		orderingcheck[ii] = matchptrdeform[ii]*refptrdeform[ii];
		if ( orderingcheck[ii] > eps ) 
		{
			//orderingcheck[ii] = -ordering_added_penalty;
			orderingcheck[ii] = -ordering_penalty;
		}
		else if ( orderingcheck[ii] < -eps )
		{
			//orderingcheck[ii] = ordering_added_penalty;
			orderingcheck[ii] = ordering_penalty;
		}
		else
		{
			orderingcheck[ii] = ordering_added_penalty;
		}

		if ( fabs(matchptrdeform[ii]) < eps && fabs(refptrdeform[ii]) < eps )
		{
			//orderingcheck[ii] -= ordering_added_penalty;
			orderingcheck[ii] -= ordering_added_penalty;
		}
	}
	double orderingcost(0.0);
	for ( size_t ii=0 ; ii < 2 ; ii++ )
	{
		if ( orderingcost < orderingcheck[ii] )
		{
			orderingcost = orderingcheck[ii];
		}
	}
	if ( refdeformsize >= ordering_range )
	{
		orderingcost = 0.9;
	}

	//orderingcost *= ordering_penalty;
	return orderingcost;
}

void computeDisparity(double* refptr, double* matchptr, double* disparity)
{
	for ( size_t ii=0 ; ii < 2 ; ii++ )
	{
		disparity[ii] = matchptr[ii] - refptr[ii];
	}
}

double computeDeformationCost(double *disparity, double *disparity_prev, double refdeformsize)
{

	double disparitydeform[2];
	for ( size_t ii=0 ; ii < 2 ; ii++ )
	{
		disparitydeform[ii] = disparity[ii] - disparity_prev[ii];
	}
	double d = sqrt(disparitydeform[0]*disparitydeform[0] + 
		            disparitydeform[1]*disparitydeform[1]);

	if ( refdeformsize <= deformation_range )
	{
        //d = d*deformation_weight;
		d = d <= max_allowable_deform ? d*deformation_weight : INFCOST;
		// d = d <= max_allowable_deform ? d*deformation_weight : max_allowable_deform*deformation_weight;
        //d = d <= max_allowable_deform ? d*deformation_weight : ordering_penalty;
        //d = d*deformation_weight;
	}
	else
	{
		double magnitude = refdeformsize/deformation_range;
        double min_magnitude = 1.0;
        double max_magnitude = sqrt(2)/deformation_range;
        double damping_factor = (-0.20/(max_magnitude-min_magnitude))*(magnitude-min_magnitude) + 1.0;
		//d = d*deformation_weight*damping_factor;
        //d = d*deformation_weight;
        d = d <= max_allowable_deform ? d*deformation_weight*damping_factor : INFCOST;
        //d = d <= max_allowable_deform ? d*deformation_weight*damping_factor : max_allowable_deform*deformation_weight*damping_factor;
        //d = d*deformation_weight*damping_factor;
		//d = d <= max_allowable_deform/magnitude ? d*deformation_weight : ordering_penalty;
	}
	return d;
}

double computeLocationCost(double *disparity)
{
	double d = sqrt(disparity[0]*disparity[0] + 
				    disparity[1]*disparity[1]); 
	double locationcost(0.0);
	if ( d > location_threshold )
	{
		locationcost = location_weight*d/location_threshold;
	}
	return locationcost;
}
/* mexFunction is the gateway routine for the MEX-file. */ 
void
mexFunction( int nlhs, mxArray *plhs[],
             int nrhs, const mxArray *prhs[] )
{
	if ( nlhs > 2 )
	{
		//cout << "usage: [matchcout matchmap] = mexStringMatch(refptrs, matchptrs, matchdists, nrefs, nmaxmatches, gridstep, dpparms)" << endl;
		return;
	}

	if ( nrhs !=7 )
	{
		//cout << "usage: [matchcout matchmap] = mexStringMatch(refptrs, matchptrs, matchdists, nrefs, nmaxmatches, gridstep, dpparms)" << endl;
		return;
	}

	double* refptrs = mxGetPr(prhs[0]);
	size_t total_num_of_cells = mxGetNumberOfElements(prhs[1]); 
	size_t nrefs = (size_t)mxGetScalar(prhs[3]);
	
	assert(total_num_of_cells == nrefs);
	if ( nrefs < 3 )
	{
		plhs[0] = mxCreateDoubleScalar(0.0);
		double *count = mxGetPr(plhs[0]);
		*count = 0;

		plhs[1] = mxCreateDoubleMatrix(0, 0, mxREAL);
		double *matchmap = mxGetPr(plhs[1]);

		return;
	}

	size_t nmatches = (size_t)mxGetScalar(prhs[4]);
	double gridstep = mxGetScalar(prhs[5]); 
	
	// parameter setting
	double *params = mxGetPr(prhs[6]);

	max_allowable_deform = params[0];
	deformation_weight = params[1];
	deformation_range = params[2];
	
	double ordering_weight1 = params[3];
	double ordering_weight2 = params[4];
	ordering_penalty = ordering_weight1*gridstep;
	ordering_added_penalty = ordering_weight2*gridstep;
	ordering_range = params[5];

	no_match_cost1 = params[6]*gridstep;
	no_match_cost2 = params[7]*gridstep;
	
	location_weight = params[8]*gridstep;
	location_threshold = params[9];	

	match_distance_weight = params[10]*gridstep;
	
	// memory allocation
	vector<vector<double> > cost(nrefs, vector<double>(nmatches+1, INFCOST));
	vector<vector<int> > bestpath(nrefs, vector<int>(nmatches+1, -1));
	vector<vector<double> > refptrs_for_no_match(nrefs, vector<double>(2,-1.0));
	vector<vector<double> > matchptrs_for_no_match(nrefs, vector<double>(2,-1.0));

	vector<vector<double> > prevrefptrs_for_no_match(nrefs, vector<double>(2,-1.0));
	vector<vector<double> > prevmatchptrs_for_no_match(nrefs, vector<double>(2,-1.0));

	vector<size_t> ncandids(nrefs, 0);

	// initialize
	for ( size_t i=0 ; i < nrefs ; i++ )
	{
		const mxArray *cell_element_ptr = mxGetCell(prhs[1], i);
		ncandids[i] = mxGetN(cell_element_ptr) + 1;
	}

	for ( size_t i=0 ; i < ncandids[0] ; i++)
	{
		cost[0][i] = 0.0;
	}
	for ( size_t i=0 ; i < ncandids[1] ; i++)
	{
		cost[1][i] = 0.0;
	}

	// start dynamic programming
	for	( size_t i = 2 ; i < nrefs ; i++ )
	{
		double refptr[2];
		double refptr_prev[2];
		double refptr_prevprev[2];

		double refptrdeform[2];
		for ( size_t ii=0 ; ii < 2 ; ii++ )
		{
			refptr[ii] = refptrs[2*i + ii];
			refptr_prev[ii] = refptrs[2*(i-1) + ii];
			refptr_prevprev[ii] = refptrs[2*(i-2) + ii];
			refptrdeform[ii] = refptr[ii] - refptr_prev[ii];
		}

		double refdeformsize = 
			sqrt(refptrdeform[0]*refptrdeform[0] + 
			     refptrdeform[1]*refptrdeform[1] );		
		
		double* matchptrs = mxGetPr(mxGetCell(prhs[1], i));
		double* matchptrs_prev = mxGetPr(mxGetCell(prhs[1], i-1));		
		double* matchptrs_prevprev = mxGetPr(mxGetCell(prhs[1], i-2));

		double* matchdists = mxGetPr(mxGetCell(prhs[2], i));
				
		size_t j(0), k(0);	
		for ( j = 0 ; j < ncandids[i]-1 ; j++ )
		{
			int best_k = -1;
            //double best_deformation = -1.0;
			double* matchptr =  &matchptrs[2*j];
			double* matchptr_prev;
			
			for ( k =0; k < ncandids[i-1]-1 ; k++)
			{
				double newcost(INFCOST);
                //double deformcost(0.0);
				if ( cost[i-1][k] < INFCOST ) 
				{
					matchptr_prev = &matchptrs_prev[2*k];
					
					// compute disparity
					double disparity[2];
					double disparity_prev[2];
					computeDisparity(refptr, matchptr, disparity);
					computeDisparity(refptr_prev, matchptr_prev, disparity_prev);

					// compute deformation cost
					double deformcost = 
						computeDeformationCost(disparity, disparity_prev, refdeformsize);

					// compute location cost
					double locationcost = computeLocationCost(disparity);

					// compute ordering cost
					double orderingcost = 
						computeOrderingCost(refptrdeform, matchptr, matchptr_prev, refdeformsize);	

					// compute matching cost
					double matchingdistancecost = 
						matchdists[j]*match_distance_weight;

					// cost update
					newcost = 
						cost[i-1][k] + deformcost + orderingcost + locationcost + matchingdistancecost;
				}
				if ( newcost <= cost[i][j] )
				{
					cost[i][j] = newcost;
                    //best_deformation = deformcost;
					best_k = k;
				}
			} // end for k
			
			k = ncandids[i-1]-1;
			double newcost(INFCOST);
            //double deformcost(0.0);
			//newcost = cost[i-1][k] + no_match_cost2;
			if ( cost[i-1][k] < INFCOST )
			{
				if ( matchptrs_for_no_match[i-1][0] == -1.0 ) // invalid points
				{
					newcost = cost[i-1][k] + no_match_cost2;
				}
				else
				{
					double refptr_prev_no_match[2];
					double refptrdeform_no_match[2];
					double matchptr_prev_no_match[2];

					double refptr_prevprev_no_match[2];
					double refptrdeform_prev_no_match[2];
					double matchptr_prevprev_no_match[2];
				
					for ( int ii=0 ; ii < 2 ; ii++ )
					{
						refptr_prev_no_match[ii] = refptrs_for_no_match[i-1][ii];
						refptrdeform_no_match[ii] = refptr[ii] - refptr_prev_no_match[ii];
						matchptr_prev_no_match[ii] = matchptrs_for_no_match[i-1][ii];

						refptr_prevprev_no_match[ii] = prevrefptrs_for_no_match[i-1][ii];
						refptrdeform_prev_no_match[ii] = refptr[ii] - refptr_prevprev_no_match[ii];
						matchptr_prevprev_no_match[ii] = prevmatchptrs_for_no_match[i-1][ii];
					}

					double d1 = sqrt(refptrdeform_no_match[0]*refptrdeform_no_match[0] + 
									 refptrdeform_no_match[1]*refptrdeform_no_match[1] );

					double d2 = sqrt(refptrdeform_prev_no_match[0]*refptrdeform_prev_no_match[0] + 
									 refptrdeform_prev_no_match[1]*refptrdeform_prev_no_match[1] );
									
					// compute disparity
					double disparity[2];
					double disparity_prev[2];
					double disparity_prevprev[2];

					computeDisparity(refptr, matchptr, disparity);
					computeDisparity(refptr_prev_no_match, matchptr_prev_no_match, disparity_prev);
					computeDisparity(refptr_prevprev_no_match, matchptr_prevprev_no_match, disparity_prevprev);

					// compute deformation cost
					double deformcost = 
						computeDeformationCost(disparity, disparity_prev ,d1);
					
					// compute location cost
					double locationcost = computeLocationCost(disparity);

					// compute ordering cost
					double orderingcost = 
						computeOrderingCost(refptrdeform_no_match, matchptr, matchptr_prev_no_match, d1);

					// compute matching cost
					double matchingdistancecost = 
						matchdists[j]*match_distance_weight;
					
// 					if ( matchptr_prevprev_no_match[0] != -1.0 )
// 					{
// 						double orderingcost_prev(0.0);
// 						orderingcost_prev =  
// 							computeOrderingCost(refptrdeform_prev_no_match, matchptr, matchptr_prevprev_no_match, d2);
// 						orderingcost += orderingcost_prev;
// 
// 						double deformcost_prev(0.0);
// 						deformcost_prev = 
// 							computeDeformationCost(disparity, disparity_prevprev, d2);
// 						if ( deformcost_prev == INFCOST )
// 							deformcost = INFCOST;
// 						else
// 						{
// 							deformcost = deformcost > deformcost_prev ? deformcost : deformcost_prev;
// 							//deformcost /= 2.0;
// 						}
// 					}
						
					newcost = cost[i-1][k] + no_match_cost1 + 
							  orderingcost + deformcost + locationcost + matchingdistancecost;				
				}
			}
			if ( newcost <= cost[i][j] )
			{
				cost[i][j] = newcost;
				best_k = k;
                //best_deformation = deformcost;
			}
			bestpath[i][j] = best_k;
		} // end for j
		
		j = ncandids[i]-1;
		int best_k(-1);
        //double best_deformation(-1.0);
		for ( k=0 ; k < ncandids[i-1]-1 ; k++ )
		{
			if ( cost[i-1][k] < INFCOST ) 
			{
				double newcost = cost[i-1][k] + no_match_cost2;
				if ( newcost <= cost[i][j] )
				{
					cost[i][j] = newcost;
					best_k = k;
                    //best_deformation = 0.0;
					for ( int ii=0 ; ii < 2 ; ii++ )
					{
						matchptrs_for_no_match[i][ii] = matchptrs_prev[2*k+ii];
						refptrs_for_no_match[i][ii] = refptr_prev[ii];
					}
					
					int prevbest_k = bestpath[i-1][best_k];
					if ( prevbest_k < ncandids[i-2]-1 )
					{
						for ( int ii=0 ; ii < 2 ; ii++ )
						{
							prevmatchptrs_for_no_match[i][ii] = matchptrs_prevprev[2*prevbest_k+ii];
							prevrefptrs_for_no_match[i][ii] = refptr_prevprev[ii];
						}
					}
					else
					{
						for ( int ii =0; ii < 2 ; ii++ )
						{
							prevmatchptrs_for_no_match[i][ii] = matchptrs_for_no_match[i-2][ii];
							prevrefptrs_for_no_match[i][ii] =  refptrs_for_no_match[i-2][ii];
						}
					}
				}
			}
		}
		k = ncandids[i-1]-1;
		double newcost = cost[i-1][k] + no_match_cost2 < INFCOST ? 
			             cost[i-1][k] + no_match_cost2 : INFCOST;			
		if ( newcost <= cost[i][j] )
		{
			cost[i][j] = newcost;
			best_k = k;
            //best_deformation = 0.0;
			for ( int ii=0 ; ii < 2 ; ii++ )
			{
				matchptrs_for_no_match[i][ii] = matchptrs_for_no_match[i-1][ii];
				refptrs_for_no_match[i][ii] = refptrs_for_no_match[i-1][ii];
			}

			int prevbest_k = bestpath[i-1][best_k];
			if ( prevbest_k < ncandids[i-2]-1 )
			{
				for ( int ii=0 ; ii < 2 ; ii++ )
				{
					prevmatchptrs_for_no_match[i][ii] = matchptrs_prevprev[2*prevbest_k+ii];
					prevrefptrs_for_no_match[i][ii] = refptr_prevprev[ii];
				}
			}
			else
			{
				for ( int ii =0; ii < 2 ; ii++ )
				{
					prevmatchptrs_for_no_match[i][ii] = matchptrs_for_no_match[i-2][ii];
					prevrefptrs_for_no_match[i][ii] =  refptrs_for_no_match[i-2][ii];
				}
			}
		}
		bestpath[i][j] = best_k;
//         if ( best_deformation > gridstep*1.25*deformation_weight )
//         {
//             mexErrMsgTxt("babo");
//         }
        
	} // end for i

	// backtrack
	vector<int> matchedpath(nrefs, -1);
	int bestidx(-1);
    //int secondbestidx(-1);
	double mincost(INFCOST);
	for ( size_t i=0 ; i < ncandids[nrefs-1] ; i++ )
	{
		if ( mincost > cost[nrefs-1][i] )
		{
			mincost = cost[nrefs-1][i];
			//secondbestidx = bestidx;
            bestidx = i;
             
		}
	}
	matchedpath[nrefs-1] = bestidx;

	for ( size_t i = nrefs-1 ; i > 1 ; i-- )
	{
		matchedpath[i-1] = bestpath[i][matchedpath[i]];
    }
	matchedpath[0] = ncandids[0]-1;

	// count the number of matches
	plhs[0] = mxCreateDoubleScalar(0.0);
	double *count = mxGetPr(plhs[0]);
	vector<int> matchedrefinds;
	for ( size_t i = 0 ; i < nrefs ; i++ )
	{
		if ( matchedpath[i] != ncandids[i]-1 )
		{
			matchedrefinds.push_back(i);
		}
	}
	*count = (double)matchedrefinds.size();

	// create matchmap
	plhs[1] = mxCreateDoubleMatrix(2, matchedrefinds.size(), mxREAL);
	double *matchmap = mxGetPr(plhs[1]);
	for ( size_t i=0 ; i < matchedrefinds.size() ; i++ )
	{
		matchmap[2*i] = matchedrefinds[i] + 1;
		matchmap[2*i+1] = matchedpath[matchedrefinds[i]] + 1;
	}
}

