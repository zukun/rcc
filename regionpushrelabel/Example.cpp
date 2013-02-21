/////////////////////////////////////////////////////////////////////////////
// Filename: Example.h
// Author:   Sameh Khamis
//
// Description: Usage example
/////////////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;

#include "RegionPushRelabel.h"

typedef Array<                 // Each cell has 1 node
	Arc<0, 0, Offsets<1, 0> >,  // From node 0 in the cell to node 0 in the cell on the right
	Arc<0, 0, Offsets<-1, 0> >, // From node 0 in the cell to node 0 in the cell on the left
	Arc<0, 0, Offsets<0, 1> >,  // From node 0 in the cell to node 0 in the cell below
	Arc<0, 0, Offsets<0, -1> >  // From node 0 in the cell to node 0 in the cell above
> FourConnected;

typedef RegionPushRelabel<
	short, long,               // Capacity Type, Flow Type
	Layout<
		FourConnected,         // Layout is four-connected
		BlockDimensions<3, 3>  // Divide the graph into blocks of 3x3 cells each
	>,
	ThreadCount<2>             // Use two threads
> RegularGraph;

int main()
{
	// Our graph is 4x4, but the class will allocate 6x6 nodes
	// to have a graph of dimensions that divide the block dimensions
	// we provided
	long d[] = {4, 4};
	RegularGraph *g = new RegularGraph(d);
	
	// All the node ids will be remapped internally by the algorithm
	g->add_terminal_weights(0, 100, 0);    // Node (0, 0) is connected to source
	g->add_terminal_weights(15, 0, 100);    // and node (3, 3) to sink
	
	g->add_terminal_weights(0, 100, 0);  // Node 0 is connected to source
	g->add_terminal_weights(15, 0, 100); // and node 15 to sink

	g->add_edge(0, 1, 5, 0);             // Edge capacities from node 0 to 1 and vice versa
	g->add_edge(0, 4, 5, 0);             // etc.
	g->add_edge(1, 2, 1, 0);
	g->add_edge(1, 5, 5, 0);
	g->add_edge(2, 3, 5, 0);
	g->add_edge(2, 6, 5, 0);
	g->add_edge(3, 7, 5, 0);
	g->add_edge(4, 5, 5, 0);
	g->add_edge(4, 8, 5, 0);
	g->add_edge(5, 6, 2, 0);
	g->add_edge(5, 9, 5, 0);
	g->add_edge(6, 7, 5, 0);
	g->add_edge(6, 10, 5, 0);
	g->add_edge(7, 11, 5, 0);
	g->add_edge(8, 12, 5, 0);
	g->add_edge(9, 13, 5, 0);
	g->add_edge(10, 11, 5, 0);
	g->add_edge(10, 14, 5, 0);
	g->add_edge(11, 15, 5, 0);
	g->add_edge(12, 13, 5, 0);
	g->add_edge(13, 14, 3, 0);
	g->add_edge(14, 15, 5, 0);

	g->compute_maxflow();
	cout << "Flow = " << g->get_flow() << endl;
	
	for (int i = 0; i < 16; i++)
		cout << "Segment of node " << i << " = " << g->get_segment(i) << endl;
	
	delete g;
	cin.ignore();
	return 0;
}
