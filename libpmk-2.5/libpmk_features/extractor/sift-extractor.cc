// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of SIFTExtractor.
//

#include <math.h>
#include "extractor/sift-extractor.h"
#include "point_set/point-set.h"

using namespace libpmk;

namespace libpmk_features {

SIFTExtractor::SIFTExtractor() : OxfordVGGExtractor("-sift") { }
SIFTExtractor::SIFTExtractor(int pca_dim) :
  OxfordVGGExtractor("-sift -pca") {
  SetTruncate(true);
  SetTruncateDim(pca_dim);
}

bool SIFTExtractor::IsValidPoint(const Point& point) const {
  if (ShouldTruncate()) {
    return !(isnan(point.feature(0)));
  }
  return point.feature(0) > -100000;
}

void SIFTExtractor::SetPCA(bool use_pca) {
  SetTruncate(use_pca);
  if (use_pca) {
    flags_ = "-sift -pca";
  } else {
    flags_ = "-sift";
  }
}


}  // end namespace libpmk_features
