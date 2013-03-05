// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Steerable filter extractor based on the Oxford VGG's binary.
//

#ifndef EXTRACTOR_STEERABLE_FILTER_EXTRACTOR_H
#define EXTRACTOR_STEERABLE_FILTER_EXTRACTOR_H

#include "extractor/oxford-vgg-extractor.h"
#include "point_set/point.h"

using namespace libpmk;

namespace libpmk_features {

/// Steerable filter extractor based on the Oxford VGG's binary.
class SteerableFilterExtractor : public OxfordVGGExtractor {
public:
  SteerableFilterExtractor();
  virtual ~SteerableFilterExtractor() { }

protected:
  virtual bool IsValidPoint(const libpmk::Point& point) const;
};
}  // end namespace libpmk_features
#endif  // EXTRACTOR_STEERABLE_FILTER_EXTRACTOR_H
