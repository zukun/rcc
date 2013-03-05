// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Shape context extractor based on the Oxford VGG's binary.
//

#ifndef EXTRACTOR_SHAPE_CONTEXT_EXTRACTOR_H
#define EXTRACTOR_SHAPE_CONTEXT_EXTRACTOR_H

#include "extractor/oxford-vgg-extractor.h"
#include "point_set/point.h"

using namespace libpmk;

namespace libpmk_features {

/// Shape context extractor based on the Oxford VGG's binary.
class ShapeContextExtractor : public OxfordVGGExtractor {
public:
  ShapeContextExtractor();
  virtual ~ShapeContextExtractor() { }

protected:
  virtual bool IsValidPoint(const libpmk::Point& point) const;
};
}  // end namespace libpmk_features
#endif  // EXTRACTOR_SHAPE_CONTEXT_EXTRACTOR_H
