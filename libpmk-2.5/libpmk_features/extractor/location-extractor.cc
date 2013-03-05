// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of LocationExtractor.
//

#include <assert.h>
#include "extractor/location-extractor.h"
#include "core/interest-point.h"
#include "point_set/point.h"
#include "point_set/point-set.h"

using namespace std;
using namespace libpmk;

namespace libpmk_features {

PointSet LocationExtractor::ExtractFeatures(
  const string&, vector<InterestPoint>* interest_points,
  vector<InterestPoint>*) const {
  assert(interest_points != NULL);
  PointSet returned(2);
  for (int ii = 0; ii < (int)interest_points->size(); ++ii) {
    Point* added_point = returned.add_point();
    added_point->set_feature(0, interest_points->at(ii).x);
    added_point->set_feature(1, interest_points->at(ii).y);
  }
  return returned;
}
}  // end namespace libpmk_features
