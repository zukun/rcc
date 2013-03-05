// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This defines a patch in the image. A patch is identified by 5 numbers,
// the x/y coordinates and the ellipse parameters:
// a(x-x0)(x-x0) + 2b(x-x0)(y-y0) + c(y-y0)(y-y0) = 1
//

#ifndef CORE_INTEREST_POINT_H
#define CORE_INTEREST_POINT_H

namespace libpmk_features {

/// Defines a patch in an image.
/**
 * A patch is represented by 5 numbers, called x, y, a, b, and c.
 * They define the following ellipse:
 *
 * a(x-x0)^2 + 2b(x-x0)(y-y0) + c(y-y0)^2 = 1
 *
 */
class InterestPoint {
public:
  /// Initializes x,y,a,b,c = 0.
  InterestPoint() : x(0), y(0), a(0), b(0), c(0) { }

  /// Initializes x,y,a,b,c.
  InterestPoint(float in_x, float in_y, float in_a, float in_b, float in_c) :
    x(in_x), y(in_y), a(in_a), b(in_b), c(in_c) { }

  /// Returns true if the region is a circle.
  bool IsCircle() const;

  /// If the region is a circle, this returns the radius.
  /**
   * If it's not a circle, the output is unspecified, so make sure to
   * call IsCircle() first.
   */
  double CircleRadius() const;

  float x;
  float y;
  float a;
  float b;
  float c;
};

}  // namespace libpmk_features

#endif  // CORE_INTEREST_POINT_H
