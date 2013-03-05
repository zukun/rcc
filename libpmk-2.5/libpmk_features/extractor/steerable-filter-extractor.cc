// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of SteerableFilterExtractor.
//

#include <math.h>
#include "extractor/steerable-filter-extractor.h"
#include "point_set/point-set.h"

using namespace libpmk;

namespace libpmk_features {
SteerableFilterExtractor::SteerableFilterExtractor()
  : OxfordVGGExtractor("-jla") { }  

bool SteerableFilterExtractor::IsValidPoint(const Point& point) const {
  return !(isnan(point.feature(0)));
}

}  // end namespace libpmk_features
