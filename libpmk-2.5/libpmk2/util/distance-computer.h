// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef UTIL_DISTANCE_COMPUTER_H
#define UTIL_DISTANCE_COMPUTER_H

#include "point_set/point.h"

namespace libpmk {

/// An abstract interface for computing distance between two Points.
class DistanceComputer {
public:
  virtual ~DistanceComputer();

  /// Compute distance.
  virtual double ComputeDistance(const Point& f1,
                                 const Point& f2) const = 0;

  /// Quickly compute min(actual_distance(f1, f2), max_distance).
  /**
   * This is the same as ComputeDistance, except it will stop
   * calculating things when it knows the distance will be greater
   * than max_distance.
   */
  virtual double ComputeDistance(const Point& f1, const Point& f2,
                                 double max_distance) const;
};

/// Computes L1 distance between two Points.
class L1DistanceComputer : public DistanceComputer {
public:
  virtual double ComputeDistance(const Point& f1, const Point& f2) const;
  virtual double ComputeDistance(const Point& f1, const Point& f2,
                                 double max_distance) const;

};

/// Computes the <b>square</b> of the L2 distance between two Points.
class L2DistanceComputer : public DistanceComputer {
public:
  virtual double ComputeDistance(const Point& f1, const Point& f2) const;
  virtual double ComputeDistance(const Point& f1, const Point& f2,
                                 double max_distance) const;

};
}  // namespace libpmk

#endif  // UTIL_DISTANCE_COMPUTER_H
