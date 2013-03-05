// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <math.h>
#include <assert.h>
#include "util/distance-computer.h"
#include "point_set/point.h"

namespace libpmk {

DistanceComputer::~DistanceComputer() { }

double DistanceComputer::ComputeDistance(const Point& f1, const Point& f2,
                                         double max_distance) const {
  return ComputeDistance(f1, f2);
}

double L1DistanceComputer::ComputeDistance(const Point& f1,
                                           const Point& f2) const {
  assert(f1.size() == f2.size());
  double sum = 0;
  for (int ii = 0; ii < f1.size(); ++ii) {
    sum += fabs(f1[ii] - f2[ii]);
  }
  return sum;
}

double L1DistanceComputer::ComputeDistance(const Point& f1,
                                           const Point& f2,
                                           double max_distance) const {
  assert(f1.size() == f2.size());
  double sum = 0;
  for (int ii = 0; ii < f1.size(); ++ii) {
    sum += fabs(f1[ii] - f2[ii]);
    if (sum > max_distance) {
      return max_distance + 1;
    }
  }
  return sum;
}


double L2DistanceComputer::ComputeDistance(const Point& f1,
                                           const Point& f2) const {
  assert(f1.size() == f2.size());
  double sum = 0;
  for (int ii = 0; ii < f1.size(); ++ii) {
    sum += (f1[ii] - f2[ii]) * (f1[ii] - f2[ii]);
  }
  return sum;
}

double L2DistanceComputer::ComputeDistance(const Point& f1,
                                           const Point& f2,
                                           double max_distance) const {
  assert(f1.size() == f2.size());
  double sum = 0;
  for (int ii = 0; ii < f1.size(); ++ii) {
    sum += (f1[ii] - f2[ii]) * (f1[ii] - f2[ii]);
    if (sum > max_distance) {
      return max_distance + 1;
    }
  }
  return sum;
}

}  // namespace libpmk
