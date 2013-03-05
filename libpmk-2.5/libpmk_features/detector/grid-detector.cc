// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//


#include <assert.h>
#include <stdio.h>
#include <vector>
#include <Magick++.h>
#include "detector/grid-detector.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

void GridDetector::SetAcceptedFormats() {
  AddAcceptedFormat("png");
  AddAcceptedFormat("tiff");
  AddAcceptedFormat("jpg");
  AddAcceptedFormat("gif");
  AddAcceptedFormat("ppm");
  AddAcceptedFormat("pgm");
}

GridDetector::GridDetector(int radius, int horizontal_spacing,
                           int vertical_spacing) :
  ImageDetector("png"), radius_(radius),
  horizontal_spacing_(horizontal_spacing),
  vertical_spacing_(vertical_spacing) {
  SetAcceptedFormats();
}

void GridDetector::GetInterestPoints(
  const string&, const Image& image,
  vector<InterestPoint>* out_points) const {
  assert(NULL != out_points);
  out_points->clear();

  int width = (int)image.columns();
  int height = (int)image.rows();

  assert(width >= 2 * radius_);
  assert(height >= 2 * radius_);
  int x = radius_;
  int end_x = width - radius_;
  int end_y = height - radius_;
  float factor = 1.0 / (radius_ * radius_);

  while (x <= end_x) {
    int y = radius_;
    while (y <= end_y) {
      InterestPoint pp(x, y, factor, 0, factor);
      out_points->push_back(pp);
      y += vertical_spacing_;
    }
    x += horizontal_spacing_;
  }
}

}  // end namespace libpmk_features
