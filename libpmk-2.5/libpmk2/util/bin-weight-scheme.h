// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef UTIL_BIN_WEIGHT_SCHEME_H
#define UTIL_BIN_WEIGHT_SCHEME_H

namespace libpmk {

// GLOBAL: Compute bin sizes over all the cluster data, then simply
//         apply those bin sizes to the pyramids we make.
// INPUT_SPECIFIC: For a particular pyramid, when computing bin sizes
//         we only count those points that are included in this pyramid.
enum BinWeightScheme {
   BIN_WEIGHT_GLOBAL,
   BIN_WEIGHT_INPUT_SPECIFIC
};
}  // namespace libpmk

#endif  // UTIL_BIN_WEIGHT_SCHEME_H
