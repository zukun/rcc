#ifndef HAGHJAHHRBAKSJGHASRHAWS
#define HAGHJAHHRBAKSJGHASRHAWS

#include "block.h"

// goal: maximize F({x_{si}}) = \sum_s w_s F_s ({x_{si}:i\in I_s)
// subject to \sum_{s: i\in I_s} w_s x_{si} = x0_i  for all i.
//
// s denotes subproblem index.
// I_s is a fixed subset of {0,...,n-1}
// F_s(...) is a concave function of |I_s| variables given by an oracle computing a subgradient
// x0_i for i=0,...,n-1 is a constant

class SubgradientDecomposition
{
public:

	// x0 must be an array of size n. It will not be modified during
	// the computation (the caller also must not modify it!)
	SubgradientDecomposition(int n, double* x0, void* user_arg);
	~SubgradientDecomposition();


	// Functions of types `SubgradientFn' and `UpperBoundFn' will be called from Maximize(). 

	// x points to array of size |I_s| (the function should not modify it).
	// g points to array of size |I_s|.
	// The function should then set it to a subgradient of (w_s*F_s()) at {x_{si}}
	// and return w_s*F_s({x_si}). (Note multiplication by w_s!)
	struct Subproblem;
	typedef double (*SubgradientFn)(Subproblem& s, double* x, double* g, void* user_arg);
	// The caller may save internally information about subgradients
	// and use it for computing an upper bound.
	typedef double (*UpperBoundFn)(void* user_arg);

	struct Subproblem
	{
		int				var_num; // = |I_s|
		double*			vars; // array of size var_num - must be allocated,deallocated and initialized by the user
		int*			array; // array of size var_num: I_s = {array[0],array[1],...,array[n-1]}

		double			weight; // >= 0

		SubgradientFn	F;
		void*			user_ptr; // reserved for the user - not modified by member functions
	};
	// the content of 's' is copied into internal memory (but not arrays s.vars and s.array!)
	void AddSubproblem(Subproblem& s);

	// Note: s->vars arrays contain initial values of x_{si} variables
	//   (they are not requred to satisfy the equality constraints).
	// Maximize() modifies these variables and returns the corresponding value of the objective function.
	//   (The equality constraints are satisfied upon termination.)
	double Maximize(int iter_max, double gap_threshold, UpperBoundFn F_upper=NULL);

	// step size: lambda = alpha*(F(x_best) + delta - F(x_current))/||g||^2
	// where g is the current subgradient, F(x_best) + delta is an estimate of the optimal value.
	// delta is updated based on the progress made.
	struct Options
	{
		Options() : alpha(1.0), A(1.5), B(0.95), C(0.05), verbose(true) {}
		double alpha;
		double A; // multiply delta by A, if the best lower bound F(x_best) improved in the current iteration.
		double B; // multiply delta by B  otherwise.
		double C; // if delta becomes smaller than (best_upper_bound-F(x_best))*C, set it to (best_upper_bound-F(x_best))*C. If this happens more than 30 times, terminate.
		bool verbose;
	} options;

	// read subproblems
	Subproblem* GetFirstSubproblem();
	Subproblem* GetNextSubproblem();





	
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

private:
	Block<Subproblem>* subproblems;
	int n0;
	double* x0;
	double* buf;
	int n; // \sum_s |I_s|
	void* user_arg;

	void ReadBest(double* vars);
	void WriteBest(double* vars);
	double ProjectAndComputeSubgradient(double* vars, double* g);
};

inline SubgradientDecomposition::Subproblem* SubgradientDecomposition::GetFirstSubproblem()
{
	return subproblems->ScanFirst();
}

inline SubgradientDecomposition::Subproblem* SubgradientDecomposition::GetNextSubproblem()
{
	return subproblems->ScanNext();
}

#endif
