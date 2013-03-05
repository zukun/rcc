// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <math.h>
#include <memory>
#include "util/bin-weight-scheme.h"
#include "pyramids/pyramid-matcher.h"
#include "histograms/multi-resolution-histogram.h"

namespace libpmk {

double PyramidMatcher::GetPyramidMatchCost(
  const MultiResolutionHistogram& first,
  const MultiResolutionHistogram& second,
  BinWeightScheme bin_weight_scheme) {
  if (first.size() < second.size()) {
    return MatchPyramids(first, second, COST, bin_weight_scheme);
  } else {
    return MatchPyramids(second, first, COST, bin_weight_scheme);
  }
}

double PyramidMatcher::GetPyramidMatchSimilarity(
  const MultiResolutionHistogram& first,
  const MultiResolutionHistogram& second,
  BinWeightScheme bin_weight_scheme) {
  if (first.size() < second.size()) {
    return MatchPyramids(first, second, SIMILARITY, bin_weight_scheme);
  } else {
    return MatchPyramids(second, first, SIMILARITY, bin_weight_scheme);
  }
}

double PyramidMatcher::MatchPyramids(
  const MultiResolutionHistogram& first,
  const MultiResolutionHistogram& second, MatchReturnType return_type,
  BinWeightScheme bin_weight_scheme) {
  double score = 0;
  double cost = 0;

  // All of the counts in "first"'s bins will be scaled by this amount.
  // The scaling is such that the total counts in the first image is
  // equal to the total counts in the second. This is optional.
  // TODO(jjl): Make this a flag.
  // double count_scale_factor = second.total_counts() / first.total_counts();
  double count_scale_factor = 1;

  vector<MatchNode*> todo;
  MatchNode* root_node = new MatchNode(first.root(),
                                       second.root(), 0, NULL);
  todo.push_back(root_node);

  // Invariant: not counting the root node, all MatchNodes in "todo"
  // must have a valid parent_ pointer. The only MatchNodes that should
  // be in "todo" are ones corresponding to Bins that appear in both
  // <first> and <second>.
  while (!todo.empty()) {
    MatchNode* current = todo.back();

    // Variable naming:
    // "first" and "second" refer to the first and second pyramids.
    // "current" and "next" refer to depth in within a pyramid.
    Bin* current_first_bin = current->node_;
    Bin* current_second_bin = current->counterpart_;

    // If this node is not marked, that means we have not processed
    // its children yet.
    if (!current->marked_) {
      // Add child nodes in <first> to <todo>, if a counterpart to
      // that child exists in <second>. Then process all children.
      // We can check for children in exactly one pass (i.e.,we look
      // at each added child exactly once).
      Bin* next_first_bin = (Bin*)current_first_bin->first_child();
      Bin* next_second_bin = (Bin*)current_second_bin->first_child();

      // This is the index of the last element of the next level's indices.
      // This is useful since all indices are prefixes of the indices in the
      // parent, so when we compare children we only have to compare the
      // last element.
      int next_level = current_first_bin->index().size();

      while (next_first_bin != NULL && next_second_bin != NULL) {
        int next_first_index = next_first_bin->index().at(next_level);
        int next_second_index = next_second_bin->index().at(next_level);
        if (next_first_index == next_second_index) {
          // Found a matching bin:
          MatchNode* next_node = new MatchNode(next_first_bin,
                                               next_second_bin, 0,
                                               current);
          next_first_bin = (Bin*)next_first_bin->next_sibling();
          next_second_bin = (Bin*)next_second_bin->next_sibling();
          todo.push_back(next_node);
        } else if (next_first_index < next_second_index) {
          next_first_bin = (Bin*)next_first_bin->next_sibling();
        } else {
          next_second_bin = (Bin*)next_second_bin->next_sibling();
        }
      }
      current->marked_ = true;
    } else {
      // If the node is marked, that means we have seen this node already,
      // added its children, and processed its children. So we can skip
      // straight to processing the intersection for this node.

      // After all children are processed, process this node.
      // We assume the same process generated both histograms, and thus
      // matching bins will have the same size. This may not be true
      // in the future, so in such a case, we'll have to choose one
      // of the sizes if they differ.
      double bin_size;
      if (bin_weight_scheme == BIN_WEIGHT_GLOBAL) {
        assert(current_first_bin->size() ==
               current_second_bin->size());
        bin_size = current_first_bin->size();
      } else {
        bin_size =
          current_first_bin->size() + current_second_bin->size();
      }
      double weight = 1.0 / (1 + bin_size);

      double first_bin_count = count_scale_factor * current_first_bin->count();
      double second_bin_count = current_second_bin->count();
      
      double intersection = first_bin_count > second_bin_count ?
        second_bin_count : first_bin_count;

      // Now we notify the parent MatchNode that there has been a new
      // intersection:
      if (current->parent_ != NULL) {
        current->parent_->intersection_ += intersection;
      }

      // <intersection> holds the total number of intersecting points at
      // this bin. We have to subtract off all the intersections in child
      // bins, which is summed up in current->intersection_.
      intersection -= current->intersection_;
      score += weight * intersection;
      cost += bin_size * intersection;
      // Done: remove this element from the stack and continue.
      todo.pop_back();
      delete current;
    }
  }

  if (return_type == COST) {
    return cost;
  }
  return score;
}
}  // namespace libpmk
