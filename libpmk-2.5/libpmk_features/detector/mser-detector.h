// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Makes system calls to the Oxford VGG mser.ln binary to get
// Maximally Stable Extremal Region interest points.
//

#ifndef DETECTOR_MSER_DETECTOR_H
#define DETECTOR_MSER_DETECTOR_H

#include <string>
#include <vector>
#include <Magick++.h>
#include "detector/image-detector.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

/// Oxford VGG's MSER detector.
/**
 * This detector makes system calls.
 *
 * The caller should specify the path to the "mser.ln" binary,
 * either through the constructor or through SetBinaryPath().
 */
class MSERDetector : public ImageDetector {
public:
  /**
   * By default, this assumes that "mser.ln" is present in the
   * current directory.
   */
  MSERDetector();
  virtual ~MSERDetector() { }

  MSERDetector(const string& binary_path);
  void SetBinaryPath(const string& binary_path);

  virtual void GetInterestPoints(const string& image_filename,
                                 const Image& image,
                                 vector<InterestPoint>* out_points) const;

protected:
  virtual void SetAcceptedFormats();

private:
  string binary_path_;
};

}  // end namespace libpmk_features
#endif  // DETECTOR_MSER_DETECTOR_H
