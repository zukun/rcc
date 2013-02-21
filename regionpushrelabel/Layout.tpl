/////////////////////////////////////////////////////////////////////////////
// Filename: Layout.tpl
// Author:   Sameh Khamis
//
// Description: Layout descriptor for the Regular Push-Relabel algorithm,
//              implemented using template metaprogramming
/////////////////////////////////////////////////////////////////////////////
#ifndef _LAYOUT_TPL
#define _LAYOUT_TPL

#include "Layout.h"

template <typename OffsetVector, typename BlockDimensions>
void Layout<OffsetVector, BlockDimensions>::init()
{
	// Check if class is already initialized
	if (initialized)
		return;

	initialized = true;

	// Collect block dimensions
	block_dimensions[0] = NODES_PER_CELL;
	mpl::for_each<BlockDimensions>(CollectIntegers(block_dimensions + 1));

	// Collect vanilla offsets
	vector<size_t> to_indices[NODES_PER_CELL];
	mpl::for_each<OffsetVector>(CollectOffsets(&offsets[0][0], to_indices, DIM_COUNT));

	// Collect number of edges for each node
	mpl::for_each<FromCountVector>(CollectIntegers(edge_count_by_cell_index));

	// Discover sister edges
	for (size_t c = 0; c < NODES_PER_CELL; c++)
		for (size_t e = 0; e < NODE_EDGE_COUNT; e++)
			edge_sister[c][e] = -1;

	for (size_t c = 0; c < NODES_PER_CELL; c++)
	{
		for (size_t e = 0; e < edge_count_by_cell_index[c]; e++)
		{
			if (edge_sister[c][e] == -1)
			{
				size_t cs = to_indices[c][e];
				vector<size_t>& tos = to_indices[cs];
				size_t es;
				for (es = 0; es < edge_count_by_cell_index[cs]; es++)
				{
					// Must have reversed from/to and negative the offsets
					if (tos[es] == c)
					{
						size_t d;
						for (d = 1; d < DIM_COUNT; d++)
							if (offsets[c][d][e] != -offsets[cs][d][es])
								break;
						if (d == DIM_COUNT)
							break;
					}
				}
				// es if the sister of e
				if (es != edge_count_by_cell_index[cs])
				{
					edge_sister[c][e] = es;
					edge_sister[cs][es] = e;
				}
			}
		}
	}
}

template <typename OffsetVector, typename BlockDimensions>
Layout<OffsetVector, BlockDimensions>::Layout(long dimensions[])
{
	Layout::init();

	// Read in dimensions and calculate node count
	sizes_changed = false;
	original_sizes[0] = NODES_PER_CELL;
	sizes[0] = NODES_PER_CELL;
	node_count = NODES_PER_CELL;
	for (size_t i = 1; i < DIM_COUNT; i++)
	{
		original_sizes[i] = dimensions[i - 1];
		sizes[i] = ceil((double)dimensions[i - 1] / block_dimensions[i]) * block_dimensions[i];
		node_count *=  sizes[i];

		// Warning: block dimension did not divide graph dimension
		if (sizes[i] != dimensions[i - 1])
		{
			sizes_changed = true;
			//cout << "Warning: block dimension did not divide graph dimension! Be careful with the node ID calculations." << endl;
		}
	}

	// Compute blocks per dimension and total block count
	block_count = 1;
	ptrdiff_t blocks_per_dim[DIM_COUNT];
	blocks_per_dim[0] = 1;
	for (size_t d = 1; d < DIM_COUNT; d++)
	{
		blocks_per_dim[d] = sizes[d] / block_dimensions[d];
		block_count *= blocks_per_dim[d];
	}

	// Compute strides
	compute_strides(original_sizes, original_size_strides);
	compute_strides(sizes, size_strides);
	compute_strides(blocks_per_dim, block_strides);
	compute_strides(block_dimensions, block_dimension_strides);

	// For each cell index:
	vector<ptrdiff_t> block_offsets[DIM_COUNT];

	for (size_t c = 0; c < NODES_PER_CELL; c++)
	{
		// Compute node look-up tables
		location_counts[c] = compute_offset_lut(block_dimensions, block_dimension_strides, offsets[c],
			offset_strides[c], ranges[c], shifts[c]);

		// Compute the corresponding block edge for every node edge
		compute_block_edges(block_dimensions, offsets[c], offset_strides[c], ranges[c], location_counts[c],
			block_offsets, block_edge[c]);
	}
	// Save total count of blocks
	block_edge_count = block_offsets[0].size();

	// Compute block look-up tables
	compute_offset_lut(blocks_per_dim, block_strides, block_offsets,
		block_offset_strides, block_ranges, block_shifts);

	// Generate node edge mask for the node edges corresponding to every block edge
	compute_node_edge_masks(block_edge, location_counts, node_edge_mask);
}

template <typename OffsetVector, typename BlockDimensions>
void Layout<OffsetVector, BlockDimensions>::compute_strides(ptrdiff_t sizes[], size_t strides[])
{
	strides[0] = 1;
	for (size_t i = 1; i < DIM_COUNT; i++)
		strides[i] = sizes[i - 1] * strides[i - 1];
}

