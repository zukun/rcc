// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of Channelizer, KMeansChannelizer, and
// HierarchicalKMeansChannelizer.
//

#include <assert.h>
#include <cfloat>
#include <fstream>
#include <iostream>
#include <vector>

#include "spatial/channelizer.h"

#include "clustering/hierarchical-clusterer.h"
#include "clustering/k-means-clusterer.h"
#include "point_set/point-set-list.h"
#include "point_set/point-ref.h"
#include "util/distance-computer.h"

using namespace libpmk;
using namespace std;

namespace spatial_pmk {

void Channelizer::WriteToStream(ostream& output_stream) const {
  assert(output_stream.good());
  int32_t num_features = (int32_t)channel_map_.size();
  int32_t num_channels = (int32_t)num_channels_;
  output_stream.write((char*)&num_features, sizeof(int32_t));
  output_stream.write((char*)&num_channels, sizeof(int32_t));
  for (int ii = 0; ii < num_features; ++ii) {
    int32_t member = channel_map_[ii];
    output_stream.write((char *)&member, sizeof(int32_t));
  }
  WriteData(output_stream);
}

void Channelizer::ReadFromStream(istream& input_stream) {
  assert(input_stream.good());
  int32_t num_features, num_channels;
  input_stream.read((char *)&num_features, sizeof(int32_t));
  input_stream.read((char *)&num_channels, sizeof(int32_t));
  num_channels_ = num_channels;

  channel_map_.clear();
  channel_map_.resize(num_features);
  for (int ii = 0; ii < num_features; ++ii) {
    int32_t member;
    input_stream.read((char *)&member, sizeof(int32_t));
    channel_map_[ii] = member;
  }
  ReadData(input_stream);
}


void Channelizer::WriteToFile(const char* output_filename) const {
  ofstream output_stream(output_filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}

void Channelizer::ReadFromFile(const char* input_filename) {
  ifstream input_stream(input_filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

void Channelizer::InitializeChannels(const PointSetList& point_set_list) {
  vector<PointRef> point_refs;
  point_set_list.GetPointRefs(&point_refs);
  InitializeChannels(point_refs);
}


/*********************************************************************
 * KMeansChannelizer                                                 *
 *********************************************************************/

KMeansChannelizer::KMeansChannelizer(int num_clusters,
                                     int max_iterations,
                                     const DistanceComputer& dist_computer) :
  Channelizer(), clusterer_(num_clusters, max_iterations, dist_computer),
  distance_computer_(dist_computer) { }

void KMeansChannelizer::InitializeChannels(
  const vector<PointRef>& point_refs) {
  channel_map_.clear();
  clusterer_.Cluster(point_refs);
  for (int ii = 0; ii < clusterer_.membership_size(); ++ii) {
    channel_map_.push_back(clusterer_.membership(ii));
  }
  num_channels_ = clusterer_.centers_size();
}

int KMeansChannelizer::Channelize(const Point& point) const {
  const PointSet centers = clusterer_.centers();
  double min_distance = DBL_MAX;
  int best_channel = -1;
  for (int ii = 0; ii < clusterer_.centers_size(); ++ii) {
    const Point& center = centers.point(ii);
    double distance = distance_computer_.ComputeDistance(point, center,
                                                         min_distance);
    if (distance < min_distance) {
      min_distance = distance;
      best_channel = ii;
    }
  }
  assert(best_channel != -1);
  return best_channel;
}

void KMeansChannelizer::ReadData(istream& input_stream) {
  clusterer_.ReadFromStream(input_stream);
}

void KMeansChannelizer::WriteData(ostream& output_stream) const {
  clusterer_.WriteToStream(output_stream);
}

void SimplePointChannelizer::InitializeChannels(
  const vector<PointRef>& point_refs) {
  channel_map_.clear();
  for (int ii = 0; ii < (int)point_refs.size(); ++ii) {
    const Point& point = point_refs[ii].point();
    channel_map_.push_back((int)point[0]);
  }
  num_channels_ = 300;
}

int SimplePointChannelizer::Channelize(const Point& point) const {
  return (int)point[0];
}

void SimplePointChannelizer::ReadData(istream& input_stream) { }
void SimplePointChannelizer::WriteData(ostream& output_stream) const { }


/*********************************************************************
 * HierarchicalKMeansChannelizer                                     *
 *********************************************************************/
// HierarchicalKMeansChannelizer::HierarchicalKMeansChannelizer(
//   int num_levels, int branch_factor, const DistanceComputer& dist_computer) 
//    : Channelizer(), num_levels_(num_levels), branch_factor_(branch_factor_),
//   distance_computer_(dist_computer) { }

// void HierarchicalKMeansChannelizer::InitializeChannels(
//   const vector<PointRef>& point_refs) {
//   channel_map_.clear();
//   HierarchicalClusterer clusterer;
//   clusterer.Cluster(num_levels_, branch_factor_, point_refs,
//                     distance_computer_);

//   // This maps the sparse leaf ids into dense [0, n) representation.
//   hash_map<int, int> used_leaf_ids;
//   int last_used_id = -1;

//   for (int ii = 0; ii < clusterer.membership_size(); ++ii) {
//     int leaf_id = clusterer.membership(ii);
//     if (used_leaf_ids.find(leaf_id) == used_leaf_ids.end()) {
//       ++last_used_id;
//       used_leaf_ids[leaf_id] = last_used_id;
//     }

//     channel_map_[ii] = used_leaf_ids[leaf_id];
//   }
//   num_channels_ = last_used_id + 1;
// }

}  // end namespace spatial_pmk
