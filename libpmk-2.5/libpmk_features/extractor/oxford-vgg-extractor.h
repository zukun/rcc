// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Abstract superclass for extractors that use the Oxford VGG's
// compute_descriptors.ln binary.
//

#ifndef EXTRACTOR_OXFORD_VGG_EXTRACTOR_H
#define EXTRACTOR_OXFORD_VGG_EXTRACTOR_H

#include <string>
#include <vector>
#include <Magick++.h>
#include "extractor/image-extractor.h"
#include "point_set/point-set.h"

using namespace std;
using namespace libpmk;
using namespace Magick;

namespace libpmk_features {

/// Base class for Oxford VGG extractors.
/**
 * This extractor is a generic base class for all extractors using the
 * Oxford VGG's "compute_descriptors.ln" binary.
 */
class OxfordVGGExtractor : public ImageExtractor {
public:
  virtual ~OxfordVGGExtractor() { }
  void SetBinaryPath(const string& path);
  virtual void GetFeatures(const string& filename,
                           const Magick::Image& image,
                           const vector<InterestPoint>& points,
                           PointSet* out_features) const;

protected:
  string binary_path_;
  string flags_;

  OxfordVGGExtractor(const string& flags);

  /// Set an option to truncate the resulting vectors.
  /* If you want to truncate the resulting feature vector and keep only the
   * first couple dims.
   */
  void SetTruncate(bool trunc) { truncate_vector_ = trunc; }
  void SetTruncateDim(int dim);
  bool ShouldTruncate() const { return truncate_vector_; }

private:
  OxfordVGGExtractor();

  // These are actually only used for PCA-SIFT.
  bool truncate_vector_;
  int truncated_dim_;
};


}  // end namespace libpmk_features

#endif  // EXTRACTOR_OXFORD_VGG_EXTRACTOR_H