template <typename OffsetVector, typename BlockDimensions>
void Layout<OffsetVector, BlockDimensions>::compute_node_edge_masks(
	vector<vector<ptrdiff_t> > block_edge[], size_t location_counts[],
	vector<vector<ptrdiff_t> > node_edge_mask[])
{
	ptrdiff_t be;

	for (unsigned c = 0; c < NODES_PER_CELL; c++)
	{
		node_edge_mask[c].resize(location_counts[c]);

		for (size_t l = 0; l < location_counts[c]; l++)
		{
			node_edge_mask[c][l].resize(block_edge_count);
			for (be = 0; be < block_edge_count; be++)
				node_edge_mask[c][l][be] = 0;

			for (size_t e = 0; e < edge_count_by_cell_index[c]; e++)
			{
				be = block_edge[c][l][e];
				node_edge_mask[c][l][be] |= 1 << e;
			}
		}
	}
}

template <typename OffsetVector, typename BlockDimensions>
void Layout<OffsetVector, BlockDimensions>::compute_block_edges(
	ptrdiff_t shift_sizes[], vector<ptrdiff_t> offsets[], size_t offset_strides[],
	vector<size_t> ranges[], size_t location_count,
	vector<ptrdiff_t> block_offsets[], vector<vector<ptrdiff_t> >& block_edge)
{
	// Create block offsets from current shifts
	size_t edge_count = offsets[0].size();
	ptrdiff_t temp_edge[DIM_COUNT];
	temp_edge[0] = 0;

	block_edge.resize(location_count);
	for (size_t l = 0; l < location_count; l++)
	{
		block_edge[l].resize(edge_count);

		for (size_t e = 0; e < edge_count; e++)
		{
			// Compute the offsets for the block edge of this node edge
			size_t remainder = l;
			ptrdiff_t d;
			for (d = DIM_COUNT - 1; d >= 1; d--)
			{
				size_t p = remainder / offset_strides[d];
				remainder -= p * offset_strides[d];
				ptrdiff_t pos = ranges[d][p] - 1 + offsets[d][e];

				if (pos < 0)
					temp_edge[d] = pos / shift_sizes[d] - 1;
				else if (pos >= shift_sizes[d])
					temp_edge[d] = pos / shift_sizes[d];
				else
					temp_edge[d] = 0;
			}

			// Add if it is not already in the list
			size_t be;
			for (be = 0; be < block_offsets[0].size(); be++)
			{
				for (d = 1; d < DIM_COUNT; d++)
					if (block_offsets[d][be] != temp_edge[d])
						break;
				if (d == DIM_COUNT)
					break;
			}
			if (be == block_offsets[0].size())
			{
				for (d = 0; d < DIM_COUNT; d++)
					block_offsets[d].push_back(temp_edge[d]);
			}
			// Corresponding edge is in any case now at index (be)
			block_edge[l][e] = be;
		}
	}
}

template <typename OffsetVector, typename BlockDimensions>
size_t Layout<OffsetVector, BlockDimensions>::compute_offset_lut(
	ptrdiff_t shift_sizes[], size_t shift_strides[],
	vector<ptrdiff_t> offsets[], size_t offset_strides[],
	vector<size_t> ranges[], vector<vector<ptrdiff_t> >& shifts)
{
	// Compute location count and unique offset values in each dimension
	size_t edge_count = offsets[0].size();
	set<ptrdiff_t, greater<ptrdiff_t> > neg_unique[DIM_COUNT];
	set<ptrdiff_t, greater<ptrdiff_t> > pos_unique[DIM_COUNT];

	size_t location_count = 1;
	for (size_t d = 1; d < DIM_COUNT; d++)
	{
		for (size_t e = 0; e < edge_count; e++)
		{
			if (offsets[d][e] < 0)
				neg_unique[d].insert(offsets[d][e]);
			else if (offsets[d][e] > 0)
				pos_unique[d].insert(offsets[d][e]);
		}

		// Number of unique negative and unique positive values, and 0 since it's always there
		location_count *= (neg_unique[d].size() + pos_unique[d].size() + 1);
	}

	// Compute position ranges and absolute offset for each position
	// The ranges can be used to group nodes by how they find their neighbors (how they calculate absolute offsets)
	ptrdiff_t offset_sizes[DIM_COUNT];
	for (size_t d = 0; d < DIM_COUNT; d++)
	{
		offset_sizes[d] = neg_unique[d].size() + pos_unique[d].size() + 1;
		ranges[d].resize(offset_sizes[d]);
		size_t size = shift_sizes[d];
		size_t i = 0;

		for (set<ptrdiff_t, greater<ptrdiff_t> >::iterator o = neg_unique[d].begin(); o != neg_unique[d].end(); o++)
			ranges[d][i++] = -*o;
		for (set<ptrdiff_t, greater<ptrdiff_t> >::iterator o = pos_unique[d].begin(); o != pos_unique[d].end(); o++)
			ranges[d][i++] = size - *o;
		ranges[d][i] = size;
	}

	// Compute offset strides
	compute_strides(offset_sizes, offset_strides);

	// Create absolute offset lookup table by location index
	shifts.resize(location_count);
	for (size_t l = 0; l < location_count; l++)
	{
		shifts[l].resize(edge_count);

		for (size_t e = 0; e < edge_count; e++)
		{
			size_t remainder = l;
			for (ptrdiff_t d = DIM_COUNT - 1; d >= 1; d--)
			{
				size_t p = remainder / offset_strides[d];
				remainder -= p * offset_strides[d];

				ptrdiff_t shift;
				if (offsets[d][e] < 0)
					shift = -(-offsets[d][e] % shift_sizes[d]);
				else
					shift = offsets[d][e] % shift_sizes[d];

				ptrdiff_t pos = ranges[d][p] - 1 + shift;

				if (pos < 0)
					shift += shift_sizes[d] * (-pos / shift_sizes[d] + 1);
				else if (pos >= shift_sizes[d])
					shift -= shift_sizes[d] * (pos / shift_sizes[d]);

				shifts[l][e] += shift * shift_strides[d];
			}

			shifts[l][e] += offsets[0][e];
		}
	}

	return location_count;
}

