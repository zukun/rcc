/*
Copyright 2008 
	Victor Lempitsky (victorlempitsky@gmail.com), 
	Yuri Boykov (yuri@csd.uwo.ca), 
	Vladimir Kolmogorov (vnk@adastral.ucl.ac.uk),
	The University of Western Ontario

 This software implements the touch-expand algorithm and demonstrates how it can be applied for shape-from-oriented points reconstruction.
The algorithm and its application were introduced in:

		Victor Lempitsky and Yuri Boykov.
		Global Optimization for Shape Fitting.
		In proceedings of IEEE COnference on Computer Vision and Pattern Recognition (CVPR),
		2007

This software also contains mincut/maxflow code by Vladimir Kolmogorov and Yuri Boykov (version 2.2), which follows
		
		An Experimental Comparison of Min-Cut/Max-Flow Algorithms
		for Energy Minimization in Vision.
		Yuri Boykov and Vladimir Kolmogorov.
		In IEEE Transactions on Pattern Analysis and Machine Intelligence (PAMI), 
		September 2004

If you use this software in your research, you should cite
the two aforementioned papers in any resulting publication.

For the licence, please see the README.TXT
The maxflow part of this software is covered by a special copyright and a special license described in the MAXFLOW\README.txt.
*/



#ifndef TOUCHEXPAND_H
#define TOUCHEXPAND_H

#include "maxflow\graph.h"
#include <memory.h>
#include <stdio.h>



//possible neighborhood systems for 3D grid
typedef enum {NEIGH6 = 6, NEIGH14 = 14, NEIGH18 = 18, NEIGH26 = 26} NEIGHBORHOOD;

//redefine here a type for tlink and nlink arrays. short is a recommended type.
//this type should be "smaller or equal" than the types used by maxflow (defined in maxflow\graph.h)
typedef short coeff_type;
const coeff_type coeff_type_min = -32768;
const coeff_type coeff_type_max = 32767;

//it is assumed that the real node pointers cannot have adresses 0x0004, 0x0005 and 0x0006
//this trick is used to save memory
const Graph::node_id SOURCE_REGION = Graph::node_id(4);
const Graph::node_id SINK_REGION = Graph::node_id(5);
const Graph::node_id NODE_IN_STACK = Graph::node_id(6);



//main class
/* example of usage
	.....
	coeff_type *tlinks;
	....//initialize tlinks voxel array from some process
	int dimensions[3];
	....//put the dimensions of the voxel grid into dimensions
	
	TouchExpand *te = new TouchExpand(dimensions, NEIGH6, 30, tLinks, NULL);
	te->InitializeCoarseToFine();
	te->MaxFlow();

	unsigned char *result = new unsigned char[dimensions[0]*dimensions[1]*dimensions[2]];
	te->GetResult(result);
	.....//output the result
	delete te;
*/
//see also FitShape.cpp for another example

class TouchExpand
{
public:
	//Constructor
	//dimensions = three grid dimensions (nx,ny,nz)
	//neigh = neighborhood system (6,14,18 or 26)
	//smoothness = the value used for all n-links in the graph (after division by the link length). Corresponds to uniform (Euclidean) regularization.
	//NB: smoothness is ignored if nLinks is non-NULL
	//tLinks = nx*ny*nz array of t-link values for each voxel. Negative values -> sink, positive values -> sourse.
	//nLinks = nx*ny*nz array of n-link values for each voxel. If non-null, the value is used for all outcoming n-links from the respective voxel node (after division by the link length).
	TouchExpand(int dimensions[], NEIGHBORHOOD neigh, double smoothness, coeff_type *tLinks, coeff_type *nLinks = NULL);
	
	//Destructor
	~TouchExpand();

	//The active band can be initialized with one of the 4 ways

	//InitializeFull - includes all nodes in the active band and run maxflow. This is equivalent to common mincut/maxflow.
	void InitializeFull();

	//InitializeSegm - initialization with the segmentation at a coarser resolution
	//Initialize to the thinnest possible band
	//coarseDims = three coarse grid dimensions (nx,ny,nz)
	//coarseSegmentation = coarse segmentation array of nx*ny*nz size. Values mean 1 - source, 0 - sink
	void InitializeSegm(int coarseDims[], unsigned char *coarseSegmentation); 

