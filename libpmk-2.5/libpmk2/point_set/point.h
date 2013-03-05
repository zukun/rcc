// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
#ifndef POINT_SET_POINT_H
#define POINT_SET_POINT_H

#include <iostream>
#include <vector>

using namespace std;

namespace libpmk {

/// A generic abstract data structure storing a weighted vector of floats.
/**
 * The size of the vector is determined at construction and can not be
 * changed. The elements in the vector and weight are mutable.
 */
class Point {
public:
  /// Initially sets all elements in the vector to 0 and the weight to 1.
  Point(int dim);

  Point(const Point& other);
  virtual ~Point() { }

  // Sets this point to be a copy of <other>.
  virtual void CopyFrom(const Point& other);

  /// Returns the size of the vector.
  int size() const { return features_.size(); }

  /// Same as size().
  int point_dim() const { return size(); }

  /// Returns the <index>th element with bounds checking.
  float feature(int index) const;

  /// Sets the <index>th element to value, with bounds checking.
  void set_feature(int index, float value);

  float weight() const { return weight_; }
  void set_weight(float new_weight) { weight_ = new_weight; }

  /// Returns a reference to the specified index. No bounds checking.
  float& operator[](int index);

  /// Returns a reference to the specified index. No bounds checking.
  float operator[](int index) const;

  /// Serialize to a stream.
  /**
   * Simply writes the vector of floats to a stream.  Note that this
   * does NOT write the weight or the feature dim.  Writing the
   * weights and feature dim are handled by PointSetList. When
   * overriding this in your own Point classes, you should do it
   * such that you write all of the data except the weight or
   * feature dim. It's OK to include them if you want, but it will
   * be redundant.
   */
  virtual void WriteToStream(ostream& output_stream) const;

  /// Read a vector of <dim> floats from a stream.
  virtual void ReadFromStream(istream& input_stream);

protected:
  float weight_;
  vector<float> features_;
};
}  // namespace libpmk

#endif  // POINT_SET_POINT_H
