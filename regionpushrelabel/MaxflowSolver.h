/////////////////////////////////////////////////////////////////////////////
// Filename: MaxflowSolver.h
// Author:   Sameh Khamis
//
// Description: Maxflow algorithm abstraction
/////////////////////////////////////////////////////////////////////////////
#ifndef _MAXFLOW_SOLVER
#define _MAXFLOW_SOLVER

template <typename IdType, typename CapType, typename FlowType>
class MaxflowSolver
{
protected:
	FlowType flow;

public:
	typedef IdType _IdType;
	typedef CapType _CapType;
	typedef FlowType _FlowType;

	virtual void add_node(IdType nnodes) = 0;
	virtual void add_edge(IdType node_i, IdType node_j, CapType cap, CapType rev_cap) = 0;
	virtual void add_terminal_weights(IdType node_id, FlowType src_cap, FlowType snk_cap) = 0;

	virtual void compute_maxflow() = 0;
	virtual FlowType get_flow() = 0;
	virtual void add_constant_to_flow(CapType amount) = 0;
	virtual int get_segment(IdType node) = 0;
};

#endif
