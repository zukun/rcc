/////////////////////////////////////////////////////////////////////////////
// Filename: DimacsReader.h
// Author:   Sameh Khamis
//
// Description: Dimacs reader
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "DimacsReader.h"

template <typename Solver>
DimacsReader<Solver>::DimacsReader(string filename, long* sizes)
{
	file.open(filename, 8*1024*1024); // 8 MB
	dims = sizes;
	solver = NULL;
}

template <typename Solver>
DimacsReader<Solver>::~DimacsReader()
{
	file.close();
	handle_destruction();
}

template <typename Solver>
inline void DimacsReader<Solver>::handle_construction(IdType nnodes, IdType nedges)
{
	if (solver == NULL)
	{
		//solver = new Solver(nnodes, nedges);
		solver = new Solver(dims);
		solver->add_node(nnodes);
	}
}

template <typename Solver>
inline void DimacsReader<Solver>::handle_destruction()
{
	if (solver != NULL)
	{
		delete solver;
		solver = NULL;
	}
}

template <typename Solver>
inline void DimacsReader<Solver>::handle_comment(string comment)
{
}

template <typename Solver>
inline void DimacsReader<Solver>::handle_from_source_edge(IdType node, FlowType cap)
{
	solver->add_terminal_weights(node, cap, (FlowType)0);
}

template <typename Solver>
inline void DimacsReader<Solver>::handle_to_sink_edge(IdType node, FlowType cap)
{
	solver->add_terminal_weights(node, (FlowType)0, cap);
}

template <typename Solver>
inline void DimacsReader<Solver>::handle_edge(IdType node_i, IdType node_j, CapType cap)
{
	// If this is the sister of the previous edge, add them together
	if (prev_from == node_j && prev_to == node_i)
	{
		solver->add_edge(prev_from, prev_to, prev_cap, cap);
		prev_from = -1;
	}
	else
	{
		if (prev_from != -1)
			solver->add_edge(prev_from, prev_to, prev_cap, (CapType)0);

		prev_from = node_i;
		prev_to = node_j;
		prev_cap = cap;
	}
}

template <typename Solver>
inline void DimacsReader<Solver>::handle_source_sink_edge(CapType cap)
{
	solver->add_constant_to_flow(cap);
}

template <typename Solver>
bool DimacsReader<Solver>::parse()
{
	if (!file.is_open())
	{
		return false;
	}

	prev_from = -1;

	file.seekg(0, ios_base::beg);

	bool problem = false;
	IdType source = 1, sink = 2;

	size_t lineno = 0;
	string line;
	const char* cline;

	while (!file.eof())
	{
		getline(file, line);
		cline = line.c_str();
		lineno++;

		if (line.empty())
			continue;

		if (cline[0] == 'c')
		{
			if (line.length() > 2)
				handle_comment(line.substr(2));
		}
		else if (cline[0] == 'p')
		{
			size_t nnodes, nedges;
			if (sscanf(cline, "%*c %*3s %lld %lld", &nnodes, &nedges) != 2)
				break;

			handle_construction((IdType)nnodes - 2, (IdType)nedges);
			problem = true;
		}
		else if (cline[0] == 'n')
		{
			size_t node;
			char type;
			if (sscanf(cline, "%*c %lld %c", &node, &type) != 2)
				break;

			if (type == 's')
				source = node;
			else if (type == 't')
				sink = node;
			else
				break;
		}
		else if (cline[0] == 'a')
		{
			size_t node_i, node_j;
			long long cap;
			if (sscanf(cline, "%*c %lld %lld %lld", &node_i, &node_j, &cap) != 3)
				break;

			if (node_i == source)
			{
				if (node_j == sink)
					handle_source_sink_edge((CapType)cap);
				else
				{
					if (source < node_j) node_j--;
					if (sink <= node_j) node_j--;
					handle_from_source_edge((IdType)node_j - 1, (FlowType)cap);
				}
			}
			else if (node_j == sink)
			{
				if (source < node_i) node_i--;
				if (sink <= node_i) node_i--;
				handle_to_sink_edge((IdType)node_i - 1, (FlowType)cap);
			}
			else
			{
				if (source < node_i) node_i--;
				if (sink <= node_i) node_i--;
				if (source < node_j) node_j--;
				if (sink <= node_j) node_j--;
				handle_edge((IdType)node_i - 1, (IdType)node_j - 1, (CapType)cap);
			}
		}
		else
		{
			break;
		}
	}

	if (!file.eof())
	{
		handle_destruction();
		return false;
	}
	else
	{
		// Handle last edge if not handled already
		handle_edge((IdType)-1, (IdType)-1, (CapType)0);
		return true;
	}
}
