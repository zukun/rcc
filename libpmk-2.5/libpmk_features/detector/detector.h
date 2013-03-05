// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Contains the abstract Detector class, as well as some stock detectors
// that make system calls to the Oxford VGG binaries.
//

#ifndef DETECTOR_DETECTOR_H
#define DETECTOR_DETECTOR_H

#include <vector>
#include <string>
#include "core/interest-point.h"

using namespace std;

namespace libpmk_features {

#define HARRIS          0
#define HARRIS_LAPLACE  1
#define HESSIAN_LAPLACE 2
#define HARRIS_AFFINE   3
#define HESSIAN_AFFINE  4

/// The Detector superclass, which converts images to lists of InterestPoints.
/**
 * This abstract class only one method to implement: DetectInterestPoints.
 *
 * DetectInterestPoints performs the actual interest point detection. The
 * input is a filename, and the output is a vector of InterestPoints.
 *
 * The most common use for this class is to detect interest points on
 * images.  Rather than directly subclassing Detector, it is a better
 * idea to subclass ImageDetector for that purpose, since it provides
 * a lot more functionality; it actually implements
 * DetectInterestPoints for you. See image-detector.h for details.
 */
class Detector {
public:
  Detector();
  virtual ~Detector() { }

  /// Populates out_points with interest points. It must not be NULL.
  /**
   * out_points will be cleared first. The given filename must
   * exist and be a valid file.
   */
  virtual void DetectInterestPoints(
    const string& filename,
    vector<InterestPoint>* out_points) const = 0;
};

}  // namespace libpmk_features

#endif  // DETECTOR_DETECTOR_H
