// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdint.h>
#include <assert.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "clustering/clusterer.h"
#include "point_set/point-set.h"
#include "point_set/point-ref.h"

using namespace std;

namespace libpmk {

Clusterer::Clusterer() : done_(false) { }

const PointSet& Clusterer::centers() const {
  assert(done_);
  return *(cluster_centers_.get());
}

int Clusterer::centers_size() const {
  assert(done_);
  return cluster_centers_->size();
}

int Clusterer::membership(int index) const {
  assert(done_);
  return membership_[index];
}

int Clusterer::membership_size() const {
  assert(done_);
  return (int)membership_.size();
}


void Clusterer::Cluster(const vector<PointRef>& data) {
  assert((int)data.size() > 0);
  cluster_centers_.reset(new PointSet(data[0].data()->point_dim()));
  membership_.resize((int)data.size());
  DoClustering(data);
}

void Clusterer::Cluster(const vector<const Point*>& data) {
  assert((int)data.size() > 0);
  cluster_centers_.reset(new PointSet(data[0]->size()));
  membership_.resize((int)data.size());
  DoClustering(data);
}


// Output format:
// (int32_t) num centers, c
// (int32_t) num points,  p
// (int32_t) feature dim, f
//   (Point) feature_1
//   (Point) feature_2
//   ...
//   (Point) feature_c
//   (int32_t) membership_1
//   (int32_t) membership_2
//   ...
//   (int32_t) membership_p
//
//
void Clusterer::WriteToStream(ostream& output_stream) const {
  assert(done_);
  assert(output_stream.good());
  int32_t num_centers = (int32_t)cluster_centers_->size();
  int32_t num_points = (int32_t)membership_.size();
  int32_t feature_dim = (int32_t)cluster_centers_->point_dim();
  output_stream.write((char*)&num_centers, sizeof(int32_t));
  output_stream.write((char*)&num_points, sizeof(int32_t));
  output_stream.write((char*)&feature_dim, sizeof(int32_t));
  for (int ii = 0; ii < num_centers; ++ii) {
    cluster_centers_->point(ii).WriteToStream(output_stream);
  }

  for (int ii = 0; ii < num_points; ++ii) {
    int32_t member = membership_[ii];
    output_stream.write((char *)&member, sizeof(int32_t));
  }
}

void Clusterer::WriteToFile(const char* filename) const {
  ofstream output_stream(filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}

void Clusterer::ReadFromFile(const char* filename) {
  ifstream input_stream(filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

void Clusterer::ReadFromStream(istream& input_stream) {
  assert(input_stream.good());
  int32_t num_centers, num_points, feature_dim;
  input_stream.read((char*)&num_centers, sizeof(int32_t));
  input_stream.read((char*)&num_points, sizeof(int32_t));
  input_stream.read((char*)&feature_dim, sizeof(int32_t));

  cluster_centers_.reset(new PointSet(feature_dim));
  membership_.resize(num_points);

  for (int ii = 0; ii < num_centers; ++ii) {
    Point* new_point = cluster_centers_->add_point();
    new_point->ReadFromStream(input_stream);
  }

  for (int ii = 0; ii < num_points; ++ii) {
    int32_t member;
    input_stream.read((char*)&member, sizeof(int32_t));
    membership_[ii] = member;
  }
  done_ = true;
}

}  // namespace libpmk
