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


#include "TouchExpand.h"
#include <math.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////

void ErrorFunction(char *mes) {
	throw mes;
}

TouchExpand::TouchExpand(int dimensions[], NEIGHBORHOOD neigh, 
				 double smoothness, coeff_type *tLinks, coeff_type *nLinks): graph(NULL), stack(NULL)
{
	
	int i, j;

	for(i = 0; i < 3; i++)
		dims[i] = dimensions[i];
	gridSize = dims[0]*dims[1]*dims[2];
	index = new Graph::node_id[gridSize];


	if(!index) throw "Not enough memory for index grid allocation!";

	//initializing neighborhood system
	nNeighbors = 0;
	memset(neighShifts, 0, sizeof(int)*26*3);
	//setting 6-neighbors
	for(i = 0; i < 3; i++)
	{
		neighShifts[3*nNeighbors+i] = 1;
		shiftLengths[nNeighbors] = 1.0;
		nNeighbors++;
		neighShifts[3*nNeighbors+i] = -1;
		shiftLengths[nNeighbors] = 1.0;
        nNeighbors++;
	}
	
	if(neigh == NEIGH18 || neigh == NEIGH26) //setting 12-neighbors
	{
		for(i = 0; i < 3; i++)
			for(j = i+1; j < 3; j++)
			{
				neighShifts[3*nNeighbors+i] = 1;
				neighShifts[3*nNeighbors+j] = 1;
				shiftLengths[nNeighbors] = sqrt(2.0);
				nNeighbors++;
				neighShifts[3*nNeighbors+i] = -1;
				neighShifts[3*nNeighbors+j] = -1;
				shiftLengths[nNeighbors] = sqrt(2.0);
				nNeighbors++;
				neighShifts[3*nNeighbors+i] = 1;
				neighShifts[3*nNeighbors+j] = -1;
				shiftLengths[nNeighbors] = sqrt(2.0);
				nNeighbors++;
				neighShifts[3*nNeighbors+i] = -1;
				neighShifts[3*nNeighbors+j] = 1;
				shiftLengths[nNeighbors] = sqrt(2.0);
				nNeighbors++;
			}
	}

	if(neigh == NEIGH14 || neigh == NEIGH26) //setting 8-neighbors (corner neighbors)
	{
		for(i = 0; i < 8; i++)
		{
			neighShifts[3*nNeighbors] = i%2? 1 : -1;
			neighShifts[3*nNeighbors+1] = (i >> 1)%2? 1 : -1;
			neighShifts[3*nNeighbors+2] = (i >> 2)%2? 1 : -1;
			shiftLengths[nNeighbors] = sqrt(3.0);
			nNeighbors++;
		}
	}

	for(i = 0; i < nNeighbors; i++)
	{
		neighOffsets[i] = neighShifts[3*i]+neighShifts[3*i+1]*dims[0]+neighShifts[3*i+2]*dims[0]*dims[1];
	}

	for(i = 0; i < nNeighbors; i++)
		for(j = i+1; j < nNeighbors; j++)
			if(neighOffsets[i] == -neighOffsets[j])
			{
				revDirection[i] = j;
				revDirection[j] = i;
			}


	//initializing active graph
 	int nodeEstimate = 10*(dims[0]*dims[1]+dims[1]*dims[2]+dims[2]*dims[1]);
	int arcEstimate = nodeEstimate*nNeighbors/2;
	graph = new Graph(ErrorFunction);
	if(!graph) throw "Not enough memory for graph allocation";

	//allocating stack
	stackLimit = 4*(dims[0]*dims[1]+dims[1]*dims[2]+dims[2]*dims[1]);
	stack = new DilationSeed[stackLimit];
	if(!stack) throw "Not enough memory for stack allocation!";

	stackSize = 0;

	globalSmoothness = smoothness;

	int dist;
	for(i = 0; i < nNeighbors; i++)
	{
		dist = 0;
		for(j = 0; j < 3; j++)
			if(neighShifts[3*i+j])
				dist++;
		edgeLengthsInv[i] = 1.0/sqrt(double(dist));
	}

	nlinks = nLinks;
	tlinks = tLinks;
	nActivated = 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

TouchExpand::~TouchExpand()
{
	if(graph) delete graph;
	if(stack) delete stack;
	delete index;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void TouchExpand::ActivateNode(int pos[], bool dilate)
{
	int adr = ADR(pos);

	if(IS_ACTIVATED(index[adr])) return; //this node is already activated
	
	Graph::node_id thisNode = graph->add_node();
	index[adr] = thisNode;
	nActivated++;

	graph->set_tweights(thisNode, tlinks[adr], 0);

	int i,j, neighPos[3];
	Graph::node_id neighbor;
	for(i = 0; i < nNeighbors; i++)
	{
		for(j = 0; j < 3; j++)
            neighPos[j] = pos[j]+neighShifts[i*3+j];
		
		if(!ISVALID(neighPos)) continue;

		neighbor = index[ADR(neighPos)];
		if(neighbor == NODE_IN_STACK) continue;

		if(IS_ACTIVATED(neighbor))
		{
			coeff_type edgeWeight, reverseEdgeWeight;
			if(nlinks)	{
				edgeWeight = coeff_type(edgeLengthsInv[i]*double(nlinks[adr]));
				reverseEdgeWeight = coeff_type(edgeLengthsInv[i]*double(nlinks[ADR(neighPos)]));
			}
			else {
				edgeWeight = coeff_type(edgeLengthsInv[i]*globalSmoothness);
				reverseEdgeWeight = coeff_type(edgeLengthsInv[i]*globalSmoothness);
			}
			graph->add_edge(thisNode, neighbor, edgeWeight, reverseEdgeWeight);
		}
		else if(dilate)
		{			
			if(stackLimit == stackSize) //checking for stack overflow
			{
				DilationSeed *oldStack = stack;
				stack = new DilationSeed[2*stackLimit];
				if(!stack) throw "Not enough memory for stack allocation!";
				memcpy(stack, oldStack, sizeof(DilationSeed)*stackSize);
				stackLimit *= 2;
				delete oldStack;		
			}
			for(j = 0; j < 3; j++)
				stack[stackSize].pos[j] = neighPos[j];
			index[ADR(neighPos)] = NODE_IN_STACK;
			stackSize++;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void TouchExpand::Dilate(int dilationSize)
{
	int stackPos = 0, runTo = stackSize, pos[3];
	
	for(int i = 0; i < dilationSize; i++)
	{
		for(;stackPos < runTo; stackPos++)
		{
			pos[0] = stack[stackPos].pos[0];
			pos[1] = stack[stackPos].pos[1];
			pos[2] = stack[stackPos].pos[2];
			ActivateNode(pos, i < dilationSize-1);
		}
		runTo = stackSize;
	}
	stackSize = 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////

void TouchExpand::InitializeSegm(int coarseDims[], unsigned char *coarseSegmentation)
{
	double factor[3];
	int i, iter[3], j, k, adr, neighPos[3];
	
	for(i=0; i < 3; i++) factor[i] = double(coarseDims[i])/dims[i];

	i = 0;
	for(iter[2] = 0; iter[2] < dims[2]; iter[2]++)
		for(iter[1] = 0; iter[1] < dims[1]; iter[1]++)
			for(iter[0] = 0; iter[0] < dims[0]; iter[0]++, i++)
			{
				index[i] = coarseSegmentation[int(factor[0]*iter[0])+int(factor[1]*iter[1])*coarseDims[0]+
					int(factor[2]*iter[2])*coarseDims[0]*coarseDims[1]]? SOURCE_REGION : SINK_REGION;
			}
	
	
	for(iter[2] = 0; iter[2] < dims[2]; iter[2]++)
		for(iter[1] = 0; iter[1] < dims[1]; iter[1]++)
			for(iter[0] = 0; iter[0] < dims[0]; iter[0]++)	
			{
				adr = ADR(iter);
				
				if(index[adr] != SINK_REGION) continue;
				for(j = 0; j < nNeighbors; j++)
				{
					for(k = 0; k < 3; k++)
						neighPos[k] = iter[k]+neighShifts[3*j+k];
					if(!ISVALID(neighPos)) continue;
					if(index[adr+neighOffsets[j]] == SOURCE_REGION)
					{
						ActivateNode(iter, true);
                        break;						
					}
				}
			}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void TouchExpand::InitializeFull()
{
	int iter[3];
	for(int i = 0; i < dims[0]*dims[1]*dims[2]; i++)
		index[i] = SOURCE_REGION;
	
	for(iter[2] = 0; iter[2] < dims[2]; iter[2]++)
		for(iter[1] = 0; iter[1] < dims[1]; iter[1]++)
			for(iter[0] = 0; iter[0] < dims[0]; iter[0]++)
				ActivateNode(iter, false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

Graph::flowtype TouchExpand::MaxFlow(int dilationStep, FILE *echo)
{
	Graph::flowtype res;
	int iter[3], i, neighPos[3], j, k;
	Graph::node_id thisNode, neighborVal;
	bool done;

	CheckTLinks();
	
	for(;;)
	{
		if(echo) fputs("Dilating active band..\n", echo);
		Dilate(dilationStep);
		if(echo) fprintf(echo, "%d (out of %d) nodes activated (%lf percent)\n", nActivated, dims[0]*dims[1]*dims[2],
			100*double(nActivated)/double(dims[0]*dims[1]*dims[2]));
		if(echo) fputs("Recomputing flow..\n", echo);
		res = graph->maxflow();
		done = true;

		if(echo) fputs("Checking criterion..\n", echo);
		i = 0;
		for(iter[2] = 0; iter[2] < dims[2]; iter[2]++)
			for(iter[1] = 0; iter[1] < dims[1]; iter[1]++)
				for(iter[0] = 0; iter[0] < dims[0]; iter[0]++, i++)
				{
					thisNode = index[i];
					if(!IS_ACTIVATED(thisNode)) continue;

					bool tmp = graph->what_segment((Graph::node_id)thisNode) == graph->SINK;
					bool isSource = graph->what_segment((Graph::node_id)thisNode, graph->SOURCE) == graph->SOURCE;
					bool isSink = graph->what_segment((Graph::node_id)thisNode, graph->SINK) == graph->SINK;

					
					for(j = 0; j < nNeighbors; j++)
					{
						for(k = 0; k < 3; k++)
							neighPos[k] = iter[k]+neighShifts[3*j+k];
						if(!ISVALID(neighPos)) continue;
						neighborVal = index[ADR(neighPos)];

						if(neighborVal == SOURCE_REGION && !isSource ||
							neighborVal == SINK_REGION && !isSink)
						{
							AddToStack(neighPos);
							done = false;
						}
					}
				}
		if(done)
		{
			if(echo) fputs("Done.\n\n", echo);
			return res;
		}
		else 
		{
			if(echo) fputs("Criterion not fulfilled. Active band should be dilated.\n", echo);
		}
					
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////

void TouchExpand::GetResult(unsigned char *segmentation)
{
	int iter[3], i = 0;

	for(iter[2] = 0; iter[2] < dims[2]; iter[2]++)
		for(iter[1] = 0; iter[1] < dims[1]; iter[1]++)
			for(iter[0] = 0; iter[0] < dims[0]; iter[0]++, i++)
				segmentation[i] = IsSource(iter)? 1 : 0;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void TouchExpand::GetNodeState(unsigned char *nodestate)
{
	int iter[3], i = 0;

	for(iter[2] = 0; iter[2] < dims[2]; iter[2]++)
		for(iter[1] = 0; iter[1] < dims[1]; iter[1]++)
			for(iter[0] = 0; iter[0] < dims[0]; iter[0]++, i++)
			{
				Graph::node_id ind = index[i];
				if(ind == SOURCE_REGION)
					nodestate[i] = 0;
				else if(ind == SINK_REGION)
					nodestate[i] = 1;
				else if(ind == NODE_IN_STACK)
					nodestate[i] = 2;
				else if(graph->what_segment((Graph::node_id)ind, graph->SINK) == graph->SOURCE)
						nodestate[i] = 5;
				else if(graph->what_segment((Graph::node_id)ind, graph->SOURCE) == graph->SINK)
						nodestate[i] = 6;			
				else nodestate[i] = 7;
			}
}

//////////////////////////////
void TouchExpand::InitializeBand(int coarseDims[], unsigned char *coarseNodeState) //1 - source, 0 - sink
{
	double factor[3];
	int i, iter[3];
	unsigned char val;
	
	for(i=0; i < 3; i++) factor[i] = double(coarseDims[i])/dims[i];

	for(i = 0; i < dims[0]*dims[1]*dims[2]; i++)
		index[i] = SOURCE_REGION;

	i = 0;
	for(iter[2] = 0; iter[2] < dims[2]; iter[2]++)
		for(iter[1] = 0; iter[1] < dims[1]; iter[1]++)
			for(iter[0] = 0; iter[0] < dims[0]; iter[0]++, i++)
			{
				val = coarseNodeState[int(factor[0]*iter[0])+int(factor[1]*iter[1])*coarseDims[0]+
					int(factor[2]*iter[2])*coarseDims[0]*coarseDims[1]];
				if(val == 0)
					index[i] = SOURCE_REGION;
				else if(val == 1)
					index[i] = SINK_REGION;
				else
					ActivateNode(iter, false);
			}
}

//////////////////////////////
void TouchExpand::CheckTLinks()
{
	int iter[3], i = 0;

	if(!tlinks) return;

	for(iter[2] = 0; iter[2] < dims[2]; iter[2]++)
		for(iter[1] = 0; iter[1] < dims[1]; iter[1]++)
			for(iter[0] = 0; iter[0] < dims[0]; iter[0]++, i++)
			{
				if(tlinks[i] > 0 && index[i] == SINK_REGION ||
					tlinks[i] < 0 && index[i] == SOURCE_REGION)
						ActivateNode(iter, false);
			}
}

///////////////////////


template<class T>T *DownsampleArray(int dims[], T *array, int newdims[], int factor, T minval, T maxval)
{
	for(int j = 0; j < 3; j++)
		newdims[j] = int(ceil(double(dims[j])/factor));

	T *newarray = new T[newdims[0]*newdims[1]*newdims[2]];
	memset(newarray, 0, sizeof(T)*newdims[0]*newdims[1]*newdims[2]);

	int pos[3];
	int i = 0;

	for(pos[2] = 0; pos[2] < dims[2]; pos[2]++)
		for(pos[1] = 0; pos[1] < dims[1]; pos[1]++)
			for(pos[0] = 0; pos[0] < dims[0]; pos[0]++,i++)
			{
				int newadr = (pos[0]/factor) + (pos[1]/factor) * newdims[0] + (pos[2]/factor) *newdims[1]*newdims[0];
				
				double val = newarray[newadr];
				val += array[i];

				if(val > maxval) val = maxval;
				if(val < minval) val = minval;
				newarray[newadr] = T(val);
			}
	return newarray;
}

///////////////////////////////////////////////////////////

void TouchExpand::InitializeCoarseToFine(int nSteps, bool conservative, FILE *echo)
{
	coeff_type *nlinksSmall;
	coeff_type *tlinksSmall;
	int dimsSmall[3];
	int prevDims[3];
	unsigned char *prevSegm;

	TouchExpand *smallVolume;


	for(int i = nSteps; i > 0; i--)
	{
		if(echo)
			fprintf(echo, "----- Coarse level %d ------\n", i);
		tlinksSmall = DownsampleArray<coeff_type>(dims, tlinks, dimsSmall, 1 << i, coeff_type_min, coeff_type_max);
		if(nlinks)
		{
			nlinksSmall = DownsampleArray<coeff_type>(dims, nlinks, dimsSmall, 1 << i, coeff_type_min, coeff_type_max);
			for(int j = 0; j < dimsSmall[0]*dimsSmall[1]*dimsSmall[2]; j++)
				nlinksSmall[j] = __max(nlinksSmall[j]/(1 << i), 1);
		}
		else	nlinksSmall = NULL;

		smallVolume = new TouchExpand(dimsSmall, NEIGHBORHOOD(nNeighbors), globalSmoothness*(1 << (2*i)), tlinksSmall, nlinksSmall);
		if(i == nSteps)
			smallVolume->InitializeFull();
		else
		{
			if(i == nSteps-1 || conservative)
				smallVolume->InitializeSegm(prevDims, prevSegm);
			else
				smallVolume->InitializeBand(prevDims, prevSegm);
			delete[] prevSegm;
		}

		smallVolume->MaxFlow(1, echo);
		prevSegm = new unsigned char[dimsSmall[0]*dimsSmall[1]*dimsSmall[2]];
		if(i == nSteps || conservative)
			smallVolume->GetResult(prevSegm);
		else
			smallVolume->GetNodeState(prevSegm);

		memcpy(prevDims, dimsSmall, sizeof(int)*3);
		delete smallVolume;
		if(nlinksSmall)
			delete[] nlinksSmall;
		delete[] tlinksSmall;
	}

	if(nSteps <= 1 || conservative)
		InitializeSegm(prevDims, prevSegm);
	else
		InitializeBand(prevDims, prevSegm);
	delete prevSegm;
}