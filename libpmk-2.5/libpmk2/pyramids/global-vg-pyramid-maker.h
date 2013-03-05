// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef PYRAMIDS_GLOBAL_VG_PYRAMID_MAKER_H
#define PYRAMIDS_GLOBAL_VG_PYRAMID_MAKER_H

#include <vector>
#include <iostream>
#include <set>
#include <ext/hash_map>

#include "clustering/hierarchical-clusterer.h"
#include "histograms/multi-resolution-histogram.h"
#include "point_set/point-set-list.h"
#include "pyramids/vg-pyramid-maker.h"
#include "tree/sparse-tree-node.h"
#include "tree/sparse-tree.cc"
#include "util/distance-computer.h"
#include "util/sparse-vector.h"

using namespace std;
using namespace __gnu_cxx;

namespace libpmk {

/// \brief Makes pyramids with bin sizes determined by a particular
/// set of points.
class GlobalVGPyramidMaker : public VGPyramidMaker {
public:
  GlobalVGPyramidMaker(const HierarchicalClusterer& clusterer,
                       const DistanceComputer& distance_computer);
  ~GlobalVGPyramidMaker();

  /// Initialize the global bin sizes.
  /**
   * This takes O(n^2) time, where n is the total number of points in
   * <point_sets>. This is because at the top level bin, it computes
   * the furthest pair between any of the two points to set the bin
   * size.
   *
   * Rather than doing this every single time, you may also call
   * ReadFromStream() which will read already-preprocessed data.
   */
  void Preprocess(const PointSetList& point_sets);

  virtual MultiResolutionHistogram* MakePyramid(const PointSet& point_set);

  /// Initialize the global bin sizes.
  /**
   * Can be called in lieu of Preprocess().  Aborts if the stream is
   * bad.
   */
  void ReadFromStream(istream& input_stream);

  /// Initialize the global bin sizes.
  /**
   * \sa ReadFromStream
   */
  void ReadFromFile(const char* filename);

  /// Write the global bin sizes to a stream.
  /**
   * <b>This output file format is not compatible with that of libpmk-1.x.</b>
   *
   * Stream format:
   * <ul>
   * <li> (int32) The number of nodes, n
   * <li> For each node n: <ul>
   *   <li> (int32) The node's ID (same as that in the HierarchicalClusterer)
   *   <li> (double) The size of the node
   * </ul></ul>
   *
   * Requires Preprocess() or ReadFromStream() to be called first.
   * Aborts if the stream is bad.
   */
  void WriteToStream(ostream& output_stream) const;

  /// Write the global bin sizes to a file.
  /**
   * \sa WriteToStream
   */
  void WriteToFile(const char* filename) const;

protected:
  /// Get the membership data relative to the global pyramid data.
  /**
   * This function is overridden so that only paths that were
   * present in the point sets that this was constructed with
   * can appear. Otherwise, this function does the same thing
   * as its parent VGPyramidMaker::GetMembershipPath().
   * \sa VGPyramidMaker::GetMembershipPath()
   */
  virtual bool GetMembershipPath(const Point& f, LargeIndex* out_path,
                                 vector<double>* out_distances);

private:
  // This maps a node ID to a double representing the size of that
  // node. Recall that the data used to preprocess a
  // GlobalVGPyramidMaker does not have to be the same as the data
  // used to cluster. Thus, there could be some nodes in the tree
  // which own no points. Those nodes simply do not show up in this
  // hash_map. (having an entry with size=0 is allowable; this is
  // the case for a node with 1 point in it).
  hash_map<int, double> node_sizes_;

  bool preprocessed_;
};
}  // namespace libpmk

#endif  // PYRAMIDS_GLOBAL_VG_PYRAMID_MAKER_H
