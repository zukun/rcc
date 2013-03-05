// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This is a uniform 3D cuboidal kernel density estimator. It is a
// radially symmetric nonnegative function centered at 0 and
// integrating to 1.
//
// The bandwidth is the length of the side of the rectangular prism.
//

#ifndef MULTIOBJ_SHAPE_MODEL_UNIFORM_BALLOON_DENSITY_ESTIMATOR_H
#define MULTIOBJ_SHAPE_MODEL_UNIFORM_BALLOON_DENSITY_ESTIMATOR_H

namespace libpmk_shape_model {

class UniformBalloonDensityEstimator {
public:
  UniformBalloonDensityEstimator(double bandidth_x, double bandwidth_y,
                                 double bandwidth_z);

  // Computes the value of a uniform kernel with the given bandwidth,
  // at the given x/y coordinate. The balloon factor means that it
  // will divide each of the coordinates by that number before
  // performing the computation.
  double ComputeValue(double x_position, double y_position,
                      double z_position, double balloon_factor) const;

private:
  double bandwidth_x_;
  double bandwidth_y_;
  double bandwidth_z_;
};
}  // end namespace libpmk_shape_model

#endif  // MULTIOBJ_SHAPE_MODEL_UNIFORM_BALLOON_DENSITY_ESTIMATOR_H
