// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <vector>
#include "spatial/spatial-pyramid-reader.h"
#include "histograms/multi-resolution-histogram.h"

using namespace libpmk;
using namespace std;

namespace spatial_pmk {

SpatialPyramidReader::SpatialPyramidReader(istream& input_stream) :
  num_channels_(0), num_pyramids_(0), num_pyramids_read_(0),
  input_stream_(input_stream) { }

void SpatialPyramidReader::ReadHeader() {
  assert(input_stream_.good());

  int32_t num_chan32, num_pyr32;
  input_stream_.read((char *)(&num_chan32), sizeof(int32_t));
  input_stream_.read((char *)(&num_pyr32), sizeof(int32_t));
  num_channels_ = num_chan32;
  num_pyramids_ = num_pyr32;
}

vector<MultiResolutionHistogram*> SpatialPyramidReader::ReadSpatialPyramid() {
  assert(input_stream_.good());
  assert(num_pyramids_read_ < num_pyramids_);
  vector<MultiResolutionHistogram*> pyramid_to_return;
  for (int ii = 0; ii < num_channels_; ++ii) {
    MultiResolutionHistogram* new_piece =
      MultiResolutionHistogram::ReadSingleHistogramFromStream(input_stream_);
    pyramid_to_return.push_back(new_piece);
  }

  ++num_pyramids_read_;
  return pyramid_to_return; 
}
}  // end namespace spatial_pmk
