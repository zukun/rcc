#include <stdio.h>
#include <math.h>
#include "GraphMatching.h"
#include "MinCost/MinCost.h"
#include "QPBO/QPBO.h"

/*

// Specification of the file format.
// Angular parentheses mean that it should be replaced with an integer number,
// curly parentheses mean a floating point number.
// Point and assignment id's are integers starting from 0.


c comment line
p <N0> <N1> <A> <E>     // # points in the left image, # points in the right image, # assignments, # edges
a <a> <i0> <i1> {cost}  // specify assignment
e <a> <b> {cost}        // specify edge

i0 <id> {xi} {yi}       // optional - specify coordinate of a point in the left image
i1 <id> {xi} {yi}       // optional - specify coordinate of a point in the left image
n0 <i> <j>              // optional - specify that points <i> and <j> in the left image are neighbors
n1 <i> <j>              // optional - specify that points <i> and <j> in the right image are neighbors
*/

void GraphMatching::Save(char* filename, char* comment)
{
	int _a, r;
	Assignment* a;
	Edge* e;
	Node* i;
	NodeNeib* nb;

	FILE* fp = fopen(filename, "w");
	if (!fp) { printf("Can't open %s!\n", filename); exit(1); }

	if (comment) fprintf(fp, "c %s\n", comment);
	fprintf(fp, "p %d %d %d %d\n", N0, N1, A, E);
	for (a=assignments, _a=0; _a<A; a++, _a++) fprintf(fp, "a %d %d %d %f\n", _a, (int)(a->i[0]-nodes[0]), (int)(a->i[1]-nodes[1]), (double)a->cost);
	for (e=edges; e<edges+E; e++) fprintf(fp, "e %d %d %f\n", (int)(e->head[1]-assignments), (int)(e->head[0]-assignments), (double)e->cost);
	for (r=0; r<2; r++)
	for (i=nodes[r]; i<nodes[r]+N[r]; i++)
	{
		for (nb=i->first_neib[0]; nb; nb=nb->next[0]) fprintf(fp, "n%d %d %d\n", r, (int)(i-nodes[r]), (int)(nb->head[0]-nodes[r]));
	}
	fclose(fp);
}




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// 'mapping' must be of size 2*N. (array[N], ... array[2*N-1] is used as a temporary buffer).
// After the call  array[mapping[0]] <= array[mapping[1]] <= ... <= array[mapping[N-1]].
// array is not modified.
template <typename Type> inline void sort(Type* array, int* mapping, int N)
{
	// mergesort
	int i;
	int* mappingSrc = mapping;
	int* mappingDst = mapping + N;
	int* pSrc1;
	int* pSrc2;
	int* pSrc1End;
	int* pSrc2End;
	int* pDst;

	for (i=0; i<(N&(~1)); i+=2)
	{
		if (array[i] < array[i+1])
		{
			mappingSrc[i]   = i;
			mappingSrc[i+1] = i+1;
		}
		else
		{
			mappingSrc[i]   = i+1;
			mappingSrc[i+1] = i;
		}
	}
	if (i != N) mappingSrc[i] = i;

	int step;
	for (step=2; step<N; step*=2)
	{
		pSrc2End = mappingSrc;
		pDst = mappingDst;
		while ( 1 )
		{
			pSrc1 = pSrc2End;
			pSrc1End = pSrc1 + step;
			if (pSrc1End >= mappingSrc + N)
			{
				memcpy(pDst, pSrc1, (int)((char*)(mappingSrc + N) - (char*)pSrc1));
				break;
			}
			pSrc2 = pSrc1End;
			pSrc2End = pSrc2 + step;
			if (pSrc2End > mappingSrc + N) pSrc2End = mappingSrc + N;
			while ( 1 )
			{
				if (array[*pSrc1] < array[*pSrc2])
				{
					*pDst ++ = *pSrc1 ++;
					if (pSrc1 == pSrc1End)
					{
						memcpy(pDst, pSrc2, (int)((char*)pSrc2End - (char*)pSrc2));
						pDst = (int*) ((char*)pDst + (int)((char*)pSrc2End - (char*)pSrc2));
						break;
					}
				}
				else
				{
					*pDst ++ = *pSrc2 ++;
					if (pSrc2 == pSrc2End)
					{
						memcpy(pDst, pSrc1, (int)((char*)pSrc1End - (char*)pSrc1));
						pDst = (int*) ((char*)pDst + (int)((char*)pSrc1End - (char*)pSrc1));
						break;
					}
				}
			}
		}
		pDst = mappingDst;
		mappingDst = mappingSrc;
		mappingSrc = pDst;
	}
	if (mappingSrc != mapping) memcpy(mapping, mappingSrc, N*sizeof(int));
}

//////////////////////////////////////////////////////////////////////////
/////////////////////// LINEAR SUBPROBLEM ////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct LinearSubproblem
{
	MinCost<int,double>* m;
	double* theta_old;
	int counter;
};

void GraphMatching::AddLinearSubproblem(REAL weight)
{
	if (!sd) InitDD();

	int a;
	SubgradientDecomposition::Subproblem s;
	s.weight = weight;
	s.F = _LinearCallbackFn;
	s.var_num = A;
	s.vars = new double[s.var_num];
	memset(s.vars, 0, s.var_num*sizeof(double));
	s.array = new int[s.var_num];
	for (a=0; a<A; a++) s.array[a] = a;
	LinearSubproblem* ls = new LinearSubproblem;
	ls->m = NULL;
	ls->theta_old = new double[A];
	ls->counter = 0;
	s.user_ptr = ls;
	sd->AddSubproblem(s);
}

double GraphMatching::_LinearCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g, void* user_arg)
{
	return ((GraphMatching*)user_arg)->LinearCallbackFn(s, x, g);
}

