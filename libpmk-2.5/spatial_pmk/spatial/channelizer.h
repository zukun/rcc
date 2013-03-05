// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Channelizers take PointSetLists and provide a map of point index
// (within the PSL) -> a channel ID (small integers). The channel IDs
// begin at 0 and increase, without skipping any numbers. Thus, the
// number of channels is equal to the last channel ID (plus 1).
//
// This file defines a Channelizer superclass as well as two
// channelizers.  The first one runs K-means with a fixed K and
// returns K channels. The second one will run hierarchical K-means
// (requires user to provide branch factor and # levels) and will use
// the leaf nodes as channels.
//

#ifndef SPATIAL_CHANNELIZER_H
#define SPATIAL_CHANNELIZER_H

#include <fstream>
#include <iostream>
#include <vector>

#include "clustering/k-means-clusterer.h"
#include "point_set/point-set-list.h"
#include "point_set/point-ref.h"
#include "util/distance-computer.h"

using namespace libpmk;
using namespace std;

namespace spatial_pmk {

class Channelizer {
public:
  Channelizer() { }
  virtual ~Channelizer() { }

  /**
   * Output format:
   * (int32_t) number of features in the whole PSL, n
   * (int32_t) number of channels, k
   * (n * int32_t) for each feature, the channel ID of the feature.
   */
  void WriteToStream(ostream& output_stream) const;
  void WriteToFile(const char* output_filename) const;
  void ReadFromStream(istream& input_stream);
  void ReadFromFile(const char* input_filename);

  // Read/write auxiliary data.
  virtual void WriteData(ostream& output_stream) const = 0;
  virtual void ReadData(istream& input_stream) = 0;

  /// Populate the feature->channel map with the given point refs.
  /**
   * This method does not do anything and will always fail. It does
   * not specify a channelizing method. This is simply here so that
   * generic Channelizers can be constructed in order to
   * ReadFromStream the output of an actual channelization.
   * To actually channelize something, use KMeansChannelizer or
   * HierarchicalKMeansChannelizer.
   *
   * Anyone who overrides this method (as you should) must make
   * sure that channel_map_'s resulting size is equal to the
   * number of features in the PSL. The Channelizer object will
   * only store the results of the last channelization. Previous
   * results will be cleared on every call to Channelize().
   * You must also properly fill in num_channels_.
   */
  virtual void InitializeChannels(const vector<PointRef>& point_refs) = 0;

  /// For convenience. Will transform the PSL into a point ref vector.
  void InitializeChannels(const PointSetList& point_set_list);

  /// Given a new point, return the channel it belongs to.
  virtual int Channelize(const Point& point) const = 0;

  /// Returns the channel that the indexth point belongs to.
  int channel(int index) const { return channel_map_.at(index); }

  /// Returns the number of channels.
  int channel_size() const { return num_channels_; }

protected:
  vector<int> channel_map_;
  int num_channels_;
};

class KMeansChannelizer : public Channelizer {
public:
  KMeansChannelizer(int num_clusters, int max_iterations,
                    const DistanceComputer& dist_computer);
  virtual ~KMeansChannelizer() { }
  virtual void InitializeChannels(const vector<PointRef>& point_refs);
  virtual int Channelize(const Point& point) const;

  virtual void ReadData(istream& input_stream);
  virtual void WriteData(ostream& output_stream) const;

private:
  KMeansClusterer clusterer_;
  const DistanceComputer& distance_computer_;
};


// This is a simple channelizer that, for each point, assumes the channel
// ID is inside point[0] (as an int).
class SimplePointChannelizer : public Channelizer {
public:
  SimplePointChannelizer() { }
  virtual ~SimplePointChannelizer() { }
  virtual void InitializeChannels(const vector<PointRef>& point_refs);
  virtual int Channelize(const Point& point) const;

  virtual void ReadData(istream& input_stream);
  virtual void WriteData(ostream& output_stream) const;
};


// The HierarchicalKMeansChannelizer has not yet been implemented.
// It needs to implement Channelize() and Read/WriteData().
//
// class HierarchicalKMeansChannelizer : public Channelizer {
//   HierarchicalKMeansChannelizer(int num_levels, int branch_factor,
//                                 const DistanceComputer& distance_computer);
//   virtual ~HierarchicalKMeansChannelizer() { }
//   virtual void InitializeChannels(const vector<PointRef>& point_refs);
//   virtual int Channelize(const Point& point) const;

// private:
//   int num_levels_;
//   int branch_factor_;
//   const DistanceComputer& distance_computer_;
// };



}  // end namespace spatial_pmk

#endif  // SPATIAL_CHANNELIZER_H
