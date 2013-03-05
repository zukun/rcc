// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// SimplePyramidMaker takes a vocabulary tree and an image, and outputs
// a pyramid corresponding to that image based on the structure of the
// input vocabulary tree. No need to instantiate SimplePyramidMaker;
// its methods are all static.
//

#ifndef TREES_SIMPLE_PYRAMID_MAKER_H
#define TREES_SIMPLE_PYRAMID_MAKER_H

#include <vector>
#include "point_set/point-ref.h"
#include "util/sparse-vector.h"
#include "histograms/multi-resolution-histogram.h"
#include "trees/incremental-vocabulary-tree.h"

using namespace std;
using namespace libpmk;

namespace incremental_vtree {

class SimplePyramidMaker {
public:

  /// Allocates new memory for a pyramid, and returns it.
  static MultiResolutionHistogram* MakePyramid(
    const IncrementalVocabularyTree& tree,
    const vector<PointRef>& image);

  static void GetMembershipPath(const IncrementalVocabularyTree& tree,
                                const Point& f, LargeIndex* out_path,
                                vector<double>* out_bin_sizes);

private:
  SimplePyramidMaker() { }
};

}  // end namespace incremental_vtree


#endif  // TREES_SIMPLE_PYRAMID_MAKER_H
