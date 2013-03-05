// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Makes system calls to the surf.ln binary to get SURF descriptors.
//

#ifndef EXTRACTOR_SURF_EXTRACTOR_H
#define EXTRACTOR_SURF_EXTRACTOR_H

#include <string>
#include <vector>
#include <Magick++.h>
#include "extractor/image-extractor.h"
#include "point_set/point-set.h"

using namespace std;
using namespace libpmk;
using namespace Magick;

namespace libpmk_features {

/// ETH's SURF extractor.
/**
 * This detector makes system calls.
 *
 * The caller should specify the path to the "surf.ln" binary,
 * either through the constructor or through SetBinaryPath().
 *
 * If none of the Set...() methods are called, it uses the
 * default values of the SURF binary.
 *
 * <b>Note</b>: The returned descriptor will actually have a length of
 * 1 more than what you might expect. The first value of each
 * descriptor will always be either -1 or 1, and represents the sign
 * of the Laplacian.
 */
class SURFExtractor : public ImageExtractor {
public:
  SURFExtractor() :
    ImageExtractor("pgm"), binary_path_("./surf.ln"), extended_(false) { }
  SURFExtractor(const string& binary_path) :
    ImageExtractor("pgm"), binary_path_(binary_path), extended_(false) { }

  virtual ~SURFExtractor() { }

  /// Set the path to the surf.ln binary.
  void SetBinaryPath(const string& binary_path);

  /// \brief Set to true if we want the extended 128-dim SURF
  /// descriptor (default false).
  void SetExtended(bool extended) { extended_ = extended; }

  virtual void GetFeatures(const string& filename,
                           const Magick::Image& image,
                           const vector<InterestPoint>& points,
                           PointSet* out_features) const;



protected:
  // The behavior of the surf.ln binary is to return all zeros if the
  // interest point is bad. So that is what gets filtered out by this.
  bool IsValidPoint(const libpmk::Point& point) const;


private:
  string binary_path_;
  bool extended_;
};


}  // end namespace libpmk_features

#endif  // EXTRACTOR_SURF_EXTRACTOR_H

