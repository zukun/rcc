// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdint.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include "point_set/point-set-list.h"
#include "point_set/point-set.h"
#include "point_set/point-ref.h"
#include "point_set/point.h"

namespace libpmk {

void PointSetList::WriteToFile(const char* output_file) const {
  ofstream output_stream(output_file, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}

void PointSetList::WriteToStream(ostream& output_stream) const {
  WriteHeaderToStream(output_stream);
  WritePointSetsToStream(output_stream);
}

void PointSetList::WriteHeaderToStream(ostream& output_stream) const {
  assert(output_stream.good());
  int32_t num_pointsets = point_set_size();
  int32_t point_dim = this->point_dim();

  // Header: num point sets and point dim
  output_stream.write((char *)&num_pointsets, sizeof(int32_t));
  output_stream.write((char *)&point_dim, sizeof(int32_t));

}

void PointSetList::WritePointSetsToStream(ostream& output_stream) const {
  assert(output_stream.good());

  int32_t num_pointsets = point_set_size();
  assert(num_pointsets > 0);

  for (int ii = 0; ii < num_pointsets; ++ii) {
    point_set(ii).WriteToStream(output_stream);
  }
}

const Point& PointSetList::point(int index) const {
  int which_point_set = 0, which_point = 0;
  GetPointIndices(index, &which_point_set, &which_point);
  return point_set(which_point_set).point(which_point);
}

bool PointSetList::GetPointIndices(int index, int* which_point_set,
                                   int* which_point) const {
  int sum = 0;
  bool found = false;
  int previous_sum = 0;

  // On the iith iteration, sum should contain the total number of points
  // in the 0 to iith point sets.
  for (int ii = 0; ii < point_set_size(); ++ii) {
    sum += point_set(ii).size();
    if (sum > index) {
      *which_point_set = ii;
      *which_point = index - previous_sum;
      found = true;
      break;
    }
    previous_sum = sum;
  }

  return found;
}

void PointSetList::GetPointRefs(vector<PointRef>* out_refs) const {
  assert(out_refs != NULL);
  out_refs->clear();
  int point_index = 0;
  int which_point_set = 0;
  int which_point = 0;
  for (int ii = 0; ii < point_set_size(); ++ii) {
    which_point_set = ii;
    for (int jj = 0; jj < this->point_set(ii).size(); ++jj) {
      which_point = jj;
      PointRef new_ref(this, point_index, which_point_set, which_point);
      out_refs->push_back(new_ref);
      ++point_index;
    }
  }
}


void PointSetList::GetPointRefs(int index, vector<PointRef>* out_refs) const {
  assert(out_refs != NULL);
  out_refs->clear();
  int point_index = 0;
  int which_point_set = 0;
  int which_point = 0;
  for (int ii = 0; ii < point_set_size(); ++ii) {
    which_point_set = ii;
    int this_size = this->point_set(ii).size();
    for (int jj = 0; jj < this_size; ++jj) {
      which_point = jj;
      PointRef new_ref(this, point_index, which_point_set, which_point);
      if (which_point_set == index) {
        out_refs->push_back(new_ref);
      }
      ++point_index;
    }
  }
}



bool PointSetList::GetSetCardinalities(vector<int>* destination) const {
  assert(NULL != destination);
  destination->clear();
  for (int ii = 0; ii < point_set_size(); ++ii) {
    destination->push_back(point_set(ii).size());
  }
  return true;
}

}  // namespace libpmk
