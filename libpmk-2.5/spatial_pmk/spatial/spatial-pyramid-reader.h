// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Utility class for reading spatial pyramids from streams. They are in a
// simple serialized format:
// (int32) Number of channels, C
// (int32) Number of spatial pyramids, S
// For each spatial pyramid:
//    (C * MultiResolutionHistogram) A MRH for each channel.
//

#ifndef SPATIAL_SPATIAL_PYRAMID_READER_H
#define SPATIAL_SPATIAL_PYRAMID_READER_H

#include <iostream>
#include <vector>
#include "histograms/multi-resolution-histogram.h"

using namespace libpmk;
using namespace std;

namespace spatial_pmk {

class SpatialPyramidReader {
public:
  SpatialPyramidReader(istream& input_stream);

  void ReadHeader();
  vector<MultiResolutionHistogram*> ReadSpatialPyramid();
  
  int num_channels() const { return num_channels_; }
  int num_pyramids() const { return num_pyramids_; }

private:
  int num_channels_;

  // This is the total number of pyramids in the stream.
  int num_pyramids_;

  // This is the number of pyramids pulled from the stream so far.
  int num_pyramids_read_;
  istream& input_stream_;
};
}  // end namespace spatial_pmk

#endif  // SPATIAL_SPATIAL_PYRAMID_READER_H
