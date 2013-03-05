// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: Tom Yeh  (tomyeh@mit.edu)
//         John Lee (jjl@mit.edu)
//
// Implementation of InvertedFile.
//

#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <list>
#include <map>
#include <vector>

#include "inverted-file.h"

#include "point_set/point.h"
#include "point_set/point-ref.h"

namespace incremental_vtree {

InvertedFile::InvertedFile(const InvertedFile& other) : num_records_(0) {
  for (map<int, list<PointRecord*> >::const_iterator iter =
         other.members_.begin(); iter != other.members_.end(); ++iter) {

    const list<PointRecord*>& records = iter->second;
    for (list<PointRecord*>::const_iterator iter2 = records.begin();
           iter2 != records.end(); ++iter2) {
      add_point(iter->first, (*iter2)->point_ref());
    }
  }
}

InvertedFile::~InvertedFile() {
  for (map<int, list<PointRecord*> >::iterator iter = members_.begin();
       iter != members_.end(); ++iter) {

    list<PointRecord*>& records = iter->second;
    for (list<PointRecord*>::iterator iter2 = records.begin();
         iter2 != records.end(); ++iter2) {
      delete (*iter2);
    }

  }
}

void InvertedFile::add_point(int image_id, const PointRef& point) {
  PointRecord* record = new PointRecord(image_id, point);
  members_[image_id].push_back(record);
  ++num_records_;
}

int InvertedFile::image_size(int index) const {
  map<int, list<PointRecord*> >::const_iterator iter = members_.find(index);
  if (iter != members_.end()) {
    return iter->second.size();
  }
  return 0;
}

int InvertedFile::size() const {
  return num_records_;
}

int InvertedFile::members_size() const {
  return (int)members_.size();
}

void InvertedFile::GetPointRecords(vector<PointRecord>* output) const {
  assert(NULL != output);
  for (map<int, list<PointRecord*> >::const_iterator iter = members_.begin();
       iter != members_.end(); ++iter) {
    const list<PointRecord*>& records = iter->second;
    for (list<PointRecord*>::const_iterator iter2 = records.begin();
         iter2 != records.end(); ++iter2) {
      output->push_back(PointRecord(**iter2));
    }
  }
}

void InvertedFile::GetPointRefs(vector<PointRef>* output) const {
  assert(NULL != output);
  for (map<int, list<PointRecord*> >::const_iterator iter = members_.begin();
       iter != members_.end(); ++iter) {
    const list<PointRecord*>& records = iter->second;
    for (list<PointRecord*>::const_iterator iter2 = records.begin();
         iter2 != records.end(); ++iter2) {
      output->push_back((*iter2)->point_ref());
    }
  }
}

void InvertedFile::ReadFromStream(istream& input_stream,
                                  const PointSetList* data) {
  assert(input_stream.good());

  // Clear all of the data stored in here:
  for (map<int, list<PointRecord*> >::iterator iter = members_.begin();
       iter != members_.end(); ++iter) {

    list<PointRecord*>& records = iter->second;
    for (list<PointRecord*>::iterator iter2 = records.begin();
         iter2 != records.end(); ++iter2) {
      delete (*iter2);
    }
    records.clear();
  }
  members_.clear();
  num_records_ = 0;

  int32_t num_images;
  input_stream.read((char *)&num_images, sizeof(int32_t));

  for (int ii = 0; ii < num_images; ++ii) {
    int32_t image_id;
    int32_t num_points;
    input_stream.read((char *)&image_id, sizeof(int32_t));
    input_stream.read((char *)&num_points, sizeof(int32_t));

    for (int jj = 0; jj < num_points; ++jj) {
      int32_t index;
      int32_t which_point_set;
      int32_t which_point;
      input_stream.read((char *)&index, sizeof(int32_t));
      input_stream.read((char *)&which_point_set, sizeof(int32_t));
      input_stream.read((char *)&which_point, sizeof(int32_t));
      PointRef added_point(data, index, which_point_set, which_point);
      add_point(image_id, added_point);
    }
  } 
}

void InvertedFile::WriteToStream(ostream& output_stream) const {
  assert(output_stream.good());
  int32_t num_images = members_size();
  output_stream.write((char *)&num_images, sizeof(int32_t));

  // Iterate over the image_id -> [point1, point2, point3, ...] mappings:
  for (map<int, list<PointRecord*> >::const_iterator iter = members_.begin();
       iter != members_.end(); ++iter) {
    int32_t image_id = iter->first;
    output_stream.write((char *)&image_id, sizeof(int32_t));

    const list<PointRecord*>& records = iter->second;
    int32_t image_size = (int32_t)records.size();
    output_stream.write((char *)&image_size, sizeof(int32_t));

    // For this image ID, iterate over the points contained by it:
    for (list<PointRecord*>::const_iterator iter2 = records.begin();
         iter2 != records.end(); ++iter2) {
      PointRecord* point_record = *iter2;
      int32_t index = point_record->point_ref().point_index();
      int32_t which_point_set = point_record->point_ref().which_point_set();
      int32_t which_point = point_record->point_ref().which_point();
      output_stream.write((char *)&index, sizeof(int32_t));
      output_stream.write((char *)&which_point_set, sizeof(int32_t));
      output_stream.write((char *)&which_point, sizeof(int32_t));
    }
  }
}


}  // end namespace incremental_vtree
