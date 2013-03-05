// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdint.h>
#include <cassert>
#include <iostream>
#include "point_set/point-set.h"
#include "point_set/point.h"

namespace libpmk {

PointSet::PointSet(int point_dim) : point_dim_(point_dim) { }

PointSet::~PointSet() {
  clear();
}

PointSet::PointSet(const PointSet& other) : point_dim_(other.point_dim_) {
  for (int ii = 0; ii < (int)other.points_.size(); ++ii) {
    add_point(*(other.points_[ii]));
  }
}

void PointSet::CopyFrom(const PointSet& other) {
  clear();
  point_dim_ = other.point_dim_;
  for (int ii = 0; ii < (int)other.points_.size(); ++ii) {
    add_point(*(other.points_[ii]));
  }
}

void PointSet::clear() {
  for (int ii = 0; ii < (int)points_.size(); ++ii) {
    delete points_[ii];
  }
  points_.clear();
}

const Point& PointSet::point(int index) const {
  return *(points_.at(index));
}

Point* PointSet::mutable_point(int index) const {
  return points_.at(index);
}

Point* PointSet::add_point(const Point& point) {
  Point* added_point = new Point(point);
  points_.push_back(added_point);
  return added_point;
}

Point* PointSet::add_point() {
  Point* added_point = new Point(point_dim_);
  points_.push_back(added_point);
  return added_point;
}

void PointSet::remove_point(int index) {
  Point* to_delete = points_.at(index);
  points_.erase(points_.begin() + index);
  delete to_delete;
}

void PointSet::set_point_dim(int new_dim) {
  assert ((int)points_.size() == 0);
  point_dim_ = new_dim;
}

void PointSet::WriteToStream(ostream& output_stream) const {
  // Write each point set:
  int32_t num_features = size();
  output_stream.write((char *)(&num_features), sizeof(int32_t));

  // Write the features in this point set:
  for (int jj = 0; jj < num_features; ++jj) {
    points_[jj]->WriteToStream(output_stream);
  }

  // Write the weights for this point set:
  for (int jj = 0; jj < num_features; ++jj) {
    float value = points_[jj]->weight();
    output_stream.write((char *)&value, sizeof(float));
  }
}

void PointSet::ReadFromStream(istream& input_stream) {
  // Delete any points we had originally:
  for (int ii = 0; ii < (int)points_.size(); ++ii) {
    delete points_[ii];
  }
  points_.clear();

  int32_t num_features;
  input_stream.read((char *)(&num_features), sizeof(int32_t));

  for (int ii = 0; ii < num_features; ++ii) {
    Point* added = add_point();
    added->ReadFromStream(input_stream);
  }

  for (int ii = 0; ii < num_features; ++ii) {
    float value;
    input_stream.read((char *)(&value), sizeof(float));
    points_[ii]->set_weight(value);
  }

}
}  // namespace libpmk