double GraphMatching::LinearCallbackFn(SubgradientDecomposition::Subproblem& s, double* theta, double* g)
{
	int a, i0, i1;
	LinearSubproblem* ls = (LinearSubproblem*)s.user_ptr;

	if (!ls->m)
	{
		ls->m = new MinCost<int,REAL>(N0+N1+1, A+N0+N1);
		for (a=0; a<A; a++) 
		{
			i0 = 1+(int)(assignments[a].i[0]-nodes[0]);
			i1 = 1+N0+(int)(assignments[a].i[1]-nodes[1]);
			ls->m->AddEdge(i0, i1, 1, 0, theta[a]);
		}
		for (i0=0; i0<N0; i0++) ls->m->AddEdge(0, 1+i0, 1, 0, 0);
		for (i1=0; i1<N1; i1++) ls->m->AddEdge(1+N0+i1, 0, 1, 0, 0);
	}
	else
	{
		for (a=0; a<A; a++)
		{
			ls->m->UpdateCost(a, 1, theta[a] - ls->theta_old[a]);
		}
	}

	double Emincost = ls->m->Solve();
	for (a=0; a<A; a++) 
	{
		if (ls->m->GetRCap(a) == 0)
		{
			g[a] = s.weight;
			RegisterActiveAssignment(assignments + a);
		}
		else g[a] = 0;
	}

	if ( ls->counter > 30 )
	{
		delete ls->m;
		ls->m = NULL;
		ls->counter = 0;
	}
	else
	{
		memcpy(ls->theta_old, theta, A*sizeof(double));
		ls->counter ++;
	}
	return s.weight*Emincost;
}

//////////////////////////////////////////////////////////////////////////
/////////////////////// MAXFLOW SUBPROBLEM ///////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct MaxflowSubproblem
{
	QPBO<double>* q;
	double* theta_old;
	int counter;
};

void GraphMatching::AddMaxflowSubproblem(REAL weight)
{
	if (!sd) InitDD();

	int a;
	SubgradientDecomposition::Subproblem s;
	s.weight = weight;
	s.F = _MaxflowCallbackFn;
	s.var_num = A+E;
	s.vars = new double[s.var_num];
	memset(s.vars, 0, s.var_num*sizeof(double));
	s.array = new int[s.var_num];
	for (a=0; a<A+E; a++) s.array[a] = a;
	MaxflowSubproblem* ms = new MaxflowSubproblem;
	ms->q = NULL;
	ms->theta_old = new double[A+E];
	ms->counter = 0;
	s.user_ptr = ms;
	sd->AddSubproblem(s);
}

double GraphMatching::_MaxflowCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g, void* user_arg)
{
	return ((GraphMatching*)user_arg)->MaxflowCallbackFn(s, x, g);
}

double GraphMatching::MaxflowCallbackFn(SubgradientDecomposition::Subproblem& s, double* theta, double* g)
{
	int a, b, _e;
	Edge* e;
	MaxflowSubproblem* ms = (MaxflowSubproblem*)s.user_ptr;

	if (!ms->q)
	{
		ms->q = new QPBO<double>(A, E);
		ms->q->AddNode(A);
		for (a=0; a<A; a++)
		{
			ms->q->AddUnaryTerm(a, 0, theta[a]);
		}
		for (_e=0, e=edges; _e<E; _e++, e++)
		{
			a = (int)(e->head[1]-assignments); b = (int)(e->head[0]-assignments);
			ms->q->AddPairwiseTerm(a, b, 0, 0, 0, theta[A+_e]);
		}
	}
	else
	{
		for (a=0; a<A; a++)
		{
			ms->q->AddUnaryTerm(a, 0, theta[a]-ms->theta_old[a]);
		}
		for (_e=0, e=edges; _e<E; _e++, e++)
		{
			a = (int)(e->head[1]-assignments); b = (int)(e->head[0]-assignments);
			ms->q->AddPairwiseTerm(_e, a, b, 0, 0, 0, theta[A+_e]-ms->theta_old[A+_e]);
		}
	}
	ms->q->Solve();
	double Emaxflow = ms->q->ComputeTwiceLowerBound()/2;
	for (a=0; a<A; a++) 
	{
		int xa = ms->q->GetLabel(a);
		if (xa < 0)       *g++ = s.weight*0.5;
		else if (xa == 0) *g++ = 0;
		else
		{
			*g++ = s.weight*ms->q->GetLabel(a);
			RegisterActiveAssignment(assignments + a);
		}
	}
	for (_e=0, e=edges; _e<E; _e++, e++)
	{
		a = (int)(e->head[1]-assignments); b = (int)(e->head[0]-assignments);
		double xa = ms->q->GetLabel(a), xb = ms->q->GetLabel(b);
		if (xa < 0 && xb < 0) 
		{
			*g ++ = (theta[A+_e] < 0) ? (s.weight*0.5) : 0;
		}
		else
		{
			if (xa < 0) xa = 0.5; if (xb < 0) xb = 0.5;
			*g ++ = s.weight*xa*xb;
		}
	}

	if ( ms->counter > 30 )
	{
		delete ms->q;
		ms->q = NULL;
		ms->counter = 0;
	}
	else
	{
		memcpy(ms->theta_old, theta, (A+E)*sizeof(double));
		ms->counter ++;
	}
	return s.weight*Emaxflow;
}

//////////////////////////////////////////////////////////////////////////
/////////////////////// LOCAL SUBPROBLEMS ////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct GraphMatching::LocalSubproblem
{
	int N, M; // number of points in the two images
	int A; // number of assignments
	int* nodes0; // array of size N
	int* ends; // array of size A
	int* ptrs; // array of size A*A
};

