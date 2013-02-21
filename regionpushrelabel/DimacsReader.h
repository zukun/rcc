/////////////////////////////////////////////////////////////////////////////
// Filename: DimacsReader.h
// Author:   Sameh Khamis
//
// Description: Dimacs reader
/////////////////////////////////////////////////////////////////////////////
#ifndef _DIMACS_READER
#define _DIMACS_READER

#include <string>
using namespace std;

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
using namespace boost::iostreams;

template <typename Solver>
class DimacsReader
{
private:
	typedef typename Solver::_IdType IdType;
	typedef typename Solver::_CapType CapType;
	typedef typename Solver::_FlowType FlowType;

	Solver* solver;
	IdType prev_from, prev_to;
	long* dims;
	CapType prev_cap;
	stream<file_source> file;

protected:
	void handle_construction(IdType nnodes, IdType nedges);
	void handle_destruction();
	void handle_comment(string comment);
	void handle_from_source_edge(IdType node, FlowType cap);
	void handle_to_sink_edge(IdType node, FlowType cap);
	void handle_edge(IdType node_i, IdType node_j, CapType cap);
	void handle_source_sink_edge(CapType cap); // can only increase the flow by cap

public:
	DimacsReader(string filename, long* sizes = NULL);
	~DimacsReader();

	bool parse();
	Solver* get_solver() { return solver; };
};

#include "DimacsReader.tpl"

#endif
