// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of ImageDetector, which contains common routines for
// interest point detection on images.
//

#include <assert.h>
#include <string>
#include <vector>
#include "detector/image-detector.h"
#include "images/image-loader.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

ImageDetector::ImageDetector(const string& preferred_format) :
  Detector(), image_loader_(preferred_format) { }

void ImageDetector::DetectInterestPoints(
  const string& image_filename, vector<InterestPoint>* out_points) const {
  assert(out_points != NULL);
  out_points->clear();

  Image image;
  string image_path;
  bool is_formatted = image_loader_.LoadImage(image_filename,
                                              &image, &image_path);
  GetInterestPoints(image_path, image, out_points);
  if (!is_formatted) {
    unlink(image_path.c_str());
  }
}

void ImageDetector::AddAcceptedFormat(const string& format) {
  image_loader_.AddAcceptedFormat(format);
}



}  // end namespace libpmk_features
