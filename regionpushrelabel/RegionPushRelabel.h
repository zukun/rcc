/////////////////////////////////////////////////////////////////////////////
// Filename: RegionPushRelabel.h
// Author:   Sameh Khamis
//
// Description: Implementation of a parallel out-of-core cache-oblivious FIFO
//              Push-Relabel algorithm for graphs of a regular (repeating)
//              edge structure, extends the work on grid graphs described
//              in Delong and Boykov, CVPR 2008
/////////////////////////////////////////////////////////////////////////////
#ifndef _REGION_PUSH_RELABEL
#define _REGION_PUSH_RELABEL

#include <boost/thread.hpp>
#include <boost/static_assert.hpp>
using namespace boost;

#define BOOST_PARAMETER_MAX_ARITY 7

#include <boost/parameter/name.hpp>
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/binding.hpp>
namespace param = boost::parameter;

#include <functional>
#include <queue>
#include <list>
using namespace std;

#include "MaxflowSolver.h"
#include "MemoryManager.h"
#include "FixedArray.h"
#include "DoublyLinkedArray.h"
#include "Layout.h"

// Class has 5 required parameters:
// 2 required positional parameters: capacity type and flow type (must be first two)
// 1 required keyword parameter: Layout
// Class also has 6 optional parameters for configuration, all keyword:
// ThreadCount, MaxBlocksPerRegion, DischargesPerBlock, BucketDensity, BlocksPerMemoryPage, GlobalUpdateFrequency

// Template parameter definition using boost parameter
BOOST_PARAMETER_TEMPLATE_KEYWORD(param_layout)
BOOST_PARAMETER_TEMPLATE_KEYWORD(param_thread_count)
BOOST_PARAMETER_TEMPLATE_KEYWORD(param_max_blocks_per_region)
BOOST_PARAMETER_TEMPLATE_KEYWORD(param_discharges_per_block)
BOOST_PARAMETER_TEMPLATE_KEYWORD(param_bucket_density)
BOOST_PARAMETER_TEMPLATE_KEYWORD(param_blocks_per_memory_page)
BOOST_PARAMETER_TEMPLATE_KEYWORD(param_global_update_frequency)

// Parameter signature class
typedef param::parameters<
	param::required<param::deduced<tag::param_layout>, is_base_and_derived<LayoutTag, mpl::_> >,
	param::optional<param::deduced<tag::param_thread_count>, is_base_and_derived<ThreadCountTag, mpl::_> >,
	param::optional<param::deduced<tag::param_max_blocks_per_region>, is_base_and_derived<MaxBlocksPerRegionTag, mpl::_> >,
	param::optional<param::deduced<tag::param_discharges_per_block>, is_base_and_derived<DischargesPerBlockTag, mpl::_> >,
	param::optional<param::deduced<tag::param_bucket_density>, is_base_and_derived<BucketDensityTag, mpl::_> >,
	param::optional<param::deduced<tag::param_blocks_per_memory_page>, is_base_and_derived<BlocksPerMemoryPageTag, mpl::_> >,
	param::optional<param::deduced<tag::param_global_update_frequency>, is_base_and_derived<GlobalUpdateFrequencyTag, mpl::_> >
> RegionPushRelabelParameters;

// Grid Push Relabel class
template <typename CapType, typename FlowType,
	typename A0 = param::void_, typename A1 = param::void_, typename A2 = param::void_,
	typename A3 = param::void_, typename A4 = param::void_, typename A5 = param::void_,
	typename A6 = param::void_>
class RegionPushRelabel : public MaxflowSolver<size_t, CapType, FlowType>
{
private:
	// Template parameter extraction
	typedef typename RegionPushRelabelParameters::bind<A0, A1, A2, A3, A4, A5, A6>::type Arguments;

	typedef typename param::binding<Arguments, tag::param_layout>::type Layout;

	typedef ThreadCount<1> DefaultThreadCount;
	static const char THREAD_COUNT = (char)param::binding<Arguments, tag::param_thread_count, DefaultThreadCount>::type::value;

	typedef MaxBlocksPerRegion<mpl::size<typename Layout::OffsetVector_>::value + 1> DefaultMaxBlocksPerRegion;
	static const size_t MAX_BLOCKS_PER_REGION = param::binding<Arguments, tag::param_max_blocks_per_region, DefaultMaxBlocksPerRegion>::type::value;

	typedef DischargesPerBlock<static_cast<long unsigned int>(0.5 * Layout::NODES_PER_BLOCK * MAX_BLOCKS_PER_REGION)> DefaultDischargesPerBlock;
	static const size_t DISCHARGES_PER_BLOCK = param::binding<Arguments, tag::param_discharges_per_block, DefaultDischargesPerBlock>::type::value;

	typedef BucketDensity<1> DefaultBucketDensity;
	static const size_t BUCKET_DENSITY = param::binding<Arguments, tag::param_bucket_density, DefaultBucketDensity>::type::value;

	typedef BlocksPerMemoryPage<50> DefaultBlocksPerMemoryPage;
	static const unsigned BLOCKS_PER_MEMORY_PAGE = (unsigned)param::binding<Arguments, tag::param_blocks_per_memory_page, DefaultBlocksPerMemoryPage>::type::value;

	typedef GlobalUpdateFrequency<200> DefaultGlobalUpdateFrequency;
	static const size_t GLOBAL_UPDATE_FREQUENCY = param::binding<Arguments, tag::param_global_update_frequency, DefaultGlobalUpdateFrequency>::type::value;