	//InitializeBand - initialization with the result of touch expand at a coarser resolution
	//coarseDims = three coarse grid dimensions (nx,ny,nz)
	//coarseSegmentation = coarse segmentation array of nx*ny*nz size. This array should be obtained by calling GetNodeState() at lower resoltion (see below).
	void InitializeBand(int coarseDims[], unsigned char *coarseNodeState);

	//InitializeCoarseToFine initializes via sequential calls to Touch-Expand at several coarser resolutions
	//at the coarsest level, IntializeFull is used
	//at the second coarsest level, InitializeSegm is used from the result of the coarsest level
	//at all next levels, InitializeSegm is called if "conservative" is true and InitializeBand otherwise
	//nSteps = number of coarser levels
	//conservative: "true" will result in a tigher band, but the algorithm may take many more iterations to converge
	//echo = output stream, NULL will result in no output
	void InitializeCoarseToFine(int nSteps = 2, bool conservative = true, FILE *echo = stdout);

	//MaxFlow runs the algorihm. Should be called after initialization.
	//dilationStep: values larger than 1 will result in faster band growth and faster convergence, but more memory will be consumed
	//echo = output stream, NULL will result in no output
	//returns the amount of maximum flow
	Graph::flowtype MaxFlow(int dilationStep = 2, FILE *echo = stdout);

	//GetResult returns an array of nx*ny*nz size with mincut result. Values mean 1 - source, 0 - sink.
    void GetResult(unsigned char *segmentation);
	//GetNodeState returns an array of nx*ny*nz size with the state of nodes at the convergence of the algorithm. Values mean:
	//0 - source component not activated, 1 - sink component not activated.
	//5 - source component activated, 6 - sink component activated, 7 - activated, connected to neither of the terminals (multiple minimal cuts found)
	void GetNodeState(unsigned char *nodestate);
	
	//GetN? return the respective dimenstions of the voxel grid
	int GetNX() { return dims[0]; }
	int GetNY() { return dims[1]; }
	int GetNZ() { return dims[2]; }

	//GetBandSize returns the number of nodes in the active band
	int GetBandSize() { return nActivated; }

private:
	int nActivated;
	
	inline int ADR(int pos[]) { 
		return (pos[0])+(pos[1])*dims[0]+(pos[2])*dims[0]*dims[1]; 
	}

	inline bool ISVALID(int pos[]) { 
		return ((pos)[0] >= 0 && (pos)[1] >= 0 && (pos)[2] >= 0 && (pos)[0] < dims[0] && (pos)[1] < dims[1] && (pos)[2] < dims[2] );
	}

	Graph *graph;
	
	int dims[3];
	int gridSize;
	Graph::node_id *index;

	coeff_type *nlinks;
	double globalSmoothness;
	double edgeLengthsInv[26];
	coeff_type *tlinks;

	int nNeighbors;
	int neighShifts[3*26];
	int neighOffsets[26];
	int revDirection[26];
	double shiftLengths[26];
	

	struct DilationSeed	{
		short pos[3];
	};
	int stackSize;
	int stackLimit;
	DilationSeed *stack;

	inline void ActivateNode(int pos[], bool dilate);
	void AddToStack(int pos[]);
    void Dilate(int dilationSize);
	void CheckTLinks();

	inline bool IsSource(int pos[]);
};


/////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool TouchExpand::IsSource(int pos[])
{
	Graph::node_id val = index[ADR(pos)];
	if(val == SOURCE_REGION) return true;
	if(val == SINK_REGION) return false;
	return graph->what_segment(val, graph->SINK) == graph->SOURCE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IS_ACTIVATED(Graph::node_id ptr)
{
	return ptr != NODE_IN_STACK && ptr != SOURCE_REGION && ptr != SINK_REGION;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TouchExpand::AddToStack(int pos[])
{
	int adr = ADR(pos);
	if(IS_ACTIVATED(index[adr]))
		return;
	if(stackLimit == stackSize) //checking for stack overflow
	{
		DilationSeed *oldStack = stack;
		stack = new DilationSeed[2*stackLimit];
		memcpy(stack, oldStack, sizeof(DilationSeed)*stackSize);
		stackLimit *= 2;
		delete oldStack;		
	}
	for(int j = 0; j < 3; j++)
		stack[stackSize].pos[j] = pos[j];
	index[adr] = NODE_IN_STACK;
	stackSize++;
}






#endif
