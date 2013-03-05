// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <math.h>
#include "core/interest-point.h"

namespace libpmk_features {

bool InterestPoint::IsCircle() const {
  return (b == 0) && (a == c);
}

double InterestPoint::CircleRadius() const {
  if (a > 0) {
    return sqrt(1.0 / a);
  }
  return 0;
}

}  // namespace libpmk_features
