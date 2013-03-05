// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include "extractor/extractor.h"
#include "core/interest-point.h"
#include "point_set/point.h"
#include "point_set/point-set.h"

using namespace libpmk;

namespace libpmk_features {

Extractor::Extractor() { }

bool Extractor::IsValidPoint(const Point&) const {
  return true;
}

void Extractor::FilterPoints(
  vector<InterestPoint>* points,
  PointSet* features,
  vector<InterestPoint>* removed_points) const {
  assert((int)points->size() == features->size());

  if (removed_points != NULL) {
    removed_points->clear();
  }

  for (int ii = features->size() - 1; ii >= 0; --ii) {
    if (!IsValidPoint(features->point(ii))) {
      features->remove_point(ii);
      points->erase(points->begin() + ii);
      if (removed_points != NULL) {
        removed_points->push_back(points->at(ii));
      }
    }
  }
}
}  // namespace libpmk_features
