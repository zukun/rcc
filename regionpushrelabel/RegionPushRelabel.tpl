/////////////////////////////////////////////////////////////////////////////
// Filename: RegionPushRelabel.tpl
// Author:   Sameh Khamis
//
// Description: Implementation of a parallel out-of-core cache-oblivious FIFO
//              Push-Relabel algorithm for graphs with a regular (repeating)
//              edge structure, extends the work on grid graphs described
//              in Delong and Boykov, CVPR 2008
/////////////////////////////////////////////////////////////////////////////
#ifndef _REGION_PUSH_RELABEL_TPL
#define _REGION_PUSH_RELABEL_TPL

#include "RegionPushRelabel.h"
#include <iostream>

//////////////////////
// RegionPushRelabel
//////////////////////

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionPushRelabel(long dimensions[])
{
	// Initialize layout offsets
	layout = new Layout(dimensions);
	bucket_count = (layout->node_count + BUCKET_DENSITY - 1) >> BUCKET_DENSITY_BITS;

	// Blocks
	memory = new MemoryManager(layout->block_count * BLOCK_SIZE,
		BLOCKS_PER_MEMORY_PAGE * BLOCK_SIZE,
		THREAD_COUNT * MAX_BLOCKS_PER_REGION);

	for (size_t i = 0; i < layout->block_count; i++)
		initialize_block(i);

	// Shared data
	block_owner = new char[layout->block_count];
	block_location_index = new unsigned short[layout->block_count];
	typename Layout::Coord block_coord;
	for (size_t i = 0; i < layout->block_count; i++)
	{
		block_owner[i] = -1;
		layout->get_block_coord(i, block_coord);
		block_location_index[i] = layout->get_block_location_index(block_coord);
	}

	gaps = new unsigned[layout->block_count];
	for (size_t i = 0; i < layout->block_count; i++)
		gaps[i] = layout->node_count;

	active_count = new size_t[layout->block_count];
	for (size_t i = 0; i < layout->block_count; i++)
		active_count[i] = 0;

	label_counts = new size_t[bucket_count];
	label_counts[0] = layout->node_count;
	for (size_t b = 1; b < bucket_count; b++)
		label_counts[b] = 0;

	active = new DoublyLinkedArray<size_t>(layout->block_count);

	busy_count = THREAD_COUNT;
	gap_count = 0;
	max_bucket = 0;
	flow = 0;
	work_done = false;
	gap_found = false;

	// Threads
	for (char i = 0; i < THREAD_COUNT; i++)
		workers[i] = new RegionWorker(this, i);
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::~RegionPushRelabel()
{
	// Need to only call destructors, which nodes and blocks don't have
	delete[] block_owner;
	delete[] gaps;
	delete[] label_counts;
	delete[] active_count;
	delete active;

	for (char i = 0; i < THREAD_COUNT; i++)
		delete workers[i];

	delete memory;
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::initialize_block(size_t i)
{
	// Initialize block data, but don't populate its node list now (lazy load it instead)
	Block* block = load_block(i);

	block->cur_node = 0;
	block->cur_edge = 0;
	block->region_id = 0;
	block->list_populated = false;
	block->id = i;
	block->discharges = 0;
	block->active.clear();

	Node* node = block->nodes;
	typename Layout::Coord node_coord;
	for (size_t j = 0; j < Layout::NODES_PER_BLOCK; j++)
	{
		layout->get_node_coord(i, j, node_coord);

		node->distance = 0;
		node->preflow = 0;
		for (size_t k = 0; k < Layout::NODE_EDGE_COUNT; k++)
			node->residual[k] = 0;
		node->cur_edge = 0;
		node->cell_index = node_coord[0];
		node->boundary = layout->get_boundary_membership(node_coord);
		node->relabel = false;
		node->location_index = layout->get_node_location_index(node_coord);

		node++;
	}

	unload_block(i);
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::populated_active_list(Block* block)
{
	Node* node = block->nodes;
	ActiveList& list = block->active;
	for (size_t j = 0; j < Layout::NODES_PER_BLOCK; j++)
	{
		if (node->preflow > 0)
			list.push_back(j);
		node++;
	}
	block->list_populated = true;
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::add_edge(size_t node_i, size_t node_j, CapType cap, CapType rev_cap)
{
	node_i = layout->get_node_id(node_i);
	node_j = layout->get_node_id(node_j);

	size_t block_i, node_subi, block_j, node_subj;
	layout->get_node_block_index(node_i, block_i, node_subi);
	layout->get_node_block_index(node_j, block_j, node_subj);

	Block* block_from = load_block(block_i);
	Block* block_to = load_block(block_j);

	Node& node_from = block_from->nodes[node_subi];
	Node& node_to = block_to->nodes[node_subj];

	ptrdiff_t shift = node_subj - node_subi;
	ptrdiff_t* offset = layout->get_node_shift_vector(node_from.cell_index, node_from.location_index);
	ptrdiff_t idx;
	ptrdiff_t nedges = layout->get_edge_count(node_from.cell_index);

	for (idx = 0; idx < nedges; idx++, offset++)
		if (*offset == shift)
			break;

	if (idx != nedges)
	{
		node_from.residual[idx] += cap;
		idx = layout->get_sister_edges(node_from.cell_index)[idx];
		if (idx != -1) node_to.residual[idx] += rev_cap;
	}
	else
	{
		// Unexpected: non-grid edge - will be ignored!
		//cout << "Warning: non-grid edge detected! Layout definition problem?" << endl;
	}

	unload_block(block_i);
	unload_block(block_j);
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::add_terminal_weights(size_t node_id, FlowType src_cap, FlowType snk_cap)
{
	node_id = layout->get_node_id(node_id);

	size_t block_id, node_subid;
	layout->get_node_block_index(node_id, block_id, node_subid);

	Block* block = load_block(block_id);
	Node& node = block->nodes[node_subid];

	if (node.preflow > 0)
		src_cap += node.preflow;
	else
		snk_cap -= node.preflow;

	flow += (src_cap < snk_cap) ? src_cap : snk_cap;

	FlowType old_preflow = node.preflow;
	node.preflow = src_cap - snk_cap;

	if (old_preflow <= 0 && node.preflow > 0)
		active_count[block_id]++;
	else if (old_preflow > 0 && node.preflow <= 0)
		active_count[block_id]--;

	unload_block(block_id);
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::compute_maxflow()
{
	// Set up the active list
	for (size_t i = 0; i < layout->block_count; i++)
		if (active_count[i] > 0)
			active->push_back(i);

	// Creates (THREAD_COUNT - 1) threads and joins in on the action
	thread_group tgrp;

	for (char i = 1; i < THREAD_COUNT; i++)
	{
		thread *t = new thread(&RegionWorker::work_loop, workers[i]);
		tgrp.add_thread(t);
	}

	workers[0]->work_loop();
	tgrp.join_all();
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::update_region_sync(RegionWorker& worker)
{
	mutex::scoped_lock lock(active_mutex);

	// Release blocks from old region back
	Block* block;
	for (unsigned i = 0; i < worker.region_size; i++)
	{
		block = worker.region[i];

		block_owner[block->id] = -1;
		if (block->is_active())
			active->push_back(block->id);

		unload_block(block->id);
	}

	worker.region_size = 0;

	// Reserve the next active block to start a region
	// Make sure that this block is not neighboring any other reserved block to avoid "livelock" situations
	size_t block_id;

	unsigned active_index, active_max = active->get_count();
	for (active_index = 0; active_index < active_max; active_index++)
	{
		block_id = active->pop_front();

		unsigned e;
		for (e = 0; e < layout->block_edge_count; e++)
		{
			if (block_owner[block_id + layout->get_block_shift_vector(block_location_index[block_id])[e]] != -1)
				break;
		}
		if (e == layout->block_edge_count)
			break;
		
		active->push_back(block_id);
	}

	// No block matches the criteria, this thread can now sleep
	if (active_index == active_max)
		return;

	// Start reserving the neighbors of that first block
	block = load_block(block_id);
	block_owner[block_id] = worker.thread_id;

	if (!block->list_populated)
		populated_active_list(block);
	block->cur_node = block->active.begin();

	worker.region[0] = block;
	block->region_id = 0;
	worker.region_size++;

	// Reserve the block neighbors to create a region that is not overlapping with any reserved blocks
	size_t block_mask = 0;
	size_t edge_count = 0;
	size_t region_index = 0;
	Block* cur_block = worker.region[0];

	while (true)
	{
		if (edge_count < layout->block_edge_count)
		{
			// Calculate the new block id using the absolute offset lookup table
			block_id = cur_block->id + layout->get_block_shift_vector(block_location_index[cur_block->id])[cur_block->cur_edge];

			// If we don't have enough blocks and this block is not owned by another thread, grab it
			if (worker.region_size < MAX_BLOCKS_PER_REGION &&
				block_owner[block_id] == -1)
			{
				block = load_block(block_id);
				block_owner[block->id] = worker.thread_id;

				if (!block->list_populated)
					populated_active_list(block);
				block->cur_node = block->active.begin();

				if (block->is_active())
					active->remove(block_id);

				worker.region[worker.region_size] = block;
				block->region_id = worker.region_size;
				worker.region_size++;

				// Set the neighbor link and build a mask for which edges we have seen
				worker.neighbors[region_index][cur_block->cur_edge] = block;
				block_mask |= (1 << (size_t)cur_block->cur_edge);
			}
			// If this block is already this thread's, just set the neighbor link
			else if (block_owner[block_id] == worker.thread_id)
			{
				block = load_block(block_id);
				worker.neighbors[region_index][cur_block->cur_edge] = block;
				block_mask |= (1 << (size_t)cur_block->cur_edge);
			}
			else
			// Otherwise, leave that one because it is already being processed
				worker.neighbors[region_index][cur_block->cur_edge] = NULL;

			edge_count++;
			cur_block->cur_edge++;
			if (cur_block->cur_edge == layout->block_edge_count)
				cur_block->cur_edge = 0;
		}
		else
		{
			// Convert the block edge mask to a boundary mask
			ptrdiff_t* node_mask;
			for (unsigned c = 0; c < Layout::NODES_PER_CELL; c++)
			{
				for (size_t l = 0; l < layout->location_counts[c]; l++)
				{
					node_mask = layout->get_node_edge_mask(c, l);
					worker.boundary_mask[region_index][c][l] = 0;
					for (size_t be = 0; be < layout->block_edge_count; be++)
					{
						if (block_mask & (1 << be))
							worker.boundary_mask[region_index][c][l] |= node_mask[be];
					}
				}
			}

			// Move to next block
			region_index++;
			if (region_index == worker.region_size)
				break;

			cur_block = worker.region[region_index];
			edge_count = 0;
			block_mask = 0;
		}
	}

	// Notify waiting threads if active is not empty
	if (!active->empty())
		work_cond.notify_all();
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::update_data_sync(RegionWorker& worker)
{
	mutex::scoped_lock lock(data_mutex);

	// Update flow to sink
	flow += worker.flow_to_sink;
	worker.flow_to_sink = 0;

	// Update bucket size arrays
	IntegerPair* current;
	size_t bucket;

	// Increment buckets first using current->second and keep track of the last bucket
	current = worker.relabels_list;
	while (current < worker.relabels_iter)
	{
		bucket = current->second >> BUCKET_DENSITY_BITS;
		if (bucket < bucket_count)
		{
			label_counts[bucket]++;

			if (bucket > max_bucket)
				max_bucket = bucket;
		}
		current++;
	}

	// Now decrement using current->first and keep track of possible gaps
	current = worker.relabels_list;
	while (current < worker.relabels_iter)
	{
		bucket = current->first >> BUCKET_DENSITY_BITS;
		label_counts[bucket]--;

		if (label_counts[bucket] == 0)
			possible_gaps.push_back(bucket);

		current++;
	}

	worker.relabels_iter = worker.relabels_list;

	// Handle gap at minimum distance
	if (!possible_gaps.empty())
		gap_found = true;
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::wait_for_gap_relabeling()
{
	// All threads collapse here, and the last thread up does gap relabeling
	mutex::scoped_lock lock(busy_mutex);

	if (busy_count > 1)
	{
		busy_count--;
		gap_count++;
		gap_cond.wait(lock);
		busy_count++;
		gap_count--;
	}
	else
	{
		update_block_gaps();

		lock.unlock();
		gap_cond.notify_all();
	}
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::wait_for_work()
{
	// All threads collapse here, and wait until more work is available, or all work is done
	mutex::scoped_lock lock(busy_mutex);

	// If other threads are running (busy or waiting for a gap that was relabeled), sleep
	if (busy_count > 1 || (!gap_found && gap_count > 0))
	{
		busy_count--;
		work_cond.wait(lock);

		if (!work_done)
			busy_count++;
	}
	// In case there is a gap and this is the last thread, it should handle the gap
	// instead of declaring work done
	else if (gap_found)
	{
		update_block_gaps();

		lock.unlock();
		gap_cond.notify_all();
	}
	// If all work is done, wake up all threads so they would finish
	else
	{
		busy_count--;
		work_done = true;

		lock.unlock();
		work_cond.notify_all();
		gap_cond.notify_all();
	}
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::update_block_gaps()
{
	// Find the minimum gap
	size_t minimum_gap = bucket_count;

	for (vector<size_t>::iterator gap = possible_gaps.begin(); gap != possible_gaps.end(); gap++)
	{
		if (label_counts[*gap] == 0 && *gap < minimum_gap)
			minimum_gap = *gap;
	}

	// Handle the gap
	if (minimum_gap < bucket_count)
	{
		// Reset the minimum gap that each block has to handle
		// Blocks should then remove nodes that are beyond the gap in parallel
		size_t gap_distance = minimum_gap << BUCKET_DENSITY_BITS;
		for (size_t i = 0; i < layout->block_count; i++)
		{
			if (gaps[i] > gap_distance)
				gaps[i] = gap_distance;
		}

		// Fix label counts in the global table while we're single-threaded still
		for (size_t distance = minimum_gap; distance <= max_bucket; distance++)
			label_counts[distance] = 0;

		max_bucket = minimum_gap - 1;
	}

	// Reset gap variables
	gap_found = false;
	possible_gaps.clear();
}

//////////////////////
// RegionWorker
//////////////////////

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::RegionWorker(RegionPushRelabel* g, char id)
{
	flow_to_sink = 0;
	relabels_list = new IntegerPair[MAX_RELABELS_PER_BLOCK * MAX_BLOCKS_PER_REGION];
	relabels_iter = relabels_list;
	thread_id = id;
	graph = g;
	region_size = 0;

	// Neighbors initial size
	for (size_t i = 0; i < MAX_BLOCKS_PER_REGION; i++)
		neighbors[i].resize(graph->layout->block_edge_count);

	// Boundary mask initial size
	for (unsigned i = 0; i < MAX_BLOCKS_PER_REGION; i++)
		for (unsigned c = 0; c < Layout::NODES_PER_CELL; c++)
			boundary_mask[i][c].resize(graph->layout->location_counts[c]);
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::~RegionWorker()
{
	delete[] relabels_list;
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::find_next_relabel_distance(size_t& distance, deque<Node*>* bucket, priority_queue<Node*, deque<Node*>, NodeCompare>* fixed)
{
	// Find the next distance to track by peeking into the bucket
	size_t d = graph->layout->node_count;
	for (unsigned i = 0; i < region_size; i++)
	{
		if (!bucket[i].empty())
		{
			d = bucket[i].front()->distance;
			break;
		}
	}

	// If the bucket is empty, find the next distance to use from the set of fixed nodes
	Node *node;
	if (d == graph->layout->node_count)
	{
		for (unsigned i = 0; i < region_size; i++)
		{
			if (!fixed[i].empty())
			{
				node = fixed[i].top();
				if (node->distance < d)
					d = node->distance;
			}
		}
	}

	// If none found, we are done
	distance = d;
	if (d == graph->layout->node_count)
		return;

	// Otherwise, collect all nodes at this distance into the bucket
	for (unsigned i = 0; i < region_size; i++)
	{
		while (!fixed[i].empty())
		{
			node = fixed[i].top();
			if (node->distance == d)
			{
				fixed[i].pop();
				bucket[i].push_back(node);
			}
			else
				break;
		}
	}
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::relabel_region()
{
	// We will use two bucket lists to do BFS on the nodes of the blocks, two buckets per block
	// We also need three bucket pointers to do the work
	deque<Node*> *bucket_from = bucket_1, *bucket_to = bucket_2, *bucket_temp;
	Node *node;
	Block* block;

	// Enqueue sink nodes for the backwards BFS
	for (unsigned i = 0; i < region_size; i++)
	{
		block = region[i];
		node = block->nodes;
		for (size_t j = 0; j < Layout::NODES_PER_BLOCK; j++)
		{
			if (node->preflow < 0) // Is sink node?
				bucket_from[i].push_back(node);
			else if (node->boundary & ~boundary_mask[i][node->cell_index][node->location_index]) // Is fixed/boundary node?
				fixed[i].push(node);
			else
				node->relabel = true;

			node++;
		}

		block->discharges = 0;
	}

	// Use fixed nodes of minimum distance if no sink nodes are in the block
	size_t distance;
	find_next_relabel_distance(distance, bucket_from, fixed);

	// Search from the bucket nodes labeling their neighbors
	IntegerPair*& r = relabels_iter;
	Node *neighbor;
	Block *neighbor_block;
	Block **all_neighbors;
	ptrdiff_t *sister;
	ptrdiff_t *offset;
	ptrdiff_t* block_edge;
	ptrdiff_t nedges;
	unsigned node_id, neighbor_id;
	bool done;
	typename deque<Node*>::iterator node_iter;

	do
	{
		done = true;
		for (unsigned i = 0; i < region_size; i++)
		{
			block = region[i];
			all_neighbors = &neighbors[i][0];

			if (bucket_from[i].empty())
				continue;
			
			done = false;
			bucket_temp = &bucket_from[i];

			// For all the nodes at the current distance, do a BFS on their neighbors and collect them
			// into the other bucket for this block
			for (node_iter = bucket_temp->begin(); node_iter != bucket_temp->end(); node_iter++)
			{
				node = *node_iter;

				node_id = node - block->nodes;
				offset = graph->layout->get_node_shift_vector(node->cell_index, node->location_index);
				sister = graph->layout->get_sister_edges(node->cell_index);
				block_edge = graph->layout->get_block_edge(node->cell_index, node->location_index);
				nedges = graph->layout->get_edge_count(node->cell_index);

				for (size_t e = 0; e < nedges; e++)
				{
					neighbor_id = node_id + *offset;

					if (node->boundary & (1 << e))
					{
						neighbor_block = all_neighbors[*block_edge];
						if (neighbor_block == NULL)
						{
							offset++; sister++; block_edge++;
							continue;
						}
						neighbor = &neighbor_block->nodes[neighbor_id];
					}
					else
					{
						neighbor_block = block;
						neighbor = node + *offset;
					}

					if (neighbor->relabel && *sister != -1 && neighbor->residual[*sister] > 0)
					{
						r->first = neighbor->distance;
						r->second = distance + 1;
						r++;

						neighbor->distance = distance + 1;
						neighbor->relabel = false;

						bucket_to[neighbor_block->region_id].push_back(neighbor);
					}

					offset++; sister++; block_edge++;
				}
			}
			bucket_temp->clear();
		}

		// Switch buckets
		bucket_temp = bucket_from;
		bucket_from = bucket_to;
		bucket_to = bucket_temp;

		// Re-populate bucket_from from the fixed nodes if it is empty
		find_next_relabel_distance(distance, bucket_from, fixed);
	}
	while (!done);

	// Erase the unreachable nodes
	typename ActiveList::Iterator iter;

	for (unsigned i = 0; i < region_size; i++)
	{
		block = region[i];

		// Make their distance unreachable in the nodes array
		node = block->nodes;
		for (size_t j = 0; j < Layout::NODES_PER_BLOCK; j++)
		{
			if (node->relabel)
			{
				r->first = node->distance;
				r->second = graph->layout->node_count;
				r++;

				node->distance = graph->layout->node_count;
				node->relabel = false;
			}
			node++;
		}

		// and remove them from the active list
		ActiveList& list = block->active;
		for (iter = block->cur_node; iter != list.end();)
		{
			if (block->nodes[list.get(iter)].distance == graph->layout->node_count)
				list.remove(iter);
			else
				iter++;
		}
	}
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE bool RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::relabel(Node* node)
{
	Node* neighbor;
	Block *neighbor_block;

	size_t min_label = graph->layout->node_count - 1;
	size_t min_edge = 0;

	ptrdiff_t *offset = graph->layout->get_node_shift_vector(node->cell_index, node->location_index);
	CapType *residual = node->residual;
	ptrdiff_t *block_edge = graph->layout->get_block_edge(node->cell_index, node->location_index);

	// Since we're checking for residual before anything, we can use the faster Layout::NODE_EDGE_COUNT
	for (size_t e = 0; e < Layout::NODE_EDGE_COUNT; e++)
	{
		if (*residual > 0)
		{
			// A boundary node with a neighbor that we do not have the right to process? don't relabel!
			if (node->boundary & (1 << e))
			{
				neighbor_block = cur_neighbors[*block_edge];
				if (neighbor_block == NULL)
					return false;
				neighbor = &neighbor_block->nodes[node + *offset - cur_block->nodes];
			}
			else
				neighbor = node + *offset;

			// Find minimum distance and corresponding edge
			if ((neighbor->distance) < (min_label))
			{
				min_label = neighbor->distance;
				min_edge = e;
			}
		}

		offset++; residual++; block_edge++;
	}

	node->cur_edge = min_edge;
	node->distance = min_label + 1;
	return true;
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::discharge()
{
	IntegerPair*& r = relabels_iter;
	ActiveList& list = cur_block->active;

	Node *node, *neighbor;
	CapType* residual;
	FlowType delta;
	ptrdiff_t* offset;
	ptrdiff_t* sister;
	ptrdiff_t* block_edge;
	size_t old_distance;
	Block* neighbor_block;
	unsigned node_id, neighbor_id;

	int count = DISCHARGES_PER_BLOCK;
	bool can_relabel;

	// For all nodes
	while (true)
	{
		if (cur_block->cur_node == list.end() || count-- == 0)
			return;

		node_id = list.get(cur_block->cur_node);
		node = &cur_block->nodes[node_id];
		can_relabel = true;

		// Push to neighbors
		old_distance = node->distance;
		residual = node->residual + node->cur_edge;
		offset = graph->layout->get_node_shift_vector(node->cell_index, node->location_index) + node->cur_edge;
		sister = graph->layout->get_sister_edges(node->cell_index) + node->cur_edge;
		block_edge = graph->layout->get_block_edge(node->cell_index, node->location_index) + node->cur_edge;

		// For all neighbors
		while (true)
		{
			// Since we're checking for residual before anything, we can use the faster Layout::NODE_EDGE_COUNT
			if (node->cur_edge == Layout::NODE_EDGE_COUNT) // preflow > 0
			{
				// Relabel the node if possible
				if (can_relabel && relabel(node))
				{
					if (node->distance == graph->layout->node_count)
					{
						list.remove(cur_block->cur_node);
						break;
					}

					residual = node->residual + node->cur_edge;
					offset = graph->layout->get_node_shift_vector(node->cell_index, node->location_index) + node->cur_edge;
					sister = graph->layout->get_sister_edges(node->cell_index) + node->cur_edge;
					block_edge = graph->layout->get_block_edge(node->cell_index, node->location_index) + node->cur_edge;
				}
				else
				{
					// Node is stuck
					cur_block->cur_node++;
					break;
				}
			}

			// Can push?
			if (*residual > 0)
			{
				neighbor_id = node_id + *offset;

				if (node->boundary & (1 << (size_t)node->cur_edge))
				{
					neighbor_block = cur_neighbors[*block_edge];
					if (neighbor_block == NULL)
					{
						// Skip edge, node cannot be relabeled
						can_relabel = false;

						node->cur_edge++;
						offset++; residual++; sister++; block_edge++;
						continue;
					}
					neighbor = &neighbor_block->nodes[neighbor_id];
				}
				else
				{
					neighbor_block = cur_block;
					neighbor = node + *offset;
				}

				if (node->distance == neighbor->distance + 1)
				{
					if (node->preflow < *residual)
					{
						delta = node->preflow;
						list.remove(cur_block->cur_node);
					}
					else
					{
						delta = *residual;
					}

					*residual -= delta;
					if (*sister != -1) neighbor->residual[*sister] += delta;
					node->preflow -= delta;
					neighbor->preflow += delta;

					if (neighbor->preflow <= 0)
					{
						flow_to_sink += delta;
					}
					else if (neighbor->preflow <= delta)
					{
						// Activated
						flow_to_sink += delta - neighbor->preflow;
						neighbor_block->active.push_back(neighbor_id);
					}

					// Deactivated
					if (node->preflow == 0)
						break;
				}
			}

			node->cur_edge++;
			offset++; residual++; sister++; block_edge++;
		}

		// Record any relabeling to track gaps
		if (node->distance != old_distance)
		{
			r->first = old_distance;
			r->second = node->distance;
			r++;
		}

		cur_block->discharges++;
	}
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::gap_relabel()
{
	region_discharges = 0;
	Block* block;
	Node* node;
	typename ActiveList::Iterator iter;

	for (unsigned i = 0; i < region_size; i++)
	{
		block = region[i];
		region_discharges += block->discharges;

		size_t gap_distance = graph->gaps[block->id];
		if (gap_distance == graph->layout->node_count)
			continue;

		// Clean up the active list
		ActiveList& list = block->active;
		for (iter = block->cur_node; iter != list.end();)
		{
			node = &block->nodes[list.get(iter)];
			if (node->distance > gap_distance)
			{
				node->distance = graph->layout->node_count;
				list.remove(iter);
			}
			else
				iter++;
		}

		// Loop through the rest of the nodes
		node = block->nodes;
		for (size_t j = 0; j < Layout::NODES_PER_BLOCK; j++)
		{
			if (node->distance != graph->layout->node_count && node->distance > gap_distance)
				node->distance = graph->layout->node_count;
			node++;
		}

		graph->gaps[block->id] = graph->layout->node_count;
	}
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::discharge_region()
{
	// Discharge blocks iteratively, break if a gap is found
	for (unsigned cur_index = 0; cur_index < region_size; cur_index++)
	{
		cur_block = region[cur_index];
		cur_neighbors = &neighbors[cur_index][0];

		if (graph->gap_found)
			return;

		discharge();
	}
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
bool RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::is_region_discharged()
{
	// If all node pointers are at the end, we finished discharging this region
	Block* block;
	for (unsigned i = 0; i < region_size; i++)
	{
		block = region[i];
		if (block->cur_node != block->active.end())
			return false;
	}
	return true;
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::RegionWorker::work_loop()
{
	while (true)
	{
		// Reserve a new region if needed
		if (is_region_discharged())
		{
			relabel_region();
			graph->update_region_sync(*this);

			// Wait for more work if region is empty
			if (region_size == 0)
			{
				graph->wait_for_work();
				if (graph->work_done)
					break;
				else
					continue;
			}
		}

		// Wait for synchronization if gap is found
		if (graph->gap_found)
			graph->wait_for_gap_relabeling();

		// Process the new region and update shared data
		gap_relabel();

		discharge_region();
		graph->update_data_sync(*this);
	}
}

#endif
