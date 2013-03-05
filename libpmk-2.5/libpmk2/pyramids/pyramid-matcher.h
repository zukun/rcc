// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
#ifndef PYRAMIDS_PYRAMID_MATCHER_H
#define PYRAMIDS_PYRAMID_MATCHER_H

#include "histograms/bin.h"
#include "histograms/multi-resolution-histogram.h"
#include "util/bin-weight-scheme.h"

namespace libpmk {

/// Matches two MultiResolutionHistograms.
class PyramidMatcher {
public:
  /// Returns the pyramid match cost/distance.
  /**
   * \param bin_weight_scheme depends on how the pyramid was generated.
   * \sa GlobalVGPyramidMaker, InputSpecificVGPyramidMaker
   */
  static double GetPyramidMatchCost(const MultiResolutionHistogram& hist1,
                                    const MultiResolutionHistogram& hist2,
                                    BinWeightScheme bin_weight_scheme);

  /// Returns the pyramid match similarity (kernel value).
  /**
   * \param bin_weight_scheme depends on how the pyramid was generated.
   * \sa GlobalVGPyramidMaker, InputSpecificVGPyramidMaker
   */
  static double GetPyramidMatchSimilarity(
    const MultiResolutionHistogram& hist1,
    const MultiResolutionHistogram& hist2,
    BinWeightScheme bin_weight_scheme);

private:
  enum MatchReturnType { COST, SIMILARITY };
  class MatchNode {
  public:
    MatchNode(Bin* n, Bin* c, double i, MatchNode* p) :
      node_(n), counterpart_(c), intersection_(i), parent_(p),
      marked_(false) { }
    Bin* node_;
    Bin* counterpart_;
    double intersection_;
    MatchNode* parent_;
    bool marked_;
  };

  static double MatchPyramids(const MultiResolutionHistogram& hist1,
                              const MultiResolutionHistogram& hist2,
                              MatchReturnType return_type,
                              BinWeightScheme bin_weight_scheme);
};
}  // namespace libpmk

#endif  // PYRAMIDS_PYRAMID_MATCHER_H
