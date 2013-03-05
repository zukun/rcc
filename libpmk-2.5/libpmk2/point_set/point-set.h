// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
#ifndef POINT_SET_POINT_SET_H
#define POINT_SET_POINT_SET_H

#include <vector>
#include "point_set/point.h"

using namespace std;

namespace libpmk {
/// A data structure representing a list of Points.
/**
 * All features in the set must have the same dimension, and this is
 * determined when the PointSet is constructed. Points may be added,
 * removed, and replaced by index. When points are inserted,
 * PointSet will automatically check to make sure that the points
 * all have the same dimension.
 *
 * This is called a PointSet just to maintain name compatibility with
 * previous versions, even though it is actually an array.
 */
class PointSet {
public:
  /// Creates an initially empty set (such that size() == 0).
  PointSet(int point_dim);
  ~PointSet();

  PointSet(const PointSet& other);
  void CopyFrom(const PointSet& other);

  int size() const { return points_.size(); }
  int point_dim() const { return point_dim_; }

  /// Set the point dim. Only works when size() == 0.
  void set_point_dim(int new_dim);

  /// Returns a const reference to the point at <index>.
  const Point& point(int index) const;
  Point* mutable_point(int index) const;
  const Point& operator[](int index) const { return point(index); }

  /// Adds a copy of the point to the end of the list.
  /**
   * Returns a pointer to the Point that was just added.
   */
  Point* add_point(const Point& point);

  /// Adds a new, empty (all zeroes) Point and returns a pointer to it.
  Point* add_point();

  /// Removes the point at <index>.
  void remove_point(int index);

  /// Deletes all of the points.
  void clear();


  /// Writes the point set to a stream.
  /**
   * The format is as follows:
   * <ul>
   * <li>(int32_t) num_features
   * <li>for each feature:
   *    <ul><li>(float * point_dim) the feature vector</ul>
   * <li>then, again, for each feature:
   *    <ul><li>(float) the feature vector's weight</li></ul>
   *
   * Note that the point dim is not written to the stream.
   */
  void WriteToStream(ostream& output_stream) const;

  /// Reads the point set from a stream.
  /**
   * This function will clear any Features present currently. It will
   * also assume that the incoming PointSet has the proper feature
   * dim (observe that a serialized PointSet doesn't actually record
   * feature dim-- that is up to PointSetList to remember).
   */
  void ReadFromStream(istream& input_stream);

private:
  int point_dim_;
  vector<Point*> points_;
};
}  // namespace libpmk
#endif  // POINT_SET_POINT_SET_H
