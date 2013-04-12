#include <stdio.h>
#include <string.h>
#include "SubgradientDecomposition.h"

inline double GetNorm2(double* g, int var_num)
{
	int i;
	double norm2 = 0;
	for (i=0; i<var_num; i++) norm2 += g[i]*g[i];
	return norm2;
}

inline void Add(double* vars, double* g, double lambda, int var_num)
{
	double* vars_end = vars + var_num;
	for ( ; vars<vars_end; vars++, g++) *vars += lambda*(*g);
}

SubgradientDecomposition::SubgradientDecomposition(int _n0, double* _x0, void* _user_arg)
	: n0(_n0), x0(_x0), n(0), user_arg(_user_arg)
{
	subproblems = new Block<Subproblem>(128);
	buf = new double[2*n0];
}

SubgradientDecomposition::~SubgradientDecomposition()
{
	delete subproblems;
	delete [] buf;
}

void SubgradientDecomposition::AddSubproblem(Subproblem& _s)
{
	Subproblem* s = subproblems->New();
	memcpy(s, &_s, sizeof(Subproblem));
	n += s->var_num;
}

void SubgradientDecomposition::ReadBest(double* vars)
{
	Subproblem* s;
	for (s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		memcpy(vars, s->vars, s->var_num*sizeof(double));
		vars += s->var_num;
	}
}

void SubgradientDecomposition::WriteBest(double* vars)
{
	Subproblem* s;
	for (s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		memcpy(s->vars, vars, s->var_num*sizeof(double));
		vars += s->var_num;
	}
}

double SubgradientDecomposition::ProjectAndComputeSubgradient(double* vars, double* g)
{
	int k = 0;
	double v = 0;
	Subproblem* s;
	memcpy(buf+n0, x0, n0*sizeof(double));
	for (s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		if (s->weight <= 0) continue;
		for (k=0; k<s->var_num; k++)
		{
			buf[n0+s->array[k]] -= s->weight*(*vars++);
		}
	}
	vars -= n;
	for (s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		if (s->weight <= 0) continue;
		for (k=0; k<s->var_num; k++) vars[k] += s->weight*buf[n0+s->array[k]]/buf[s->array[k]];
		v += s->F(*s, vars, g, user_arg);
		vars += s->var_num;
		g += s->var_num;
	}
	return v;
}

double SubgradientDecomposition::Maximize(int iter_max, double gap_threshold, UpperBoundFn F_upper)
{
	double theta, z, delta;
	double upper_bound_best = 1e100;
	Subproblem* s;
	int k;
	int gamma; // number of steps since the last improvement of the best objective value
	int gamma_bar = 20;
	int delta_min_num = 0;
	double norm2_max = 0;

	// init
	memset(buf, 0, n0*sizeof(double));
	n = 0;
	for (s=subproblems->ScanFirst(); s; s=subproblems->ScanNext())
	{
		if (s->weight < 0) { printf("Error: subproblem weight cannot be negative!\n"); exit(1); }
		if (s->weight == 0) continue;
		for (k=0; k<s->var_num; k++) buf[s->array[k]] += s->weight*s->weight;
		n += s->var_num;
	}
	for (k=0; k<n0; k++)
	{
		if (buf[k] <= 0) { printf("Incorrect decomposition: not all elements are covered!\n"); exit(1); }
	}

	double* vars = new double[2*n];
	double* g = vars + n;

	ReadBest(vars);
	z = theta = ProjectAndComputeSubgradient(vars, g);
	WriteBest(vars);

	gamma = 0;
	for (k=0; k<iter_max; k++)
	{
		double upper_bound = F_upper(user_arg);
		if (k==0 || upper_bound_best>upper_bound) upper_bound_best = upper_bound;
		if (k==0) delta = (upper_bound_best - z)*options.C;
		if (z>=upper_bound_best-gap_threshold) break;
		double norm2 = GetNorm2(g, n);
		if (norm2 == 0) break;
		if (norm2_max < norm2) norm2_max = norm2;
		double upper_bound_estimate = z+delta;
		//if (upper_bound_estimate > upper_bound_best) upper_bound_estimate = upper_bound_best;
		double lambda = options.alpha*(upper_bound_estimate-theta)/norm2;
		if (options.verbose) printf("%d\t%f\t%f\tstep=%f", k, theta, upper_bound, lambda);
		Add(vars, g, lambda, n);
		theta = ProjectAndComputeSubgradient(vars, g);

		if (theta > z)
		{
			if (options.verbose) printf(" *");
			delta *= options.A;
			z = theta;
			WriteBest(vars);
			gamma = 0;
		}
		else 
		{
			delta *= options.B;
			if (gamma ++ >= gamma_bar)
			{
				if (options.verbose) printf(" restart");
				gamma_bar += 10; if (gamma_bar > 50) gamma_bar = 50;
				ReadBest(vars);
				gamma = 0;
				theta = ProjectAndComputeSubgradient(vars, g);
				if (delta < (upper_bound_best - z)*0.01)
				{
					if (options.verbose) printf("\nconverged?\n");
					break;
				}
			}
		}
		if (delta < (upper_bound_best - z)*options.C)
		{
			if (options.verbose) printf(" !");
			if (delta_min_num ++ >= 30)
			{
				if (options.verbose) printf("\nconverged?\n");
				break;
			}
			delta = (upper_bound_best - z)*options.C;
		}
		if (options.verbose) printf("\n");
	}
	
	if (options.verbose) printf("lower_bound=%f\tupper_bound=%f\n", z, upper_bound_best);

	delete [] vars;

	return z;
}
