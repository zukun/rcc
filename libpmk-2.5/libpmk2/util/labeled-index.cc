// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include "util/labeled-index.h"

namespace libpmk {

bool LabeledIndex::operator<(const LabeledIndex& other) const {
  return index < other.index;
}

bool LabeledIndex::operator==(const LabeledIndex& other) const {
  return (index == other.index) && (label == other.label);
}

}  // namespace
