// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This works the same as GridDetector, except that it'll check a
// box around every interest point. If every pixel in that box is 
// black (all 0s), the interest point will be thrown out.

#ifndef DETECTOR_FILTERING_GRID_DETECTOR_H
#define DETECTOR_FILTERING_GRID_DETECTOR_H

#include <Magick++.h>
#include "detector/image-detector.h"
#include "detector/grid-detector.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;


namespace libpmk_features {

/// Detector for sampling circles of fixed radius on a uniform grid.
/**
 * This works the same as GridDetector, except that it'll check a
 * box around every interest point. If every pixel in that box is 
 * black (all 0s), the interest point will be thrown out.
 *
 * The "box_size" parameter specifies the length of one side of the
 * box.
 */
class FilteringGridDetector : public ImageDetector {
public:
  FilteringGridDetector(int radius,
                        int horizontal_spacing,
                        int vertical_spacing,
                        int box_size);
  virtual ~FilteringGridDetector() { }
  virtual void GetInterestPoints(const string& image_name,
                                 const Image& image,
                                 vector<InterestPoint>* out_points) const;

protected:
  virtual void SetAcceptedFormats();

private:
  GridDetector grid_detector_;
  int box_size_;
  
};
}  // end namespace libpmk_features


#endif  // DETECTOR_FILTERING_GRID_DETECTOR_H
