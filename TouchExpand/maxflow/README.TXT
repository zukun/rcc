###################################################################
#                                                                 #
#    MAXFLOW - software for computing mincut/maxflow in a graph   #
#                        Version 2.2b                              #
#    http://www.cs.cornell.edu/People/vnk/software.html           #
#                                                                 #
#    Yuri Boykov (yuri@csd.uwo.ca)                                #
#    Vladimir Kolmogorov (vnk@cs.cornell.edu)                     #
#    2001                                                         #
#                                                                 #
###################################################################

1. Introduction.

This software library implements the maxflow algorithm
described in

	An Experimental Comparison of Min-Cut/Max-Flow Algorithms
	for Energy Minimization in Vision.
	Yuri Boykov and Vladimir Kolmogorov.
	In IEEE Transactions on Pattern Analysis and Machine Intelligence (PAMI), 
	September 2004

This algorithm was developed by Yuri Boykov and Vladimir Kolmogorov
at Siemens Corporate Research. To make it available for public use,
it was later reimplemented by Vladimir Kolmogorov based on open publications.

If you use this software for research purposes, you should cite
the aforementioned paper in any resulting publication.

Tested under windows, Visual C++ 6.0 compiler and unix (SunOS 5.8
and RedHat Linux 7.0, GNU c++ compiler).

##################################################################

2. License & disclaimer.

Definitions:

UCLB:   UCL Business PLC, a wholly owned subsidiary of University College London (UCL)
 with responsibility for transacting knowledge transfer activities on behalf of UCL.

Licensee: University Western Ontario
MaxFlow Software: The Kolmogorov / Boykov Max Flow software, version 2.2 which implements the algorithm described in:
V. Kolmogorov and Y. Boykov, An Experimental Comparison of Min-Cut/Max-Flow Algorithms for Energy Minimization in Vision
PAMI, 26(9):1124-1137, September 2004
 

UCLB grants to Licensee a royalty free, non-exclusive licence with the right to sub-license the MaxFlow Software when
and only when it has been incorporated into the Licensee’s TouchExpand software and is for research purposes only.


The MaxFlow Software is provided by the authors and copyright holder “AS IS” and any express of implied warranties, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose are disclaimed. In no event shall the authors or copyright holder be liable for any direct, indirect, incidental, special, exemplary or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.


Licensee acknowledges and agrees that any and all intellectual property rights subsisting in the 
MaxFlow Software shall be and remain the sole property of UCLB.

 

Any sub-licence shall include obligations on the sub-licensee which are no less onerous than the 
obligations on the Licensee in this Agreement.
 

The following copyright notice must be displayed on all distributed copies of the sublicensed software:

MaxFlow software (version 2.2) by Vladimir Kolmogorov and Yuri Boykov, © University College London

##################################################################

3. Graph representation.

There are two versions of the algorithm using different
graph representations (adjacency list and forward star).
The former one uses more than twice as much memory as the
latter one but is 10-20% faster.

Memory allocation (assuming that all capacities are 'short' - 2 bytes):

                 |   Nodes    |   Arcs
------------------------------------------
Adjacency list   | *24 bytes  | *14 bytes
Forward star     | *28 bytes  |  6 bytes

(* means that often it should be rounded up to be a multiple of 4
- some compilers (e.g. Visual C++) seem to round up elements
of arrays unless the are structures containing only char[].)

Note that arcs are always added in pairs - in forward and reverse directions.
Arcs between nodes and terminals (the source and the sink) are
not stored as arcs, but rather as a part of nodes.

The assumption for the forward star representation is that
the maximum number of arcs per node (except the source
and the sink) is much less than ARC_BLOCK_SIZE (1024 by default).

Both versions have the same interface.

##################################################################

4. Example usage.

This section shows how to use the library to compute
a minimum cut on the following graph:

		        SOURCE
		       /       \
		     1/         \2
		     /      3    \
		   node0 -----> node1
		     |   <-----   |
		     |      4     |
		     \            /
		     5\          /6
		       \        /
		          SINK

///////////////////////////////////////////////////

#include <stdio.h>
#include "graph.h"

void main()
{
	Graph::node_id nodes[2];
	Graph *g = new Graph();

	nodes[0] = g -> add_node();
	nodes[1] = g -> add_node();
	g -> set_tweights(nodes[0], 1, 5);
	g -> set_tweights(nodes[1], 2, 6);
	g -> add_edge(nodes[0], nodes[1], 3, 4);

	Graph::flowtype flow = g -> maxflow();

	printf("Flow = %d\n", flow);
	printf("Minimum cut:\n");
	if (g->what_segment(nodes[0]) == Graph::SOURCE)
		printf("node0 is in the SOURCE set\n");
	else
		printf("node0 is in the SINK set\n");
	if (g->what_segment(nodes[1]) == Graph::SOURCE)
		printf("node1 is in the SOURCE set\n");
	else
		printf("node1 is in the SINK set\n");

	delete g;
}

///////////////////////////////////////////////////