void GraphMatching::AddLocalSubproblem(REAL weight)
{
	if (!sd) InitDD();

	Node* i;
	int r, a, b, e;
	LocalSubproblem* ls = new LocalSubproblem;

	RequestBufSize(A*sizeof(int));
	int* array = (int*) buf;

	SubgradientDecomposition::Subproblem s;
	s.weight = weight;
	s.F = _LocalCallbackFn;
	s.user_ptr = ls;
	s.var_num = 0;
	for (i=nodes[0]; i<nodes[0]+N0+N1; i++) i->match = NULL;
	for (a=0; a<A; a++)
	{
		if (assignments[a].flag <= 0) { array[a] = -1; continue; }
		array[a] = 0;
		assignments[a].i[0]->match = assignments[a].i[1]->match = assignments + a;
		s.var_num ++;
	}
	ls->A = s.var_num;
	for (e=0; e<E; e++)
	{
		a = (int)(edges[e].head[1]-assignments); b = (int)(edges[e].head[0]-assignments);
		if (array[a]>=0 && array[b]>=0) s.var_num ++;
	}
	int _N[2] = { 0, 0 };
	for (r=0; r<2; r++) for (i=nodes[r]; i<nodes[r]+N[r]; i++) if (i->match) _N[r]++;
	r = (_N[0] < _N[1]) ? 0 : 1;
	ls->N = _N[r];

	ls->M = 0;
	int* nodes1 = new int[N[1-r]];
	for (i=nodes[1-r]; i<nodes[1-r]+N[1-r]; i++) if (i->match) nodes1[(int)(i-nodes[1-r])] = ls->M++;

	s.vars = new double[s.var_num];
	memset(s.vars, 0, s.var_num*sizeof(double));
	s.array = new int[s.var_num];
	ls->nodes0 = new int[ls->N + 1 + ls->A + ls->A*ls->A];
	ls->ends = ls->nodes0 + ls->N + 1;
	ls->ptrs = ls->ends + ls->A;
	memset(ls->nodes0, 0, (ls->N + 1 + ls->A + ls->A*ls->A)*sizeof(int));

	s.var_num = 0;
	ls->N = 0;
	for (i=nodes[r]; i<nodes[r]+N[r]; i++)
	{
		if (!i->match) continue;
		ls->nodes0[ls->N++] = s.var_num;
		Assignment* a;
		for (a=i->first; a; a=a->next[r])
		{
			if (array[(int)(a-assignments)] < 0) continue;
			ls->ends[s.var_num] = nodes1[(int)(a->i[1-r]-nodes[1-r])];
			s.array[s.var_num] = (int)(a-assignments);
			array[(int)(a-assignments)] = s.var_num ++;
		}
	}
	ls->nodes0[ls->N] = s.var_num;

	delete [] nodes1;

	for (e=0; e<E; e++)
	{
		a = (int)(edges[e].head[1]-assignments); b = (int)(edges[e].head[0]-assignments);
		if (array[a]>=0 && array[b]>=0)
		{
			s.array[s.var_num] = A+e;
			a = array[a]; b = array[b];
			ls->ptrs[ls->A*a + b] = ls->ptrs[ls->A*b + a] = s.var_num ++;
		}
	}

	RequestBufSize((2*ls->N+ls->M)*sizeof(int) + (ls->N+1+ls->A)*sizeof(double));

	sd->AddSubproblem(s);
}

double GraphMatching::_LocalCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g, void* user_arg)
{
	if (((GraphMatching*)user_arg)->options.use_branch_and_bound)
		return ((GraphMatching*)user_arg)->LocalBranchAndBoundCallbackFn(s, x, g);
	else
		return ((GraphMatching*)user_arg)->LocalCallbackFn(s, x, g);
}

double GraphMatching::LocalCallbackFn(SubgradientDecomposition::Subproblem& s, double* theta, double* g)
{
	LocalSubproblem* ls = (LocalSubproblem*)s.user_ptr;

	int i, j, a, b, N0 = ls->N;
	int* solution0_best = (int*)buf;
	int* solution0 = solution0_best+N0;
	int* nodes1 = solution0+N0;
	double* costs0 = ((double*)(nodes1+ls->M)) + 1;
	costs0[-1] = 0;
	memset(nodes1, 0, ls->M*sizeof(int));
	double cost_best = 0;
	for (i=0; i<N0; i++) solution0_best[i] = -1;

	i = 0;
	while ( 1 )
	{
		if (i < N0)
		{
			solution0[i] = -1;
			costs0[i] = costs0[i-1];
			i ++;
			continue;
		}
		if (cost_best > costs0[i-1])
		{
			cost_best = costs0[i-1];
			memcpy(solution0_best, solution0, N0*sizeof(int));
		}

		// backtrack
		for (i--; i>=0; i--)
		{
			if (solution0[i]>=0)
			{
				nodes1[ls->ends[solution0[i]]] = 0;
				a = solution0[i] + 1;
			}
			else a = ls->nodes0[i];
			for ( ; a<ls->nodes0[i+1]; a++)
			{
				if (nodes1[ls->ends[a]] == 0) break;
			}
			if (a<ls->nodes0[i+1]) break;
		}
		if (i < 0) break;
		solution0[i] = a;
		nodes1[ls->ends[a]] = 1;
		costs0[i] = costs0[i-1] + theta[a];
		int* ptr = ls->ptrs + a*ls->A;
		for (j=0; j<i; j++) 
		{
			b = solution0[j];
			if (b >= 0 && ptr[b]) costs0[i] += theta[ptr[b]];
		}
		i ++;
	}

	memset(g, 0, s.var_num*sizeof(double));
	for (i=0; i<N0; i++)
	{
		a = solution0_best[i];
		if (a < 0) continue;
		RegisterActiveAssignment(assignments + s.array[a]);
		g[a] = s.weight;
		int* ptr = ls->ptrs + a*ls->A;
		for (j=0; j<i; j++)
		{
			b = solution0_best[j];
			if (b >= 0 && ptr[b]) g[ptr[b]] = s.weight;
		}
	}

	return s.weight*cost_best;
}

struct GraphMatching::LocalLowerBoundData
{
	LocalLowerBoundData(SubgradientDecomposition::Subproblem& _s, double* _theta, double* _D_min) : s(_s), theta(_theta), D_min(_D_min)
	{
		int i, j, a, b;
		ls = (LocalSubproblem*)s.user_ptr;
		A = ls->A; N = ls->N; M = ls->M;
		for (i=0; i<N; i++)
		{
			for (a=ls->nodes0[i]; a<ls->nodes0[i+1]; a++)
			{
				D_min[a] = theta[a];
				for (j=i+1; j<N; j++)
				{
					double cost_min = 0;
					for (b=ls->nodes0[j]; b<ls->nodes0[j+1]; b++)
					{
						if (ls->ptrs[A*a + b])
						{
							double cost = theta[ls->ptrs[A*a + b]];
							if (cost_min > cost) cost_min = cost;
						}
					}
					D_min[a] += cost_min;
				}
			}
		}
	}
	~LocalLowerBoundData() {}
	double GetLowerBound(int i0, int* solution0)
	{
		int i, j, a, b;
		double lower_bound = 0;
		for (j=i0; j<N; j++)
		{
			double cost_min = 0;
			for (b=ls->nodes0[j]; b<ls->nodes0[j+1]; b++)
			{
				double cost = D_min[b];
				for (i=0; i<i0; i++)
				{
					a = solution0[i];
					if (a >= 0 && ls->ptrs[A*a + b]) cost += theta[ls->ptrs[A*a + b]];
				}
				if (cost_min > cost) cost_min = cost;
			}
			lower_bound += cost_min;
		}
		return lower_bound;
	}
private:

