// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// SIFT extractor based on the Oxford VGG binary.
//

#ifndef EXTRACTOR_SIFT_EXTRACTOR_H
#define EXTRACTOR_SIFT_EXTRACTOR_H

#include "extractor/oxford-vgg-extractor.h"
#include "point_set/point.h"

using namespace libpmk;

namespace libpmk_features {

/// SIFT extractor based on the Oxford VGG's binary.
class SIFTExtractor : public OxfordVGGExtractor {
public:
  /// Standard 128-d SIFT extractor.
  SIFTExtractor();

  /// A PCA-SIFT descriptor of dimension <pca_dim>.
  SIFTExtractor(int pca_dim);

  virtual ~SIFTExtractor() { }

  void SetPCA(bool use_pca);
  void SetPCADim(int dim) { SetTruncateDim(dim); }

protected:
  virtual bool IsValidPoint(const libpmk::Point& point) const;
};

}  // end namespace libpmk_features
#endif  // EXTRACTOR_SIFT_EXTRACTOR_H
