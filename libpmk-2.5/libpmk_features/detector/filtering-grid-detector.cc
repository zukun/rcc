// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Magick++.h>
#include "detector/filtering-grid-detector.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

void FilteringGridDetector::SetAcceptedFormats() {
  AddAcceptedFormat("png");
  AddAcceptedFormat("tiff");
  AddAcceptedFormat("jpg");
  AddAcceptedFormat("gif");
  AddAcceptedFormat("pnm");
}

FilteringGridDetector::FilteringGridDetector(int radius,
                                             int horizontal_spacing,
                                             int vertical_spacing,
                                             int box_size) :
  ImageDetector("png"),
  grid_detector_(radius, horizontal_spacing, vertical_spacing),
  box_size_(box_size) {
  SetAcceptedFormats();
}

void FilteringGridDetector::GetInterestPoints(
  const string& image_name, const Image& image,
  vector<InterestPoint>* out_points) const {
  vector<InterestPoint> candidates;
  grid_detector_.GetInterestPoints(image_name, image, &candidates);
  

  // Read the image, creating this 2D vector. Entry ii,jj is true if
  // the image pixel at ii,jj (width,height) is equal to 0, and false
  // otherwise. ii and jj increase left-to-right, top-to-bottom.
  int image_width = (int)image.columns();
  int image_height = (int)image.rows();
  vector<vector<bool> > is_empty;
  is_empty.resize(image_width);
  for (int ii = 0; ii < image_width; ++ii) {
    is_empty[ii].resize(image_height);
  }

  for (int ii = 0; ii < image_height; ++ii) {
    for (int jj = 0; jj < image_width; ++jj) {
      ColorRGB rgb_pixel(image.pixelColor(jj, ii));
      if (rgb_pixel.red() == 0 &&
          rgb_pixel.blue() == 0 &&
          rgb_pixel.green() == 0) {
        is_empty[jj][ii] = true;
      } else {
        is_empty[jj][ii] = false;
      }
    }
  }

  int half_box_size = box_size_ / 2;
  int num_ignored_points = 0;
  int num_used_points = 0;
  for (int ii = 0; ii < (int)candidates.size(); ++ii) {
    int candidate_x = (int)candidates[ii].x;
    int candidate_y = (int)candidates[ii].y;

    // Check the box around it to make sure it's not all 0.
    int start_x = candidate_x - half_box_size;
    int end_x = candidate_x + half_box_size;
    int start_y = candidate_y - half_box_size;
    int end_y = candidate_y + half_box_size;

    if (start_x < 0) {
      start_x = 0;
    }
    if (end_x >= image_width) {
      end_x = image_width - 1;
    }
    if (start_y < 0) {
      start_y = 0;
    }
    if (end_y >= image_height) {
      end_y = image_height - 1;
    }

    bool is_blank_box = true;
    for (int xx = start_x; xx <= end_x; ++xx) {
      if (!is_blank_box) {
        break;
      }
      for (int yy = start_y; yy <= end_y; ++yy) {
        if (!is_empty[xx][yy]) {
          is_blank_box = false;
          break;
        }
      }
    }

    if (!is_blank_box) {
      out_points->push_back(candidates[ii]);
      ++num_used_points;
    } else {
      ++num_ignored_points;
    }
  }
}



}  // end namespace libpmk_features
