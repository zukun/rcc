// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of SimplePyramidMaker.
//

#include <assert.h>
#include <cfloat>
#include <vector>

#include "trees/simple-pyramid-maker.h"

#include "point_set/point-ref.h"
#include "util/sparse-vector.h"
#include "histograms/multi-resolution-histogram.h"
#include "trees/incremental-vocabulary-tree.h"
#include "util/distance-computer.h"

using namespace libpmk;

namespace incremental_vtree {

MultiResolutionHistogram* SimplePyramidMaker::MakePyramid(
    const IncrementalVocabularyTree& tree,  const vector<PointRef>& image) {
  MultiResolutionHistogram* pyramid = new MultiResolutionHistogram();
  for (int ii = 0; ii < (int)image.size(); ++ii) {
    const Point& point = image[ii].point();
    LargeIndex membership_path;
    vector<double> bin_sizes;
    GetMembershipPath(tree, point, &membership_path, &bin_sizes);

    LargeIndex path;
    Bin* finger;
    Bin root_bin(path);
    root_bin.set_count(1);
    root_bin.set_size(bin_sizes.at(0));
    finger = pyramid->add_bin(root_bin);

    for (int jj = 1; jj < (int)membership_path.size(); ++jj) {
      path.push_back(membership_path[jj]);
      Bin new_bin(path);
      new_bin.set_count(1);
      new_bin.set_size(bin_sizes.at(jj));
      finger = pyramid->add_bin(new_bin, finger);
    }
  }
  return pyramid;

}


void SimplePyramidMaker::GetMembershipPath(
  const IncrementalVocabularyTree& tree, const Point& f,
  LargeIndex* out_path, vector<double>* out_bin_sizes) {
  assert(NULL != out_path);
  assert(NULL != out_bin_sizes);
  out_path->clear();
  out_bin_sizes->clear();

  L2DistanceComputer distance_computer;

  int node_id = 0;  // Start at the root
  const VisualWord* node = tree.word(node_id);


  // For each level, find the bin center closest to f, and add it
  // to <answer>. We can skip the first computation since it's always 0.
  out_path->push_back(0);
  out_bin_sizes->push_back(node->size());

  while (node->has_child()) {
    double min_distance = DBL_MAX;
    int best_index = -1;
    const VisualWord* best_child = NULL;

    for (int jj = 0; jj < node->child_size(); ++jj) {
      const VisualWord* child = tree.word(node->child(jj));
      double distance =
        distance_computer.ComputeDistance(f, child->center(), min_distance);
      if (distance < min_distance) {
        min_distance = distance;
        best_index = jj;
        best_child = child;
      }
    }
    assert(best_index != -1);
    assert(best_child != NULL);

    out_path->push_back(best_index);
    out_bin_sizes->push_back(best_child->size());
    node = best_child;
  }

  assert(out_path->size() == out_bin_sizes->size());
}


}  // end namespace incremental_vtree
