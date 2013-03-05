// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <math.h>
#include "shape_model/uniform-balloon-density-estimator.h"

namespace libpmk_shape_model {

UniformBalloonDensityEstimator::UniformBalloonDensityEstimator(
  double bandwidth_x, double bandwidth_y, double bandwidth_z) :
  bandwidth_x_(bandwidth_x), bandwidth_y_(bandwidth_y),
  bandwidth_z_(bandwidth_z) { }

double UniformBalloonDensityEstimator::ComputeValue(
  double x_position, double y_position, double z_position,
  double balloon_factor) const {
  //x_position /= balloon_factor;
  //y_position /= balloon_factor;
  //z_position /= balloon_factor;

  double bx = bandwidth_x_ * balloon_factor;
  double by = bandwidth_y_ * balloon_factor;
  double bz = bandwidth_z_ * balloon_factor;

  if (fabs(x_position) <= bx &&
      fabs(y_position) <= by &&
      fabs(z_position) <= bz) {
    return (1.0 / (bx * by * bz));
  }

  return 0;
}


}  // end namespace libpmk_shape_model