	SubgradientDecomposition::Subproblem& s;
	LocalSubproblem* ls;
	int A, N, M;
	double* theta;
	double* D_min;
};

double GraphMatching::LocalBranchAndBoundCallbackFn(SubgradientDecomposition::Subproblem& s, double* theta, double* g)
{
	LocalSubproblem* ls = (LocalSubproblem*)s.user_ptr;

	int i, j, a, b, N0 = ls->N;
	int* solution0_best = (int*)buf;
	int* solution0 = solution0_best+N0;
	int* nodes1 = solution0+N0;
	double* costs0 = ((double*)(nodes1+ls->M)) + 1;
	double* D_min = costs0 + N0;
	costs0[-1] = 0;
	memset(nodes1, 0, ls->M*sizeof(int));
	double cost_best = 0;
	for (i=0; i<N0; i++) solution0_best[i] = -1;

	LocalLowerBoundData D(s, theta, D_min);

	////////////////////////////////////////////////////////////////////////////
	/////////////////  two ICMs - in forward and backward order  ///////////////
	////////////////////////////////////////////////////////////////////////////
	if ( 1 )
	{
		// forward
		double cost_ICM = 0;
		for (i=0; i<N0; i++)
		{
			double cost_min = 0;
			solution0[i] = -1;
			for (a=ls->nodes0[i]; a<ls->nodes0[i+1]; a++)
			{
				if (nodes1[ls->ends[a]]) continue;
				double cost = theta[a];
				int* ptr = ls->ptrs + a*ls->A;
				for (j=0; j<i; j++)
				{
					b = solution0[j];
					if (b >= 0 && ptr[b]) cost += theta[ptr[b]];
				}
				if (cost_min > cost) { cost_min = cost; solution0[i] = a; }
			}
			cost_ICM += cost_min;
			nodes1[ls->ends[solution0[i]]] = 1;
		}
		memset(nodes1, 0, ls->M*sizeof(int));
		if (cost_best > cost_ICM) { cost_best = cost_ICM; memcpy(solution0_best, solution0, N0*sizeof(int)); }

		// backward
		cost_ICM = 0;
		for (i=N0-1; i>=0; i--)
		{
			double cost_min = 0;
			solution0[i] = -1;
			for (a=ls->nodes0[i]; a<ls->nodes0[i+1]; a++)
			{
				if (nodes1[ls->ends[a]]) continue;
				double cost = theta[a];
				int* ptr = ls->ptrs + a*ls->A;
				for (j=N0-1; j>i; j--)
				{
					b = solution0[j];
					if (b >= 0 && ptr[b]) cost += theta[ptr[b]];
				}
				if (cost_min > cost) { cost_min = cost; solution0[i] = a; }
			}
			cost_ICM += cost_min;
			nodes1[ls->ends[solution0[i]]] = 1;
		}
		memset(nodes1, 0, ls->M*sizeof(int));
		if (cost_best > cost_ICM) { cost_best = cost_ICM; memcpy(solution0_best, solution0, N0*sizeof(int)); }
	}

	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	i = 0;
	while ( 1 )
	{
		if (costs0[i-1] + D.GetLowerBound(i, solution0) < cost_best)
		{
			if (i < N0)
			{
				solution0[i] = -1;
				costs0[i] = costs0[i-1];
				i ++;
				continue;
			}
			if (cost_best > costs0[i-1])
			{
				cost_best = costs0[i-1];
				memcpy(solution0_best, solution0, N0*sizeof(int));
			}
		}

		// backtrack
		for (i--; i>=0; i--)
		{
			if (solution0[i]>=0)
			{
				nodes1[ls->ends[solution0[i]]] = 0;
				a = solution0[i] + 1;
			}
			else a = ls->nodes0[i];
			for ( ; a<ls->nodes0[i+1]; a++)
			{
				if (nodes1[ls->ends[a]] == 0) break;
			}
			if (a<ls->nodes0[i+1]) break;
		}
		if (i < 0) break;
		solution0[i] = a;
		nodes1[ls->ends[a]] = 1;
		costs0[i] = costs0[i-1] + theta[a];
		int* ptr = ls->ptrs + a*ls->A;
		for (j=0; j<i; j++) 
		{
			b = solution0[j];
			if (b >= 0 && ptr[b]) costs0[i] += theta[ptr[b]];
		}
		i ++;
	}

	memset(g, 0, s.var_num*sizeof(double));
	for (i=0; i<N0; i++)
	{
		a = solution0_best[i];
		if (a < 0) continue;
		RegisterActiveAssignment(assignments + s.array[a]);
		g[a] = s.weight;
		int* ptr = ls->ptrs + a*ls->A;
		for (j=0; j<i; j++)
		{
			b = solution0_best[j];
			if (b >= 0 && ptr[b]) g[ptr[b]] = s.weight;
		}
	}

	return s.weight*cost_best;
}

void GraphMatching::AddLocalSubproblems(int K, REAL weight)
{
	if (!sd) InitDD();
	bool empty_neighbors = false;
	if (!node_neibs) { empty_neighbors = true; ConstructNeighbors(1); }
	ComputeNeibWeights();

	if (K <= 0) return;

	Node* i;
	Node* j;
	Assignment* a;
	NodeNeib* nb;
	int r, q, num;

	for (a=assignments; a<assignments+A; a++) a->flag = -a->flag;

	int* array = new int[2*N_max];
	Node** neibs = new Node*[N_max];
	RequestBufSize(N_max*sizeof(double));

	for (r=0; r<2; r++)
	for (i=nodes[r]; i<nodes[r]+N[r]; i++)
	{
		num = 0;
		double* weights = (double*)buf;
		for (q=0; q<2; q++)
		for (nb=i->first_neib[q]; nb; nb=nb->next[q], num++)
		{
			weights[num] = -nb->weight;
			neibs[num] = nb->head[q];
		}
		sort<double>(weights, array, num);
		if (num > K) num = K;

		int _A = 0;
		if (options.verbose) printf("local subproblem in image %d: [%d", r, (int)(i-nodes[r]));
		for (a=i->first; a; a=a->next[r]) { a->flag = -a->flag; if (a->flag > 0) _A ++; }
		for (q=0; q<num; q++)
		{
			j = neibs[array[q]];
			if (options.verbose) printf(" %d", (int)(j-nodes[r]));
			for (a=j->first; a; a=a->next[r]) { a->flag = -a->flag; if (a->flag > 0) _A ++; }
		}
		if (options.verbose) printf("] %d assignments\n", _A);

		AddLocalSubproblem(weight);

		for (a=i->first; a; a=a->next[r]) a->flag = -a->flag;
		for (q=0; q<num; q++)
		{
			j = neibs[array[q]];
			for (a=j->first; a; a=a->next[r]) a->flag = -a->flag;
		}
	}

	for (a=assignments; a<assignments+A; a++) a->flag = -a->flag;

	delete [] neibs;
	delete [] array;
	if (empty_neighbors) ResetNeighbors();
}
				   
