/////////////////////////////////////////////////////////////////////////////
// Filename: MemoryManager.cpp
// Author:   Sameh Khamis
//
// Description: Implementation of a paging memory manager for out-of-core
//              processing of graphs by the Push-Relabel algorithm
/////////////////////////////////////////////////////////////////////////////
#include "MemoryManager.h"

#include <iostream>
using namespace std;

MemoryManager::MemoryManager(int64 space_size, int64 page_size, int resident_page_count = 0)
{
	this->page_size = page_size;
	int page_count = (space_size + page_size - 1) / page_size;
	timestamp = 0;

	resident.resize(resident_page_count);
	for (int i = 0; i < resident.size(); i++)
	{
		try
		{
			resident[i].addr = new char[page_size];
		}
		catch (bad_alloc& ba)
		{
			for (int id = 0; id < i; id++)
				delete[] resident[id].addr;

			cout << "Allocation failure. Try tweaking the BlocksPerMemoryPage and BlockDimensions parameters." << endl;
			exit(1);
		}
		resident[i].lru_timestamp = 0;
		resident[i].ref_count = 0;
		resident[i].page_id = PAGE_NOT_FOUND;
	}

	page_table = new int[page_count];
	for (int i = 0; i < page_count; i++)
		page_table[i] = PAGE_NOT_FOUND;

	mapped_before = new bool[page_count];
	for (int i = 0; i < page_count; i++)
		mapped_before[i] = false;

	// Create the memory mapped file
	srand(time(0));
	int random = (double)rand() / (RAND_MAX + 1) * 9999;
	stringstream namestream;
	namestream << "temp" << setfill('0') << setw(4) << random << ".mem";
	name = namestream.str();

	handle = new fstream(name.c_str(), ios::in | ios::out | ios::binary | ios::trunc);
}

MemoryManager::~MemoryManager()
{
	delete[] page_table;
	delete[] mapped_before;
	for (int i = 0; i < resident.size(); i++)
		delete[] resident[i].addr;

	delete handle;
	filesys::remove(name);
}

void* MemoryManager::add_ref(int64 addr)
{
	ResidentPage* page;
	int page_id = addr / page_size;
	int64 offset = addr - page_id * page_size;

	int resident_id = page_table[page_id];

	if (resident_id == PAGE_NOT_FOUND)
	{
		// Find LRU resident page id
		unsigned min_timestamp = timestamp + 1;
		for (int i = 0; i < resident.size(); i++)
		{
			if (resident[i].page_id == PAGE_NOT_FOUND)
			{
				resident_id = i;
				break;
			}
			if (resident[i].ref_count == 0 && resident[i].lru_timestamp < min_timestamp)
			{
				resident_id = i;
				min_timestamp = resident[i].lru_timestamp;
			}
		}
		if (resident_id == PAGE_NOT_FOUND)
		{
			resident_id = resident.size();
			resident.resize(resident.size() + 1);

			page = &resident[resident_id];
			page->addr = new char[page_size];
		}
		else
		{
			page = &resident[resident_id];
			if (page->page_id != PAGE_NOT_FOUND)
			{
				page_table[page->page_id] = PAGE_NOT_FOUND;
				unmap(page);
			}
		}

		page_table[page_id] = resident_id;
		page->page_id = page_id;
		page->ref_count = 0;

		if (mapped_before[page_id])
			map(page);
		mapped_before[page_id] = true;
	}
	else
		page = &resident[resident_id];
	
	page->ref_count++;
	page->lru_timestamp = ++timestamp;

	return page->addr + offset;
}

void MemoryManager::remove_ref(int64 addr)
{
	int page_id = addr / page_size;
	int resident_id = page_table[page_id];

	if (resident_id != PAGE_NOT_FOUND)
	{
		ResidentPage& page = resident[resident_id];
		page.ref_count--;
		if (page.ref_count < 0)
			page.ref_count = 0;
		page.lru_timestamp = ++timestamp;
	}
}

inline void MemoryManager::map(ResidentPage* page)
{
	int64 offset = page->page_id * page_size;
	streampos pos = offset_to_position(offset);
	handle->seekg(pos);
	handle->read(page->addr, page_size);
}

inline void MemoryManager::unmap(ResidentPage* page)
{
	int64 offset = page->page_id * page_size;
	streampos pos = offset_to_position(offset);
	handle->seekp(pos);
	handle->write(page->addr, page_size);
}
