// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include "point_set/point-ref.h"
#include "point_set/point-set-list.h"
#include "point_set/point.h"

namespace libpmk {

PointRef::PointRef(const PointSetList* data, int point_index) :
  data_(data), point_index_(point_index) {
  data_->GetPointIndices(point_index, &which_point_set_, &which_point_);
}

PointRef::PointRef(const PointSetList* data, int which_point_set,
                   int which_point) :
  data_(data), which_point_set_(which_point_set), which_point_(which_point) {
  int sum = 0;
  assert(which_point_set < data_->point_set_size());
  for (int ii = 0; ii < which_point_set - 1; ++ii) {
    sum += data_->point_set(ii).size();
  }
  sum += which_point;
  point_index_ = sum;
}

PointRef::PointRef(const PointSetList* data, int point_index,
                   int which_point_set, int which_point) :
  data_(data), point_index_(point_index),
  which_point_set_(which_point_set), which_point_(which_point) { }

PointRef::PointRef(const PointRef& other) :
  data_(other.data_), point_index_(other.point_index_),
  which_point_set_(other.which_point_set_),
  which_point_(other.which_point_) { }

const Point& PointRef::point() const {
  return data_->point_set(which_point_set_).point(which_point_);
}

int PointRef::which_point_set() const {
  return which_point_set_;
}

int PointRef::which_point() const {
  return which_point_;
}

int PointRef::point_index() const {
  return point_index_;
}

}  // namespace libpmk
