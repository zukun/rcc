// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <vector>

#include "pyramids/input-specific-vg-pyramid-maker.h"

#include "clustering/hierarchical-clusterer.h"
#include "histograms/multi-resolution-histogram.h"
#include "point_set/point-set.h"
#include "util/distance-computer.h"
#include "util/sparse-vector.h"

namespace libpmk {

InputSpecificVGPyramidMaker::
InputSpecificVGPyramidMaker(const HierarchicalClusterer& c,
                            const DistanceComputer& distance_computer) :
  VGPyramidMaker(c, distance_computer) { }

MultiResolutionHistogram* InputSpecificVGPyramidMaker::
MakePyramid(const PointSet& point_set) {
  MultiResolutionHistogram* pyramid = new MultiResolutionHistogram();

  for (int ii = 0; ii < point_set.size(); ++ii) {
    // This describes the path down the hierarchical cluster tree that
    // this point is placed in.
    LargeIndex membership_path;

    // Element jj of the <distances> vector is the distance from
    // all_points[ii] to its respective cluster center at the jj'th
    // level.
    vector<double> distances;

    GetMembershipPath(point_set.point(ii), &membership_path, &distances);

    LargeIndex path;
    Bin* finger;
    Bin root_bin(path);
    root_bin.set_count(point_set.point(ii).weight());
    root_bin.set_size(distances.at(0));
    finger = pyramid->add_bin(root_bin);

    for (int jj = 1; jj < (int)membership_path.size(); ++jj) {
      path.push_back(membership_path[jj]);
      Bin new_bin(path);
      new_bin.set_count(point_set.point(ii).weight());
      new_bin.set_size(distances.at(jj));
      finger = pyramid->add_bin(new_bin, finger);
    }
  }
  return pyramid;
}

}  // namespace libpmk
