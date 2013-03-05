// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef PYRAMIDS_VG_PYRAMID_MAKER_H
#define PYRAMIDS_VG_PYRAMID_MAKER_H

#include "clustering/hierarchical-clusterer.h"
#include "histograms/multi-resolution-histogram.h"
#include "point_set/point.h"
#include "point_set/point-set.h"
#include "point_set/point-set-list.h"
#include "pyramids/pyramid-maker.h"
#include "util/distance-computer.h"
#include "util/sparse-vector.h"

namespace libpmk {
/// Abstract interface for vocabulary-guided pyramid makers.
/**
 * The vocabulary-guided pyramid makers here use the output of a
 * hierarchical clustering algorithm to generate pyramids.
 */
class VGPyramidMaker : public PyramidMaker {
public:
  /**
   * Requires a HierarchicalCluster which has already been
   * Preprocess()'d (or ReadFromStream()), and a DistanceComputer
   * which computes the same distances that were used to generate the
   * HierarchicalClusterer's data.
   */
  VGPyramidMaker(const HierarchicalClusterer& c,
                 const DistanceComputer& distance_computer);

  virtual MultiResolutionHistogram* MakePyramid(const PointSet& point_set) = 0;

protected:
  const HierarchicalClusterer& clusterer_;

  /// The centers extracted from clusterer_.
  const Tree<PointTreeNode>& centers_;
  const DistanceComputer& distance_computer_;

  /// \brief For a new feature, find which bin it would be placed in
  /// according to the HierarchicalClusterer.
  /**
   * The LargeIndex returned is the same size as the number of levels
   * in the tree. Each element of the LargeIndex tells you
   * which child index to traverse. The first element is 0 by default, since
   * the root of the tree has no parent. For instance, if the returned
   * LargeIndex is [0 3 9], then the path down the tree is: root R,
   * 3rd child of R, followed by 9th child of that node.
   *
   * The returned vector of doubles gives the distance to the
   * corresponding center at each level, according to
   * distance_computer_. Thus, the first element is the distance from
   * the point <f> to the center of the root node, etc.
   *
   * The pointers must not be NULL.
   *
   * Returns true on success, which is always.
   */
  bool GetMembershipPath(const Point& f, LargeIndex* out_path,
                         vector<double>* out_distances);
};
}  // namespace libpmk

#endif  // PYRAMIDS_INPUT_SPECIFIC_VG_PYRAMID_MAKER_H