	// More constants
	static const size_t BUCKET_DENSITY_BITS = log_n<BUCKET_DENSITY, 2>::value;
	static const size_t MAX_RELABELS_PER_BLOCK = max_of<Layout::NODES_PER_BLOCK, DISCHARGES_PER_BLOCK>::value;
	static const size_t LOCAL_WORK_THRESHOLD = MAX_BLOCKS_PER_REGION * DISCHARGES_PER_BLOCK * GLOBAL_UPDATE_FREQUENCY;

	// Data type definitions
	typedef pair<size_t, size_t> IntegerPair;

	// Node definition
	struct Node
	{
		size_t distance;
		FlowType preflow;
		CapType residual[Layout::NODE_EDGE_COUNT];
		unsigned char cur_edge;
		unsigned char cell_index;
		unsigned short location_index;
		unsigned long boundary; // bits = Layout::block_edge_count
		bool relabel;

		bool is_active() { return preflow > 0 && distance < layout->node_count; }
	};

	typedef FixedArray<unsigned, Layout::NODES_PER_BLOCK> ActiveList;

	// Block definition
	struct Block
	{
		typename ActiveList::Iterator cur_node;
		unsigned char cur_edge;
		unsigned char region_id;
		bool list_populated;
		unsigned id;
		size_t discharges;

		ActiveList active;
		Node nodes[Layout::NODES_PER_BLOCK];

		bool is_active() { return !active.empty(); }
	};

	static const MemoryManager::int64 BLOCK_SIZE = sizeof(Block);

	// Node compare function for priority queue
	struct NodeCompare : public binary_function<Node*, Node*, bool>
	{
		bool operator()(const Node* node1, const Node* node2) const
		{
			return node1->distance > node2->distance;
		}
	};

	// RegionWorker definition
	class RegionWorker
	{
	private:
		friend class RegionPushRelabel;

		RegionPushRelabel* graph;
		IntegerPair* relabels_list;
		IntegerPair* relabels_iter;

		FlowType flow_to_sink;
		char thread_id;
		size_t region_discharges;
		unsigned region_size;
		Block* cur_block;
		Block** cur_neighbors;

		Block* region[MAX_BLOCKS_PER_REGION];
		vector<Block*> neighbors[MAX_BLOCKS_PER_REGION];
		vector<unsigned long> boundary_mask[MAX_BLOCKS_PER_REGION][Layout::NODES_PER_CELL]; // bits = Layout::NODE_EDGE_COUNT

		deque<Node*> bucket_1[MAX_BLOCKS_PER_REGION];
		deque<Node*> bucket_2[MAX_BLOCKS_PER_REGION];
		priority_queue<Node*, deque<Node*>, NodeCompare> fixed[MAX_BLOCKS_PER_REGION];

		void find_next_relabel_distance(size_t& distance, deque<Node*>* bucket, priority_queue<Node*, deque<Node*>, NodeCompare>* fixed);

		void discharge_region();
		void gap_relabel();
		void relabel_region();
		void discharge();
		bool relabel(Node* node);
		bool is_region_discharged();

	public:
		RegionWorker(RegionPushRelabel* g, char id);
		~RegionWorker();
		void work_loop();
	};

	// Shared variables
	mutex active_mutex;
	DoublyLinkedArray<size_t>* active;

	mutex busy_mutex;
	condition_variable gap_cond;
	condition_variable work_cond;
	bool work_done;
	int busy_count;
	int gap_count;

	mutex data_mutex;
	size_t* label_counts;
	bool gap_found;
	vector<size_t> possible_gaps;
	unsigned* gaps;
	size_t* active_count;
	size_t max_bucket;

	// Main variables
	Layout* layout;
	MemoryManager* memory;
	char* block_owner;
	unsigned short* block_location_index;
	RegionWorker* workers[THREAD_COUNT];
	FlowType flow;
	size_t bucket_count;

	// Functions
	void update_data_sync(RegionWorker& worker);
	void update_region_sync(RegionWorker& worker);
	void wait_for_gap_relabeling();
	void wait_for_work();
	void update_block_gaps();

	void initialize_block(size_t i);
	void populated_active_list(Block* block);

	Block* load_block(size_t i);
	void unload_block(size_t i);

public:
	RegionPushRelabel(long dimensions[]);
	~RegionPushRelabel();

	void add_node(size_t unused_nnodes);
	void add_edge(size_t node_i, size_t node_j, CapType cap, CapType rev_cap);
	void add_terminal_weights(size_t node_id, FlowType src_cap, FlowType snk_cap);

	void compute_maxflow();
	FlowType get_flow();
	void add_constant_to_flow(CapType amount);
	int get_segment(size_t id);
};

// Inline functions
template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE FlowType RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::get_flow()
{
	return flow;
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::add_constant_to_flow(CapType amount)
{
	flow += amount;
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::add_node(size_t unused_nnodes)
{
	// Do nothing!
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE int RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::get_segment(size_t id)
{
	id = layout->get_node_id(id);

	size_t bi, ni;
	layout->get_node_block_index(id, bi, ni);
	Node& node = load_block(bi)->nodes[ni];
	int segment = ((node.distance) < (gaps[bi])) ? 1 : 0;
	unload_block(bi);
	return segment;
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE typename RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::Block* RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::load_block(size_t i)
{
	return (Block*)memory->add_ref(i * BLOCK_SIZE);
}

template <typename CapType, typename FlowType, typename A0, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
INLINE void RegionPushRelabel<CapType, FlowType, A0, A1, A2, A3, A4, A5, A6>::unload_block(size_t i)
{
	memory->remove_ref(i * BLOCK_SIZE);
}

#include "RegionPushRelabel.tpl"

#endif
