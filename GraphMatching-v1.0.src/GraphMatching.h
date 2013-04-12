/*
    GraphMatching.h - interface to graph matching optimization code via dual decomposition.

    Copyright 2008 Vladimir Kolmogorov (vnk@adastral.ucl.ac.uk)

    This software can be used for research and evaluation purposes only. Commercial use is prohibited.
    Public redistribution of the code or its derivatives is prohibited.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef AFAJOSHDAJKSHFKASJDH
#define AFAJOSHDAJKSHFKASJDH

#include <assert.h>
#include <math.h>
#include "SubgradientDecomposition.h"


class GraphMatching
{
public:
	typedef double REAL;
	#define GM_INFTY ((REAL)1e100)

	typedef int NodeId; // feature id's in the left/right images
	typedef int AssignmentId; // potential correspondence between features


	// N0, N1 - # of features in the first/second image. A - # of assignments. E - # of edges between assignments.
	GraphMatching(int N0, int N1, int A_max, int E_max); 
	~GraphMatching();



	// first call returns 0, second returns 1, and so on. 
	// DUPLICATES ARE NOT ALLOWED!
	AssignmentId AddAssignment(NodeId i0, NodeId i1, REAL cost);
	// Parallel edges may be added (they are merged during the algorithm).
	void AddEdge(AssignmentId a, AssignmentId b, REAL cost);
	void AddEdge(AssignmentId a, AssignmentId b, REAL E00, REAL E01, REAL E10, REAL E11);



	// For local and tree subproblems you need to specify neighborhood structure for points in the left
	// and right images by calling AddNeighbors() (recommended) or ConstructNeib().
	void AddNeighbors(int r, NodeId i, NodeId j);
	// Determine neighbors automatically:
	//     option=0: make points p and q in the same image neighbors if there exists an edge between 
	//               some assignments <p,p'> and <q,q'>.
	//     option=1: assign each edge (a,b) between assignments either to the left or to the right image
	//               using a heuristic. Then similar to option=0. 
	// This function will be called automatically from AddLocalSubproblems() and AddTreeSubproblems()
	// if no neighbors were added by then.
	void ConstructNeighbors(int option);
	void ResetNeighbors(); // deletes all neighbors. Useful if you want to specify different neighborhoods for local and tree subproblems.



	// usage: add suproblems and call SolveDD(). Should be called AFTER all assignments and edges are specified.
	void AddLinearSubproblem(REAL weight = 1);
	void AddMaxflowSubproblem(REAL weight = 1);
	// for each point p, add all incident assignments for p and its K nearest neighbors.
	void AddLocalSubproblems(int K, REAL weight = 1);
	void AddTreeSubproblems(REAL weight = 1);



	// terminate after iter_max iterations, or if the gap between the cost and the lower bound becomes <= gap_threshold.
	// See SubgradientDecomposition::Options for more options.
	// Returns lower bound.
	double SolveDD(int iter_max, double gap_threshold); 

	// can be called after SolveDD().
	int GetLabel(AssignmentId a); // returns 0 or 1.
	int* GetSolution(); // alternative way to get solution. Returns pointer to array 'solution_left' of size N0.
	                    // solution_left[i] is a number in [0,N1-1) or to -1, if point i is occluded.
	                    // This array should not be modified.

	int GetN0() { return N0; }
	int GetN1() { return N1; }
	int GetA() { return A; }
	int GetE() { return E; }

	// solution_left must be an array of size N0 (e.g. returned by GetSolution())
	// NOTE: (1) if the solution does not satisfy the uniqueness constraint, then the cost will >= INFTY.
	//       (2) a constant is added to the energy, so that empty solution has cost 0
	double ComputeEnergy(NodeId* solution_left);

	void ResetSubproblems(); // sets all variables to zero

	struct Options
	{
		Options() : verbose(true), use_branch_and_bound(true) {}
		bool use_branch_and_bound; // for local subproblems
		bool verbose;
	} options;

	void Save(char* filename, char* comment=NULL);


	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

private:
	struct Node;
	struct Assignment;
	struct Edge;
	struct NodeNeib;


	struct Node
	{
		Assignment*	first;
		int			degree;

		NodeNeib*	first_neib[2];

		Assignment* match;
	};
	struct Assignment
	{
		Node*		i[2];
		Assignment*	next[2];
		Edge*		first[2];
		REAL		cost;
		int			flag;
	};
	struct Edge
	{
		Assignment*	head[2];
		Edge*		next[2];
		REAL		cost;
	};
	struct NodeNeib
	{
		Node*		head[2];
		NodeNeib*	next[2];
		REAL		weight;
		bool		flag;
	};

	int N0, N1, N_max, N[2], A, A_max, E, E_max;
	Node* nodes[2];
	Assignment* assignments;
	Edge* edges;

	Block<NodeNeib>* node_neibs;
	NodeNeib** node_neib_ptrs[2]; // arrays of size N0 and N1
	Node* node_neib_tails[2];

	int* solution_left_best; // N0
	REAL Ebest;

	SubgradientDecomposition* sd;
	double* theta0;

	char* buf;
	int buf_size;
	void RequestBufSize(int size_in_bytes);

	////////////////////////////////////////////////////////

	double ComputeEnergy();
	void InitDD();

	void MergeEdges();

	NodeNeib* _AddNeighbors(int r, NodeId i, NodeId j, bool do_not_allocate=false); // if a neighbor already exists, returns existing neighbor

	void ComputeNeibWeights();

	static double _UpperBoundCallbackFn(void* user_arg);

	void RegisterActiveAssignment(Assignment* a);

	////////////////////////////////////////////////////////
	static double _LinearCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g, void* user_arg);
	double LinearCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g);
	////////////////////////////////////////////////////////
	static double _MaxflowCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g, void* user_arg);
	double MaxflowCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g);
	////////////////////////////////////////////////////////
	struct LocalSubproblem;

	void AddLocalSubproblem(REAL weight); // add a if assignments[a].flag!=0
	static double _LocalCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g, void* user_arg);
	double LocalCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g);

	struct LocalLowerBoundData;
	//double GetLocalLowerBound(SubgradientDecomposition::Subproblem& s, int* solution0, int i, double* theta);
	double LocalBranchAndBoundCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g);
	////////////////////////////////////////////////////////
	struct TreeNode;
	Block<TreeNode>* tree_nodes;

	void AddTreeSubproblem(Node* root, REAL weight);
	static double _TreeCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g, void* user_arg);
	double TreeCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g);
	////////////////////////////////////////////////////////
};

inline GraphMatching::AssignmentId GraphMatching::AddAssignment(NodeId i0, NodeId i1, REAL cost)
{
	assert(i0>=0 && i0<N0);
	assert(i1>=0 && i1<N1);
	assert(A < A_max);
	Assignment* a = assignments + A;
	a->i[0] = nodes[0]+i0;
	a->i[1] = nodes[1]+i1;
	a->i[0]->degree ++;
	a->i[1]->degree ++;
	a->next[0] = a->i[0]->first;
	a->i[0]->first = a;
	a->next[1] = a->i[1]->first;
	a->i[1]->first = a;
	a->cost = cost;
	a->first[0] = a->first[1] = NULL;
	a->flag = 1;
	return A ++;
}

inline void GraphMatching::AddEdge(AssignmentId _a, AssignmentId _b, REAL cost)
{
	assert(_a>=0 && _a<A && _b>=0 && _b<A && _a!=_b);

	if (!cost) return;

	Assignment* a = assignments + _a;
	Assignment* b = assignments + _b;

	if (a->i[0] == b->i[0] || a->i[1] == b->i[1]) return;

	assert(E < E_max);

	Edge* e = &edges[E ++];
	e->head[0] = b;
	e->head[1] = a;
	e->next[0] = a->first[0];
	a->first[0] = e;
	e->next[1] = b->first[1];
	b->first[1] = e;

	e->cost = cost;
}

inline void GraphMatching::AddEdge(AssignmentId _a, AssignmentId _b, REAL E00, REAL E01, REAL E10, REAL E11)
{
	/*
		E = A B = A + 0   0   + 0 B-A + 0 0
		    C D       C-A C-A   0 B-A   0 A+D-B-C

	*/

	E01 -= E00;
	E10 -= E00;
	E11 -= E00;
	AddEdge(_a, _b, E11-E01-E10);
	assignments[_a].cost += E10;
	assignments[_b].cost += E01;
}

inline int GraphMatching::GetLabel(AssignmentId _a)
{
	assert(_a>=0 && _a<A);
	Assignment* a = assignments + _a;
	return (a->i[0]->match == a) ? 1 : 0;
}

inline int* GraphMatching::GetSolution() { return solution_left_best; }

inline void GraphMatching::RegisterActiveAssignment(Assignment* a)
{
	a->flag = 1;
	if (!a->i[0]->match && !a->i[1]->match) a->i[0]->match = a->i[1]->match = a;
}

#endif