template <typename OffsetVector, typename BlockDimensions>
unsigned long Layout<OffsetVector, BlockDimensions>::get_boundary_membership(Coord& coord)
{
	// Return boundary membership from node coordinates for each node edge
	unsigned long boundary = 0;
	ptrdiff_t pos;
	size_t c = coord[0]; // cell index

	for (size_t e = 0; e < edge_count_by_cell_index[c]; e++)
	{
		for (ptrdiff_t d = DIM_COUNT - 1; d >= 1; d--)
		{
			pos = coord[d] + offsets[c][d][e];
			if (pos < 0 || pos >= block_dimensions[d])
			{
				boundary |= 1 << e;
				break;
			}
		}
	}

	return boundary;
}

template <typename OffsetVector, typename BlockDimensions>
unsigned short Layout<OffsetVector, BlockDimensions>::get_location_index(Coord& coord, size_t offset_strides[], vector<size_t> ranges[])
{
	// Return offset index from coordinates
	unsigned short index = 0;

	for (ptrdiff_t d = DIM_COUNT - 1; d >= 0; d--)
	{
		unsigned short i = 0;
		while (coord[d] >= ranges[d][i]) i++;

		index += i * (unsigned short)offset_strides[d];
	}

	return index;
}

template <typename OffsetVector, typename BlockDimensions>
unsigned short Layout<OffsetVector, BlockDimensions>::get_block_location_index(Coord& coord)
{
	return get_location_index(coord, block_offset_strides, block_ranges);
}

template <typename OffsetVector, typename BlockDimensions>
unsigned short Layout<OffsetVector, BlockDimensions>::get_node_location_index(Coord& coord)
{
	size_t c = coord[0];
	return get_location_index(coord, offset_strides[c], ranges[c]);
}

template <typename OffsetVector, typename BlockDimensions>
void Layout<OffsetVector, BlockDimensions>::get_block_coord(size_t block_id, Coord& coord)
{
	for (ptrdiff_t d = DIM_COUNT - 1; d >= 0; d--)
	{
		coord[d] = block_id / block_strides[d];
		block_id -= coord[d] * block_strides[d];
	}
}


template <typename OffsetVector, typename BlockDimensions>
void Layout<OffsetVector, BlockDimensions>::get_node_coord(size_t block_id, size_t node_subid, Coord& coord)
{
	for (ptrdiff_t d = DIM_COUNT - 1; d >= 0; d--)
	{
		coord[d] = node_subid / block_dimension_strides[d];
		node_subid -= coord[d] * block_dimension_strides[d];
	}
}

template <typename OffsetVector, typename BlockDimensions>
void Layout<OffsetVector, BlockDimensions>::get_node_block_index(size_t node_id, size_t& block_id, size_t& node_subid)
{
	// Get the id of the block that a node belongs to and its index in that block
	block_id = 0;
	node_subid = 0;
	size_t pos;
	for (ptrdiff_t d = DIM_COUNT - 1; d >= 0; d--)
	{
		pos = node_id / size_strides[d];
		node_id -= pos * size_strides[d];

		block_id += pos / block_dimensions[d] * block_strides[d];
		node_subid += pos % block_dimensions[d] * block_dimension_strides[d];
	}
}

template <typename OffsetVector, typename BlockDimensions>
size_t Layout<OffsetVector, BlockDimensions>::get_node_id(size_t node_id)
{
	// Recalculate the node index using the new grid dimensions
	if (!sizes_changed)
		return node_id;

	size_t new_node_id = 0;
	size_t pos;
	for (ptrdiff_t d = DIM_COUNT - 1; d >= 0; d--)
	{
		pos = node_id / original_size_strides[d];
		node_id -= pos * original_size_strides[d];
		new_node_id += pos * size_strides[d];
	}
	return new_node_id;
}

#endif
