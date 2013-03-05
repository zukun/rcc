// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdint.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "point_set/mutable-point-set-list.h"
#include "point_set/point-set.h"
#include "point_set/point.h"

namespace libpmk {

MutablePointSetList::~MutablePointSetList() {
  clear();
}

void MutablePointSetList::clear() {
  for (int ii = 0; ii < (int)list_.size(); ++ii) {
    delete list_[ii];
  }
  list_.clear();
}

void MutablePointSetList::ReadFromFile(const char* filename) {
  ifstream input_stream(filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

void MutablePointSetList::ReadFromStream(istream& input_stream) {
  clear();

  assert(input_stream.good());
  int32_t num_pointsets, feature_dim;

  input_stream.read((char *)&num_pointsets, sizeof(int32_t));
  input_stream.read((char *)&feature_dim, sizeof(int32_t));

  // Read in each point set
  for (int ii = 0; ii < num_pointsets; ++ii) {
    PointSet* added = new PointSet(feature_dim);
    added->ReadFromStream(input_stream);
    list_.push_back(added);
  }
}

void MutablePointSetList::ReadSelectionFromStream(istream& input_stream,
                                                  int start,
                                                  int selection_size) {
  assert(input_stream.good());
  clear();
  int32_t num_pointsets, feature_dim;

  input_stream.read((char *)&num_pointsets, sizeof(int32_t));
  input_stream.read((char *)&feature_dim, sizeof(int32_t));

  int end = start + selection_size - 1;
  assert(start >= 0);
  assert(start < num_pointsets);
  assert(end < num_pointsets);
  assert(start <= end);

  // Read in each point set
  for (int ii = 0; ii < num_pointsets; ++ii) {

    // If this point set is in the selection:
    if (ii >= start && ii <= end) {
      PointSet* added = new PointSet(feature_dim);
      added->ReadFromStream(input_stream);
      list_.push_back(added);
    } else if (ii > end) { // if we're past the end of the selection
      break;
    } else { // if we're before the start of the selection
      PointSet dummy(feature_dim);
      dummy.ReadFromStream(input_stream);
    }
  }
}

PointSet* MutablePointSetList::add_point_set(const PointSet& point_set) {
  // If there are points already, make sure that they are the same
  // point dim.
  if (point_set_size() != 0) {
    assert(point_set.point_dim() == point_dim());
  }

  PointSet* added = new PointSet(point_set);
  list_.push_back(added);
  return added;
}

PointSet* MutablePointSetList::add_point_set(int point_dim) {
  if (point_set_size() != 0) {
    assert(point_dim == this->point_dim());
  }

  PointSet* added = new PointSet(point_dim);
  list_.push_back(added);
  return added;
}

int MutablePointSetList::point_set_size() const {
  return list_.size();
}

int MutablePointSetList::point_dim() const {
  if (list_.size() == 0) {
    return 0;
  }

  return list_.at(0)->point_dim();
}

PointSet* MutablePointSetList::mutable_point_set(int index) {
  return list_.at(index);
}

const PointSet& MutablePointSetList::point_set(int index) const {
  return *(list_.at(index));
}

int MutablePointSetList::point_size() const {
  int count = 0;
  for (int ii = 0; ii < (int)list_.size(); ++ii) {
    count += list_.at(ii)->size();
  }
  return count;
}
}  // namespace libpmk
