/////////////////////////////////////////////////////////////////////////////
// Filename: MemoryManager.h
// Author:   Sameh Khamis
//
// Description: Implementation of a paging memory manager for out-of-core
//              processing of graphs by the Push-Relabel algorithm
/////////////////////////////////////////////////////////////////////////////
#ifndef _MEMORY_MANAGER
#define _MEMORY_MANAGER

#include <fstream>
#include <sstream>
#include <iomanip>
#include <deque>
using namespace std;

#include <boost/filesystem.hpp>
namespace filesys = boost::filesystem;

#include <boost/iostreams/positioning.hpp>
using namespace boost::iostreams;

class MemoryManager
{
public:
	typedef stream_offset int64;
	static const int PAGE_NOT_FOUND = -1;

private:
	struct ResidentPage
	{
		char* addr;
		int ref_count;
		unsigned lru_timestamp;
		int page_id;
	};

	fstream *handle;
	string name;
	int *page_table;
	bool *mapped_before;
	int64 page_size;
	unsigned timestamp;
	deque<ResidentPage> resident;

	void map(ResidentPage* page);
	void unmap(ResidentPage* page);

public:
	MemoryManager(int64 space_size, int64 page_size, int resident_page_count);
	~MemoryManager();

	void* add_ref(int64 addr);
	void remove_ref(int64 addr);
};

#endif
