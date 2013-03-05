// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <vector>
#include "spatial/spatial-pyramid-writer.h"
#include "histograms/multi-resolution-histogram.h"

using namespace libpmk;
using namespace std;

namespace spatial_pmk {

SpatialPyramidWriter::SpatialPyramidWriter(ostream& output_stream) :
  output_stream_(output_stream) { }

void SpatialPyramidWriter::WriteHeader(int num_channels, int num_pyramids) {
  assert(output_stream_.good());
  int32_t num_chan32 = num_channels;
  int32_t num_pyr32 = num_pyramids;
  output_stream_.write((char *)(&num_chan32), sizeof(int32_t));
  output_stream_.write((char *)(&num_pyr32), sizeof(int32_t));
}

void SpatialPyramidWriter::WriteSpatialPyramid(
  const vector<MultiResolutionHistogram*>& pyramid) {
  assert(output_stream_.good());
  for (int ii = 0; ii < (int)pyramid.size(); ++ii) {
    MultiResolutionHistogram::WriteSingleHistogramToStream(output_stream_,
                                                           pyramid[ii]);
  }
}
}  // end namespace spatial_pmk
