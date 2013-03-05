// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <cfloat>

#include "pyramids/vg-pyramid-maker.h"

#include "histograms/multi-resolution-histogram.h"
#include "point_set/point.h"
#include "point_set/point-set.h"
#include "util/distance-computer.h"
#include "util/sparse-vector.h"

namespace libpmk {

VGPyramidMaker::VGPyramidMaker(const HierarchicalClusterer& clusterer,
                               const DistanceComputer& distance_computer) :
  clusterer_(clusterer), centers_(clusterer.centers()),
  distance_computer_(distance_computer) { }

bool VGPyramidMaker::GetMembershipPath(const Point& f, LargeIndex* out_path,
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
      double distance =
        distance_computer_.ComputeDistance(f, child->point(), min_distance);
      if (distance < min_distance) {
        min_distance = distance;
        best_index = jj;
        best_child = child;
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
