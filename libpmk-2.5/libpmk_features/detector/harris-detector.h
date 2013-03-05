// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Makes system calls to the Oxford VGG h_affine.ln binary to get
// Harris, Harris-Affine, Harris-Laplace, Hessian-Affine, or
// Hessian-Laplace interest points.
//

#ifndef DETECTOR_HARRIS_DETECTOR_H
#define DETECTOR_HARRIS_DETECTOR_H

#include <string>
#include <vector>
#include <Magick++.h>
#include "detector/image-detector.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

/// Oxford VGG's Harris detector.
/**
 * This detector makes system calls.
 *
 * The caller should specify the path to the "h_affine.ln" binary,
 * either through the constructor, or by calling SetBinaryPath().
 * This detector has 5 schemes available:
 * <li> Harris
 * <li> Harris-Laplace
 * <li> Hessian-Laplace
 * <li> Harris-Affine (default)
 * <li> Hessian-Affine
 * This can be set via the constructor or with SetDetector().
 */
class HarrisDetector : public ImageDetector {
public:
  /**
   * This uses a default threshold of 0, with the HARRIS_AFFINE
   * detector, and assumes that the binary "h_affine.ln" is in the
   * current directory.
   */
  HarrisDetector();
  HarrisDetector(const string& binary_path, int threshold, int detector_type);
  virtual ~HarrisDetector() { }

  // These are the various detector types that this class recognizes.
  static const int kHarris = 0;
  static const int kHarrisLaplace = 1;
  static const int kHessianLaplace = 2;
  static const int kHarrisAffine = 3;
  static const int kHessianAffine = 4;

  void SetBinaryPath(const string& bin_path);
  void SetThreshold(int threshold);
  void SetDetectorType(int detector);

  virtual void GetInterestPoints(const string& image_filename,
                                 const Magick::Image& image,
                                 vector<InterestPoint>* out_points) const;

private:
  string binary_path_;
  int threshold_;
  int detector_;
};

}  // end namespace libpmk_features

#endif  // DETECTOR_HARRIS_DETECTOR_H
