// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of ShapeContextExtractor.
//

#include "extractor/shape-context-extractor.h"
#include "point_set/point-set.h"

using namespace libpmk;

namespace libpmk_features {
ShapeContextExtractor::ShapeContextExtractor() : OxfordVGGExtractor("-sc") { }

bool ShapeContextExtractor::IsValidPoint(const Point& point) const {
  return point.feature(0) > -100000;
}
}  // end namespace libpmk_features
