// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of ImageExtractor.
//

#include <string>
#include <vector>
#include "extractor/image-extractor.h"
#include "images/image-loader.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

ImageExtractor::ImageExtractor(const string& preferred_format) :
  Extractor(), image_loader_(preferred_format) { }

PointSet ImageExtractor::ExtractFeatures(
  const string& filename, vector<InterestPoint>* points,
  vector<InterestPoint>* removed_points) const {
  Image image;
  PointSet output(0);
  string image_path;
  bool is_formatted = image_loader_.LoadImage(filename,
                                              &image, &image_path);
    GetFeatures(image_path, image, *points, &output);
  if (!is_formatted) {
    unlink(image_path.c_str());
  }
  FilterPoints(points, &output, removed_points);
  return output;
}

void ImageExtractor::AddAcceptedFormat(const string& format) {
  image_loader_.AddAcceptedFormat(format);
}



}  // end namespace libpmk_features

