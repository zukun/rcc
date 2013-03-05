// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef PYRAMIDS_PYRAMID_MAKER_H
#define PYRAMIDS_PYRAMID_MAKER_H

#include <vector>
#include "point_set/point-set.h"
#include "point_set/point-set-list.h"
#include "histograms/multi-resolution-histogram.h"

namespace libpmk {
/// \brief Abstract interface for turning PointSets into
/// MultiResolutionHistograms.
class PyramidMaker {
public:
  PyramidMaker() { }
  virtual ~PyramidMaker() { }

  /// Turn a single PointSet into a MultiResolutionHistogram.
  /**
   * This function allocates memory on its own. It is up to the
   * caller to free it.
   */
  virtual MultiResolutionHistogram* MakePyramid(const PointSet& ps) = 0;

  /// Turn a list of PointSets into a bunch of MultiResolutionHistograms.
  /**
   * It is up to the caller to free the memory.
   */
  vector<MultiResolutionHistogram*> MakePyramids(const PointSetList& psl);
};
}  // namespace libpmk

#endif  // PYRAMIDS_PYRAMID_MAKER_H
