// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a channelizer and a list of feature locations, computes
// spatial pyramids. Given a new PointSet, this object will return a
// set of MultiResolutionHistograms, where each one corresponds to one
// channel.
//

#ifndef SPATIAL_SPATIAL_PYRAMID_MAKER_H
#define SPATIAL_SPATIAL_PYRAMID_MAKER_H

#include <memory>
#include <vector>
#include "point_set/point-set-list.h"
#include "pyramids/uniform-pyramid-maker.h"
#include "spatial/channelizer.h"

using namespace std;
using namespace libpmk;

namespace spatial_pmk {

class SpatialPyramidMaker {
public:
  SpatialPyramidMaker(const Channelizer& channelizer);
  virtual ~SpatialPyramidMaker();

  /// Outputs a vector of size N, where N is the number of channels.
  /// out_levels must not be NULL.
  void GetNumLevels(vector<int>* out_levels) const;

  void Preprocess(const vector<PointRef>& locations,
                  double finest_side_length, double side_factor,
                  int discretize_factor, bool do_translations,
                  bool global_translations);

  void Preprocess(const PointSetList& locations,
                  double finest_side_length, double side_factor,
                  int discretize_factor, bool do_translations,
                  bool global_translations);

  /// Creates a pyramid for each channel. The caller is responsible for
  /// freeing this memory.
  vector<MultiResolutionHistogram*> MakePyramid(
    const vector<PointRef>& image_features,
    const vector<PointRef>& image_locs) const;
  
  /**
   * Format:
   * (int32_t) N, the number of channels
   * (N * UniformPyramidMaker) the pyramid makers for each channel.
   *
   * Requires Preprocess() or ReadFromStream() to have been called first.
   */
  void WriteToStream(ostream& output_stream) const;
  void WriteToFile(const char* output_filename) const;
  void ReadFromStream(istream& input_stream);
  void ReadFromFile(const char* input_filename);

  
private:
  vector<UniformPyramidMaker*> pyramid_makers_;
  const Channelizer& channelizer_;
  bool preprocessed_;
};
}  // end namespace spatial_pmk

#endif  // SPATIAL_SPATIAL_PYRAMID_MAKER_H
