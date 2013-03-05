// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// A simple extractor that just returns the X/Y coordinates as a feature.
//

#ifndef EXTRACTOR_LOCATION_EXTRACTOR_H
#define EXTRACTOR_LOCATION_EXTRACTOR_H

#include <string>
#include <vector>
#include "core/interest-point.h"
#include "extractor/extractor.h"
#include "point_set/point-set.h"

using namespace std;
using namespace libpmk;

namespace libpmk_features {

/// Returns X/Y coordinates as features.
/**
 * This extractor simply makes 2D features, which are the location of
 * each of the keypoints.
 */
class LocationExtractor : public Extractor {
public:
  virtual ~LocationExtractor() { }
  virtual PointSet ExtractFeatures(
    const string& file, vector<InterestPoint>* points,
    vector<InterestPoint>* removed = NULL) const;
};

}  // end namespace libpmk_features

#endif  // EXTRACTOR_LOCATION_EXTRACTOR_H
