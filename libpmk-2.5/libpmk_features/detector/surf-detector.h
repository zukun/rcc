// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Makes system calls to the surf.ln binary to get SURF interest
// points.
//

#ifndef DETECTOR_SURF_DETECTOR_H
#define DETECTOR_SURF_DETECTOR_H

#include <string>
#include <vector>
#include <Magick++.h>
#include "detector/image-detector.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

/// ETH's SURF detector.
/**
 * This detector makes system calls.
 *
 * The caller should specify the path to the "surf.ln" binary,
 * either through the constructor or through SetBinaryPath().
 *
 * If none of the Set...() methods are called, it uses the
 * default values of the SURF binary.
 */
class SURFDetector : public ImageDetector {
public:
  /**
   * By default, this assumes that "surf.ln" is present in the
   * current directory.
   */
  SURFDetector();
  virtual ~SURFDetector() { }

  SURFDetector(const string& binary_path);
  void SetBinaryPath(const string& binary_path);

  /// Sets the blob response threshold (default 1000).
  void SetThreshold(int threshold);
  
  virtual void GetInterestPoints(const string& image_filename,
                                 const Image& image,
                                 vector<InterestPoint>* out_points) const;

private:
  string binary_path_;
  int threshold_;
  bool extended_;
};

}  // end namespace libpmk_features
#endif  // DETECTOR_SURF_DETECTOR_H
