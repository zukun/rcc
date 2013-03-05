// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <iostream>
#include "point_set/point.h"

using namespace libpmk;

namespace libpmk {

Point::Point(int dim) : weight_(1), features_(dim, 0) { }

Point::Point(const Point& other) :
  weight_(other.weight_), features_(other.features_) { }

void Point::CopyFrom(const Point& other) {
  weight_ = other.weight_;
  features_ = other.features_;
}


float Point::feature(int index) const {
  return features_.at(index);
}

void Point::set_feature(int index, float value) {
  features_.at(index) = value;
}

float& Point::operator[](int index) {
  return features_[index];
}

float Point::operator[](int index) const {
  return features_[index];
}

void Point::WriteToStream(ostream& output_stream) const {
  for (int kk = 0; kk < (int)features_.size(); ++kk) {
    float value = features_[kk];
    output_stream.write((char *)&value, sizeof(float));
  }
}

void Point::ReadFromStream(istream& input_stream) {
  int dim = (int)features_.size();
  for (int kk = 0; kk < dim; ++kk) {
    float value;
    input_stream.read((char *)(&value), sizeof(float));
    features_[kk] = value;
  }
}
}  // namespace libpmk
