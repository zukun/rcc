// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef DETECTOR_GRID_DETECTOR_H
#define DETECTOR_GRID_DETECTOR_H

#include <Magick++.h>
#include "detector/image-detector.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;


namespace libpmk_features {

/// Detector for sampling circles of fixed radius on a uniform grid.
class GridDetector : public ImageDetector {
public:
  GridDetector(int radius, int horizontal_spacing, int vertical_spacing);
  virtual ~GridDetector() { }

  virtual void GetInterestPoints(const string& image_name,
                                 const Magick::Image& image,
                                 vector<InterestPoint>* out_points) const;

protected:
  virtual void SetAcceptedFormats();

private:
  int radius_;
  int horizontal_spacing_;
  int vertical_spacing_;
};
}  // end namespace libpmk_features


#endif  // DETECTOR_GRID_DETECTOR_H
