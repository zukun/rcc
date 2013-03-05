// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef UTIL_SPARSE_VECTOR_H
#define UTIL_SPARSE_VECTOR_H

#include <vector>

using namespace std;

namespace libpmk {

typedef vector<int> LargeIndex;
typedef pair<LargeIndex, double> IndexValuePair;
typedef vector<IndexValuePair> SparseVector;

}  // namespace libpmk

#endif  // UTIL_SPARSE_VECTOR_H
