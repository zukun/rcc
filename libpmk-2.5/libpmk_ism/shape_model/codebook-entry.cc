// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Implementation of CodebookEntry.
//

#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <vector>

#include "shape_model/codebook-entry.h"

#include "point_set/point.h"
#include "point_set/point-set.h"

using namespace std;
using namespace libpmk;

namespace libpmk_shape_model {

CodebookEntry::CodebookEntry() : features_(0), locations_(2) { }

const Point& CodebookEntry::feature(int index) const {
  return features_.point(index);
}

const Point& CodebookEntry::location(int index) const {
  return locations_.point(index);
}

double CodebookEntry::scale(int index) const {
  return scales_.at(index);
}

int CodebookEntry::size() const {
  return (int)features_.size();
}

void CodebookEntry::AppendOccurrence(const Point& feature,
                                     const Point& location,
                                     double scale) {
  if (features_.size() == 0) {
    features_.set_point_dim(feature.size());
  }
  features_.add_point(feature);
  locations_.add_point(location);
  scales_.push_back(scale);
}

void CodebookEntry::RemoveOccurrence(int index) {
  features_.remove_point(index);
  locations_.remove_point(index);
  scales_.erase(scales_.begin() + index);
}

void CodebookEntry::WriteToStream(ostream& output_stream) const {
  int32_t num_occurrences = features_.size();
  output_stream.write((char *)(&num_occurrences), sizeof(int32_t));

  int32_t feature_dim = features_.point_dim();
  output_stream.write((char *)(&feature_dim), sizeof(int32_t));

  features_.WriteToStream(output_stream);
  locations_.WriteToStream(output_stream);
  for (int ii = 0; ii < num_occurrences; ++ii) {
    double scale = scales_[ii];
    output_stream.write((char *)(&scale), sizeof(double));
  }
}

void CodebookEntry::ReadFromStream(istream& input_stream) {
  features_.clear();
  locations_.clear();

  int32_t num_occurrences;
  input_stream.read((char *)(&num_occurrences), sizeof(int32_t));

  int32_t feature_dim;
  input_stream.read((char *)(&feature_dim), sizeof(int32_t));
  features_.set_point_dim(feature_dim);
  features_.ReadFromStream(input_stream);

  locations_.set_point_dim(2);
  locations_.ReadFromStream(input_stream);
  scales_.clear();
  
  for (int ii = 0; ii < num_occurrences; ++ii) {
    double scale;
    input_stream.read((char *)(&scale), sizeof(double));
    scales_.push_back(scale);    
  }
}



}  // end namespace libpmk_shape_model
