// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef UTIL_LABELED_INDEX_H
#define UTIL_LABELED_INDEX_H

namespace libpmk {
/// An index-label pair.
/**
 * This is a pair of two integers: the <i>index</i>, which will
 * usually refer to an image, and a <i>label</i> which refers to its
 * class.
 */
class LabeledIndex {
public:
  int label;
  int index;

  /// Compares the index only.
  bool operator<(const LabeledIndex& other) const;

  /// Checks both the label and the index.
  bool operator==(const LabeledIndex& other) const;
};
}  // namespace libpmk

#endif  // UTIL_LABELED_INDEX_H