//////////////////////////////////////////////////////////////////////////
/////////////////////// TREE SUBPROBLEMS  ////////////////////////////////
//////////////////////////////////////////////////////////////////////////

struct GraphMatching::TreeNode
{
	Node*		i;
	TreeNode*	parent;
	TreeNode*	first_child;
	TreeNode*	sibling;
	int*		ptrs; // array of size i->degree*(1+parent->i->degree)
	int			back_array_ptr;
	int			k;
};

void GraphMatching::AddTreeSubproblem(Node* root, REAL weight)
{
	if (!sd) InitDD();

	TreeNode* i;
	TreeNode* j;
	NodeNeib* nb;
	Assignment* a;
	Assignment* b;
	Edge* e;
	int ptrs_num, back_array_ptr = 0, point_num = 1, q, r = (root < nodes[1]) ? 0 : 1;
	int* rev_mapping = new int[A];

	SubgradientDecomposition::Subproblem s;
	s.weight = weight;
	s.F = _TreeCallbackFn;

	i = tree_nodes->New();
	s.user_ptr = i;
	i->parent = i->first_child = i->sibling = NULL;
	i->i = root;
	s.var_num = ptrs_num = i->i->degree;
	i->i->match = (Assignment*)i;

	TreeNode* queue_first = i;
	TreeNode* queue_last = i;
	i->ptrs = NULL;

	while ( (i=queue_first) )
	{
		if (i->ptrs) queue_first = (TreeNode*) i->ptrs;
		else queue_first = queue_last = NULL;

		for (q=0; q<2; q++)
		for (nb=i->i->first_neib[q]; nb; nb=nb->next[q])
		{
			if (nb->flag || nb->head[q]->match) continue;
			nb->flag = true;
			j = tree_nodes->New();
			j->parent = i;
			j->sibling = i->first_child;
			i->first_child = j;
			j->first_child = NULL;
			j->i = nb->head[q];

			ptrs_num += j->i->degree*(1+i->i->degree);
			s.var_num += j->i->degree;
			point_num ++;
			j->back_array_ptr = back_array_ptr;
			back_array_ptr += i->i->degree;

			j->i->match = (Assignment*)j;
			if (queue_last) queue_last->ptrs = (int*)j;
			else            queue_first      = j;
			queue_last = j;
			j->ptrs = NULL;
		}
	}

	RequestBufSize(s.var_num*sizeof(double) + back_array_ptr*sizeof(int));

	for (e=edges; e<edges+E; e++)
	{
		if (!e->head[0]->i[r]->match) continue;
		if (!e->head[1]->i[r]->match) continue;
		i = (TreeNode*) e->head[0]->i[r]->match;
		j = (TreeNode*) e->head[1]->i[r]->match;
		if (i->parent == j || j->parent == i) s.var_num ++;
	}

	//////////////////////////////////////////////////////////////////
	i = (TreeNode*)s.user_ptr;
	i->back_array_ptr = back_array_ptr;
	int* ptrs = new int[ptrs_num];
	memset(ptrs, 0, ptrs_num*sizeof(int));
	s.vars = new double[s.var_num];
	memset(s.vars, 0, s.var_num*sizeof(double));
	s.array = new int[s.var_num];
	s.var_num = 0;
	while ( 1 )
	{
		i->ptrs = ptrs;
		int k;
		for (a=i->i->first, k=0; a; a=a->next[r], k++)
		{
			rev_mapping[(int)(a-assignments)] = k;
			i->ptrs[k] = s.var_num;
			s.array[s.var_num++] = (int)(a-assignments);
		}
		ptrs += i->i->degree;
		if (i->parent)
		{
			ptrs += i->i->degree*i->parent->i->degree;
			int* p = i->ptrs + i->i->degree;
			for (b=i->parent->i->first; b; b=b->next[r])
			for (a=i->i->first; a; a=a->next[r], p++)
			{
				if (a->i[1-r] == b->i[1-r]) *p = 1;
			}
		}

		if (i->first_child) i = i->first_child;
		else
		{
			while (i->parent && !i->sibling) i = i->parent;
			if (!i->parent) break;
			i = i->sibling;
		}
	}
	for (e=edges; e<edges+E; e++)
	{
		a = e->head[0];
		b = e->head[1];
		if (!a->i[r]->match) continue;
		if (!b->i[r]->match) continue;
		i = (TreeNode*) e->head[0]->i[r]->match;
		j = (TreeNode*) e->head[1]->i[r]->match;
		if (j->parent == i)
		{
			TreeNode* k = i; i = j; j = k;
			Assignment* c = a; a = b; b = c;
		}
		else if (i->parent != j) continue;
		i->ptrs[i->i->degree*(1 + rev_mapping[(int)(b-assignments)]) + rev_mapping[(int)(a-assignments)]] = s.var_num;
		s.array[s.var_num++] = (int)(e-edges) + A;
	}

	i = (TreeNode*)s.user_ptr;
	while ( 1 )
	{
		i->i->match = NULL;

		if (i->first_child) i = i->first_child;
		else
		{
			while (i->parent && !i->sibling) i = i->parent;
			if (!i->parent) break;
			i = i->sibling;
		}
	}

	delete [] rev_mapping;

	sd->AddSubproblem(s);
	if (options.verbose) printf("tree subproblem in image %d, %d points\n", r, point_num);

}

