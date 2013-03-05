// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Utility class for writing spatial pyramids to streams. They are in a
// simple serialized format:
// (int32) Number of channels, C
// (int32) Number of spatial pyramids, S
// For each spatial pyramid:
//    (C * MultiResolutionHistogram) A MRH for each channel.
//

#ifndef SPATIAL_SPATIAL_PYRAMID_WRITER_H
#define SPATIAL_SPATIAL_PYRAMID_WRITER_H

#include <iostream>
#include <vector>
#include "histograms/multi-resolution-histogram.h"

using namespace libpmk;
using namespace std;

namespace spatial_pmk {

class SpatialPyramidWriter {
public:
  SpatialPyramidWriter(ostream& output_stream);
  void WriteHeader(int num_channels, int num_pyramids);
  void WriteSpatialPyramid(const vector<MultiResolutionHistogram*>& pyramid);

private:
  ostream& output_stream_;
};

}  // end namespace spatial_pmk


#endif  // SPATIAL_SPATIAL_PYRAMID_WRITER_H
