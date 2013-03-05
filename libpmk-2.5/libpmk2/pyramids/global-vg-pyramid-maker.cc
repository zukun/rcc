// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <cfloat>
#include <vector>
#include <fstream>
#include <ext/hash_set>

#include "pyramids/global-vg-pyramid-maker.h"

#include "clustering/hierarchical-clusterer.h"
#include "histograms/multi-resolution-histogram.h"
#include "point_set/point-set-list.h"
#include "util/distance-computer.h"
#include "util/sparse-vector.h"

using namespace std;
using namespace __gnu_cxx;

namespace libpmk {

GlobalVGPyramidMaker::GlobalVGPyramidMaker(const HierarchicalClusterer& c,
                                           const DistanceComputer& dist) :
  VGPyramidMaker(c, dist), preprocessed_(false) { }

GlobalVGPyramidMaker::~GlobalVGPyramidMaker() { }

void GlobalVGPyramidMaker::Preprocess(const PointSetList& point_sets) {

  vector<PointRef> all_points;
  point_sets.GetPointRefs(&all_points);

  // Records, for each node in the cluster tree, the set of all
  // PointRef indices that the node owns.
  hash_map<int, list<int>* > inverted_tree;

  for (int ii = 0; ii < (int)all_points.size(); ++ii) {
    // We explicitly use VGPyramidMaker::GetMembershipPath because it
    // will search every node in the tree, while
    // GlobalVGPyramidMaker's will ignore those that are not used.
    // However, at this point, GlobalVGPyramidMaker doesn't know which
    // ones are not used, so its output would be invalid.
    LargeIndex membership_path;
    vector<double> distances;
    VGPyramidMaker::GetMembershipPath(all_points[ii].point(),
                                      &membership_path, &distances);

    // Walk down the membership path, using the IDs of the nodes along
    // the path to add entries into inverted_tree.
    // Begin at the root of the tree.
    int node_id = 0;
    const PointTreeNode* node = centers_.node(node_id);

    hash_map<int, list<int>* >::iterator iter = inverted_tree.find(node_id);
    // If we've never put a point in this cluster before, create an entry:
    if (iter == inverted_tree.end()) {
      inverted_tree[node_id] = new list<int>(1, ii);
    } else {
      // Otherwise, just add on to the existing entry.
      inverted_tree[node_id]->push_back(ii);
    }

    // Walk down the tree (start at 1 because we already did the root):
    for (int jj = 1; jj < (int)membership_path.size(); ++jj) {
      int child_index = membership_path[jj];
      node = centers_.node(node->child(child_index));

      hash_map<int, list<int>* >::iterator iter =
        inverted_tree.find(node_id);
      // If we've never put a point in this cluster before, create an entry:
      if (iter == inverted_tree.end()) {
        inverted_tree[node_id] = new list<int>(1, ii);
      } else {
        // Otherwise, just add on to the existing entry.
        inverted_tree[node_id]->push_back(ii);
      }
    }
  }


  // Now the inverted tree is complete. We traverse the tree once, and
  // for each node, we compute the pairwise distances and store the result
  // in node_sizes_. Note that there can be NULL pointers since not
  // every node is guaranteed to be hit; in such cases, simply ignore
  // that node. To save memory, we can delete the pointers as soon as
  // we're done with them.
  // TODO(jjl): Faster way to do this incrementally? If we start from
  // the leaves...

  for (hash_map<int, list<int>* >::iterator iter = inverted_tree.begin();
       iter != inverted_tree.end(); ++iter) {
    // Find the max pairwise distance.
    double max_size = 0;
    int node_id = iter->first;
    list<int>* indices = iter->second;

    // If it's not NULL, that means this node contains points.
    // If it is NULL, that means it doesn't contain points and we should
    // ignore it.
    if (indices != NULL) {
      for (list<int>::iterator iter1 = indices->begin();
           iter1 != indices->end(); ++iter1) {
        for (list<int>::iterator iter2 = indices->begin();
             iter2 != indices->end(); ++iter2) {
          double distance =
            distance_computer_.ComputeDistance(all_points[*iter1].point(),
                                               all_points[*iter2].point());
          if (distance > max_size) {
            max_size = distance;
          }
        }
      }

      node_sizes_[node_id] = max_size;
      delete indices;
    }
  }
  inverted_tree.clear();
  preprocessed_ = true;
}


MultiResolutionHistogram* GlobalVGPyramidMaker::
MakePyramid(const PointSet& point_set) {
  assert(preprocessed_);

  MultiResolutionHistogram* pyramid = new MultiResolutionHistogram();

  for (int ii = 0; ii < point_set.size(); ++ii) {
    // Place the point into the appropriate set of bins:
    LargeIndex membership_path;
    vector<double> distances;
    GlobalVGPyramidMaker::GetMembershipPath(point_set.point(ii),
                                            &membership_path, &distances);

    LargeIndex path;
    Bin* finger;         // Current position in the local pyramid
    const PointTreeNode* global_node = centers_.node(0);

    Bin root_bin(path);
    root_bin.set_count(point_set.point(ii).weight());

    // node_sizes_[global_finger->id()] is guaranteed to exist, since
    // GlobalVGPyramidMaker::GetMembershipPath() will only return paths
    // for which preprocessed points fell into.
    root_bin.set_size(node_sizes_[global_node->id()]);
    finger = pyramid->add_bin(root_bin);

    for (int jj = 1; jj < (int)membership_path.size(); ++jj) {
      int child_index = membership_path[jj];
      path.push_back(child_index);
      global_node = centers_.node(global_node->child(child_index));
      Bin new_bin(path);

      new_bin.set_count(point_set.point(ii).weight());
      new_bin.set_size(node_sizes_[global_node->id()]);

      finger = pyramid->add_bin(new_bin, finger);

    }
  }
  return pyramid;
}


void GlobalVGPyramidMaker::ReadFromStream(istream& input_stream) {
  int32_t node_info_size;
  input_stream.read((char *)&node_info_size, sizeof(int32_t));

  assert(node_info_size > 0);
  for (int ii = 0; ii < node_info_size; ++ii) {
    int32_t node_id;
    double node_size;
    input_stream.read((char *)&node_id, sizeof(int32_t));
    input_stream.read((char *)&node_size, sizeof(double));
    assert(node_sizes_.find(node_id) == node_sizes_.end());
    node_sizes_[node_id] = node_size;
  }
  preprocessed_ = true;
}

void GlobalVGPyramidMaker::ReadFromFile(const char* filename) {
  ifstream input_stream(filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

void GlobalVGPyramidMaker::WriteToStream(ostream& output_stream) const {
  assert(preprocessed_);
  int32_t node_info_size = node_sizes_.size();
  output_stream.write((char *)&node_info_size, sizeof(int32_t));
  for (hash_map<int, double>::const_iterator iter = node_sizes_.begin();
       iter != node_sizes_.end(); ++iter) {
    int32_t node_id = iter->first;
    double node_size = iter->second;
    output_stream.write((char *)&node_id, sizeof(int32_t));
    output_stream.write((char *)&node_size, sizeof(double));
  }
}

void GlobalVGPyramidMaker::WriteToFile(const char* filename) const {
  ofstream output_stream(filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}



bool GlobalVGPyramidMaker::GetMembershipPath(const Point& f,
                                             LargeIndex* out_path,
                                             vector<double>* out_distances) {
  assert(NULL != out_path);
  assert(NULL != out_distances);
  out_path->clear();
  out_distances->clear();

  int node_id = 0;  // Start at the root
  const PointTreeNode* node = centers_.node(node_id);


  // For each level, find the bin center closest to f, and add it
  // to <answer>. We can skip the first computation since it's always 0.
  out_path->push_back(0);
  out_distances->push_back(
    distance_computer_.ComputeDistance(f, node->point()));

  while (node->has_child()) {
    double min_distance = DBL_MAX;
    int best_index = -1;
    const PointTreeNode* best_child = NULL;

    for (int jj = 0; jj < node->child_size(); ++jj) {
      const PointTreeNode* child = centers_.node(node->child(jj));

      // Was any point ever assigned to this node? If so, then we'll
      // consider it. If not, do not bother checking.
      hash_map<int, double>::iterator iter = node_sizes_.find(child->id());
      if (iter != node_sizes_.end()) {
        double distance =
          distance_computer_.ComputeDistance(f, child->point(), min_distance);
        if (distance < min_distance) {
          min_distance = distance;
          best_index = jj;
          best_child = child;
        }
      }
    }
    assert(best_index != -1);
    assert(best_child != NULL);

    out_path->push_back(best_index);
    out_distances->push_back(min_distance);
    node = best_child;
  }

  assert(out_path->size() == out_distances->size());
  return true;
}

}  // namespace libpmk