void GraphMatching::AddTreeSubproblems(REAL weight)
{
	if (!sd) InitDD();
	bool empty_neighbors = false;
	if (!node_neibs) { empty_neighbors = true; ConstructNeighbors(0); }

	int i, r, q;
	NodeNeib* nb;

	if (!tree_nodes) tree_nodes = new Block<TreeNode>(128);

	for (r=0; r<2; r++)
	{
		for (i=0; i<N[r]; i++)
		{
			nodes[r][i].match = NULL;
			for (q=0; q<2; q++)
			for (nb=nodes[r][i].first_neib[q]; nb; nb=nb->next[q])
			{
				nb->flag = false;
			}
		}
		for (i=0; i<N[r]; i++)
		{
			for (q=0; q<2; q++)
			for (nb=nodes[r][i].first_neib[q]; nb; nb=nb->next[q])
			{
				if (!nb->flag) AddTreeSubproblem(&nodes[r][i], weight);
			}
		}
	}

	if (empty_neighbors) ResetNeighbors();
}

double GraphMatching::_TreeCallbackFn(SubgradientDecomposition::Subproblem& s, double* x, double* g, void* user_arg)
{
	return ((GraphMatching*)user_arg)->TreeCallbackFn(s, x, g);
}

double GraphMatching::TreeCallbackFn(SubgradientDecomposition::Subproblem& s, double* theta, double* g)
{
	double cost = 0;
	TreeNode* i;

	double theta_bak[2] = { theta[0], theta[1] };

	i = (TreeNode*)s.user_ptr;
	int* back_ptrs = (int*)buf;
	double* messages = (double*)(back_ptrs+i->back_array_ptr);
	memset(messages+i->ptrs[0], 0, i->i->degree*sizeof(double));
	while ( 1 )
	{
		if (i->first_child)
		{
			i = i->first_child;
			memset(messages+i->ptrs[0], 0, i->i->degree*sizeof(double));
		}
		else
		{
			while ( 1 )
			{
				if (i->parent)
				{
					// pass message
					int ki, kj, Ki=i->i->degree, Kj = i->parent->i->degree;
					double* Di = messages + i->ptrs[0];
					double* Dj = messages + i->parent->ptrs[0];
					int* Dj_mapping = back_ptrs + i->back_array_ptr;
					int* V = i->ptrs + i->i->degree;

					int ki_min = -1;
					double Dj0 = 0;
					for (ki=0; ki<Ki; ki++)
					{
						Di[ki] += theta[i->ptrs[ki]];
						if (Dj0 > Di[ki]) { Dj0 = Di[ki]; ki_min = ki; }
					}
					i->k = ki_min;
					cost += Dj0;

					theta[0] = 0; theta[1] = GM_INFTY;

					for (kj=0; kj<Kj; kj++)
					{
						ki_min = -1;
						double Dj_min = 0;
						for (ki=0; ki<Ki; ki++)
						{
							double Dj = Di[ki] + theta[*V++];
							if (Dj_min > Dj) { Dj_min = Dj; ki_min = ki; }
						}
						(*Dj ++) += Dj_min - Dj0;
						*Dj_mapping ++ = ki_min;
					}

					theta[0] = theta_bak[0]; theta[1] = theta_bak[1];
				}
				else break;
				if (i->sibling) break;
				i = i->parent;
			}
			if (!i->parent) break;
			i = i->sibling;
			memset(messages+i->ptrs[0], 0, i->i->degree*sizeof(double));
		}
	}

	memset(g, 0, s.var_num*sizeof(double));

	i = (TreeNode*)s.user_ptr;

	i->k = -1;
	double Dj_min = 0;
	int kj;
	for (kj=0; kj<i->i->degree; kj++)
	{
		double Dj = messages[i->ptrs[kj]] + theta[i->ptrs[kj]];
		if (Dj_min > Dj) { Dj_min = Dj; i->k = kj; }
	}
	cost += Dj_min;

	while ( 1 )
	{
		if (i->k >= 0)
		{
			g[i->ptrs[i->k]] = s.weight;
			Assignment* a = assignments + s.array[i->ptrs[i->k]];
			RegisterActiveAssignment(a);
		}
		if (i->first_child)
		{
			TreeNode* j = i;
			if (j->k >= 0)
			{
				for (i=j->first_child; i; i=i->sibling)
				{
					int* Dj_mapping = back_ptrs + i->back_array_ptr;
					i->k = Dj_mapping[j->k];
					if (i->k >= 0)
					{
						int ind = i->ptrs[i->i->degree*(1 + j->k) + i->k];
						if (ind > 1) g[ind] = s.weight;
					}
				}
			}
			i = j->first_child;
		}
		else
		{
			while (i->parent && !i->sibling) i = i->parent;
			if (!i->parent) break;
			i = i->sibling;
		}
	}

	return cost*s.weight;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

GraphMatching::GraphMatching(int _N0, int _N1, int _A_max, int _E_max)
	: N0(_N0),
	  N1(_N1),
	  A(0),
	  A_max(_A_max),
	  E(0),
	  E_max(_E_max),
	  tree_nodes(NULL),
	  Ebest(0),
	  node_neibs(NULL),
	  sd(NULL),
	  theta0(NULL),
	  buf(NULL),
	  buf_size(0)
{
	int i;
	void* array = malloc((N0+N1)*sizeof(Node) + A_max*sizeof(Assignment) + E_max*sizeof(Edge) + N0*sizeof(int));
	memset(array, 0, (N0+N1)*sizeof(Node) + A_max*sizeof(Assignment) + E_max*sizeof(Edge) + N0*sizeof(int));
	nodes[0] = (Node*) array;
	nodes[1] = nodes[0] + N0;
	assignments = (Assignment*)(nodes[1] + N1);
	edges = (Edge*)(assignments+A_max);
	solution_left_best = (int*)(edges + E_max);
	for (i=0; i<N0; i++) solution_left_best[i] = -1;
	N[0] = N0;
	N[1] = N1;
	N_max = (N0 > N1) ? N0 : N1;
	node_neib_ptrs[0] = (NodeNeib**)malloc((N0+N1)*sizeof(NodeNeib*));
	node_neib_ptrs[1] = node_neib_ptrs[0]+N0;
	memset(node_neib_ptrs[0], 0, (N0+N1)*sizeof(NodeNeib*));
	node_neib_tails[0] = node_neib_tails[1] = NULL;

}

GraphMatching::~GraphMatching()
{
	if (sd)
	{
		SubgradientDecomposition::Subproblem* s;
		for (s=sd->GetFirstSubproblem(); s; s=sd->GetNextSubproblem())
		{
			if (s->F == _LinearCallbackFn)
			{
				LinearSubproblem* ls = (LinearSubproblem*)s->user_ptr;
				if (ls->m) delete ls->m;
				delete [] ls->theta_old;
				delete ls;
			}
			else if (s->F == _MaxflowCallbackFn)
			{
				MaxflowSubproblem* ms = (MaxflowSubproblem*)s->user_ptr;
				if (ms->q) delete ms->q;
				delete [] ms->theta_old;
				delete ms;
			}
			else if (s->F == _LocalCallbackFn)
			{
				LocalSubproblem* ls = (LocalSubproblem*)s->user_ptr;
				delete [] ls->nodes0;
				delete ls;
			}
			else if (s->F == _TreeCallbackFn)
			{
				TreeNode* i = (TreeNode*)s->user_ptr;
				delete [] i->ptrs;
			}
			delete [] s->vars;
			delete [] s->array;
		}
		delete sd;
	}
	free(nodes[0]);
	if (node_neibs) delete node_neibs;
	if (tree_nodes) delete tree_nodes;
	if (theta0) delete [] theta0;
	if (buf) free(buf);
	if (node_neib_ptrs[0]) free(node_neib_ptrs[0]);
}


void GraphMatching::RequestBufSize(int size)
{
	if (size < buf_size) return;
	if (buf) free(buf);
	buf_size = size;
	buf = (char*)malloc(buf_size);
}


double GraphMatching::ComputeEnergy(int* solution_left)
{
	Node* i;
	Assignment* a;
	Assignment* b;
	Edge* e;
	double E = 0;

	for (i=nodes[1]; i<nodes[1]+N1; i++) i->match = NULL;
	for (i=nodes[0]; i<nodes[0]+N0; i++, solution_left++)
	{
		if (*solution_left < 0) { i->match = NULL; continue; }
		if (*solution_left >= N1) { printf("Error in ComputeEnergy(): not a valid solution!\n"); exit(1); }
		Node* j = nodes[1]+(*solution_left);
		if (j->match) return GM_INFTY;
		for (a=i->first; a; a=a->next[0]) if (a->i[1] == j) break;
		if (!a) { printf("Error in ComputeEnergy(): not a valid solution!\n"); exit(1); }
		i->match = j->match = a;
	}

	for (i=nodes[0]; i<nodes[0]+N0; i++)
	{
		a = i->match;
		if (!a) continue;
		E += a->cost;
		for (e=a->first[0]; e; e=e->next[0])
		{
			b = e->head[0];
			if (b->i[0]->match == b) E += e->cost;
		}
	}

	return E;
}

double GraphMatching::ComputeEnergy()
{
	Node* i;
	Assignment* a;
	Assignment* b;
	Edge* e;
	double E = 0;

	for (i=nodes[0]; i<nodes[0]+N0; i++)
	{
		a = i->match;
		if (!a) continue;
		E += a->cost;
		for (e=a->first[0]; e; e=e->next[0])
		{
			b = e->head[0];
			if (b->i[0]->match == b) E += e->cost;
		}
	}

	if (Ebest > E)
	{
		Ebest = E;
		int _i;
		for (_i=0, i=nodes[0]; _i<N0; _i++, i++)
		{
			solution_left_best[_i] = (i->match) ? ((int)(i->match->i[1]-nodes[1])) : -1;
		}
	}

	return E;
}

void GraphMatching::InitDD()
{
	Assignment* a;
	Edge* e;

	assert(!sd && !theta0);

	MergeEdges();

	A_max = A;
	E_max = E;
	theta0 = new double[A+E];

	for (a=assignments; a<assignments+A; a++) *theta0 ++ = a->cost;
	for (e=edges; e<edges+E; e++)             *theta0 ++ = e->cost;
	theta0 -= A+E;

	sd = new SubgradientDecomposition(A+E, theta0, this);
}

void GraphMatching::ResetSubproblems()
{
	if (!sd) return;

	SubgradientDecomposition::Subproblem* s;
	for (s=sd->GetFirstSubproblem(); s; s=sd->GetNextSubproblem())
	{
		memset(s->vars, 0, s->var_num*sizeof(double));
	}
}

double GraphMatching::_UpperBoundCallbackFn(void* user_arg)
{
	GraphMatching* m = (GraphMatching*)user_arg;
	double cost = m->ComputeEnergy();
	Node* i;
	for (i=m->nodes[0]; i<m->nodes[0]+m->N0+m->N1; i++) i->match = NULL;
	return cost;
}


double GraphMatching::SolveDD(int iter_max, double gap_threshold)
{
	if (!sd) { printf("Error: add subproblems before calling SolveDD()!\n"); exit(1); }

	SubgradientDecomposition::Subproblem* s;
	for (s=sd->GetFirstSubproblem(); s; s=sd->GetNextSubproblem())
	{
		if (s->F == _LinearCallbackFn)
		{
			LinearSubproblem* ls = (LinearSubproblem*)s->user_ptr;
			if (ls->m)
			{
				delete ls->m;
				ls->m = NULL;
			}
		}
		else if (s->F == _MaxflowCallbackFn)
		{
			MaxflowSubproblem* ms = (MaxflowSubproblem*)s->user_ptr;
			if (ms->q)
			{
				delete ms->q;
				ms->q = NULL;
			}
		}
	}

	Node* i;
	Assignment* a;
	for (i=nodes[0]; i<nodes[0]+N0+N1; i++) i->match = NULL;
	for (a=assignments; a<assignments+A; a++) a->flag = 0;

	sd->options.verbose = options.verbose;
	return sd->Maximize(iter_max, gap_threshold, _UpperBoundCallbackFn);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


void GraphMatching::MergeEdges()
{
	Assignment* a;
	Edge* e;
	int _e, q;
	Edge** edge_ptrs = new Edge*[A];
	memset(edge_ptrs, 0, A*sizeof(Edge*));
	bool flag = false;

	for (a=assignments; a<assignments+A; a++)
	{
		for (q=0; q<2; q++)
		for (e=a->first[q]; e; e=e->next[q])
		{
			if (e->head[q] <= a) continue;
			int _b = (int)(e->head[q]-assignments);
			if (edge_ptrs[_b])
			{
				edge_ptrs[_b]->cost += e->cost;
				e->cost = 0;
				flag = true;
			}
			else edge_ptrs[_b] = e;
		}
		for (q=0; q<2; q++)
		for (e=a->first[q]; e; e=e->next[q])
		{
			if (e->head[q] <= a) continue;
			int _b = (int)(e->head[q]-assignments);
			edge_ptrs[_b] = NULL;
		}
	}

	delete [] edge_ptrs;

	if (!flag) return;

	for (a=assignments; a<assignments+A; a++) a->first[0] = a->first[1] = NULL;

	struct NewEdge { int a, b; REAL cost; }* new_edges = new NewEdge[E];
	int E_new = 0;
	for (e=edges; e<edges+E; e++)
	if (e->cost)
	{
		new_edges[E_new].a = (int)(e->head[1]-assignments);
		new_edges[E_new].b = (int)(e->head[0]-assignments);
		new_edges[E_new].cost = e->cost;
		E_new ++;
	}
	E = 0;
	for (_e=0; _e<E_new; _e++) AddEdge(new_edges[_e].a, new_edges[_e].b, new_edges[_e].cost);
	delete [] new_edges;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


GraphMatching::NodeNeib* GraphMatching::_AddNeighbors(int r, NodeId i, NodeId j, bool do_not_allocate)
{
	assert((r==0 || r==1) && i>=0 && i<N[r] && j>=0 && j<N[r] && i!=j);

	int q;
	NodeNeib* nb;

	if (node_neib_tails[r] && node_neib_tails[r] != &nodes[r][i])
	{
		for (q=0; q<2; q++)
		for (nb=node_neib_tails[r]->first_neib[q]; nb; nb=nb->next[q])
		{
			node_neib_ptrs[r][(int)(nb->head[q]-nodes[r])] = NULL;
		}
		node_neib_tails[r] = NULL;
	}

	if (!node_neib_tails[r])
	{
		node_neib_tails[r] = &nodes[r][i];
		for (q=0; q<2; q++)
		for (nb=node_neib_tails[r]->first_neib[q]; nb; nb=nb->next[q])
		{
			node_neib_ptrs[r][(int)(nb->head[q]-nodes[r])] = nb;
		}
	}

	if (node_neib_ptrs[r][j]) { nb = node_neib_ptrs[r][j]; return nb; }

	if (do_not_allocate) return NULL;

	node_neib_ptrs[r][j] = nb = node_neibs->New();
	nb->head[0] = &nodes[r][j];
	nb->head[1] = &nodes[r][i];
	nb->next[0] = nb->head[1]->first_neib[0];
	nb->head[1]->first_neib[0] = nb;
	nb->next[1] = nb->head[0]->first_neib[1];
	nb->head[0]->first_neib[1] = nb;
	nb->weight = 0;
	return nb;
}

void GraphMatching::AddNeighbors(int r, NodeId i, NodeId j)
{
	if (!node_neibs) node_neibs = new Block<NodeNeib>(128);
	_AddNeighbors(r, i, j);
}

void GraphMatching::ResetNeighbors()
{
	int i;
	for (i=0; i<N0+N1; i++) nodes[0][i].first_neib[0] = nodes[0][i].first_neib[1] = NULL;
	memset(node_neib_ptrs[0], 0, (N0+N1)*sizeof(NodeNeib*));
	node_neib_tails[0] = node_neib_tails[1] = NULL;
	if (node_neibs)
	{
		delete node_neibs;
		node_neibs = NULL;
	}
}

void GraphMatching::ConstructNeighbors(int option)
{
	int i, j, r, _e;
	Assignment* a;
	Edge* e;
	NodeNeib* nb;

	node_neibs = new Block<NodeNeib>(128);
	NodeNeib** edge_ptrs[2];
	edge_ptrs[0] = new NodeNeib*[2*E];
	edge_ptrs[1] = edge_ptrs[0] + E;

	for (r=0; r<2; r++)
	for (i=0; i<N[r]; i++)
	{
		for (a=nodes[r][i].first; a; a=a->next[r])
		for (e=a->first[0]; e; e=e->next[0])
		{
			j = (int)(e->head[0]->i[r]-nodes[r]);
			nb = _AddNeighbors(r, i, j);
			nb->weight ++;//= (e->cost > 0) ? (e->cost) : (-e->cost);
			edge_ptrs[r][(int)(e-edges)] = nb;
		}
	}
	if (option)
	{
		for (_e=0; _e<E; _e++)
		{
			if (edge_ptrs[0][_e]->weight < edge_ptrs[1][_e]->weight) edge_ptrs[0][_e] = NULL;
			else                                                     edge_ptrs[1][_e] = NULL;
		}

		ResetNeighbors();
		node_neibs = new Block<NodeNeib>(128);

		for (r=0; r<2; r++)
		for (i=0; i<N[r]; i++)
		{
			for (a=nodes[r][i].first; a; a=a->next[r])
			for (e=a->first[0]; e; e=e->next[0])
			{
				if (!edge_ptrs[r][(int)(e-edges)]) continue;
				j = (int)(e->head[0]->i[r]-nodes[r]);
				nb = _AddNeighbors(r, i, j);
				nb->weight ++;//= (e->cost > 0) ? (e->cost) : (-e->cost);
			}
		}
	}

	delete [] edge_ptrs[0];
}

void GraphMatching::ComputeNeibWeights()
{
	int i, j, r;
	Assignment* a;
	Edge* e;
	NodeNeib* nb;

	memset(node_neib_ptrs[0], 0, (N0+N1)*sizeof(NodeNeib*));
	node_neib_tails[0] = node_neib_tails[1] = NULL;

	for (r=0; r<2; r++)
	for (i=0; i<N[r]; i++)
	{
		for (nb=nodes[r][i].first_neib[0]; nb; nb=nb->next[0]) nb->weight = 0;
	}

	for (r=0; r<2; r++)
	for (i=0; i<N[r]; i++)
	{
		for (a=nodes[r][i].first; a; a=a->next[r])
		for (e=a->first[0]; e; e=e->next[0])
		{
			j = (int)(e->head[0]->i[r]-nodes[r]);
			nb = _AddNeighbors(r, i, j, true);
			if (nb) nb->weight ++;//= (e->cost > 0) ? (e->cost) : (-e->cost);
		}
	}
}

