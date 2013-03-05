// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <memory>
#include <vector>

#include "spatial/normalized-spatial-pyramid-maker.h"

#include "point_set/point-ref.h"
#include "point_set/point-set-list.h"
#include "pyramids/uniform-pyramid-maker.h"
#include "spatial/channelizer.h"

using namespace std;
using namespace libpmk;

namespace spatial_pmk {

NormalizedSpatialPyramidMaker::NormalizedSpatialPyramidMaker(
  const Channelizer& channelizer, int num_levels) :
  channelizer_(channelizer), num_levels_(num_levels) { }

NormalizedSpatialPyramidMaker::~NormalizedSpatialPyramidMaker() {
  for (int ii = 0; ii < (int)pyramid_makers_.size(); ++ii) {
    assert(pyramid_makers_[ii] != NULL);
    delete pyramid_makers_[ii];
  }
}
  
void NormalizedSpatialPyramidMaker::WriteToStream(
  ostream& output_stream) const {
  assert(output_stream.good());
  int32_t num_channels = channelizer_.channel_size();
  output_stream.write((char*)&num_channels, sizeof(int32_t));

  for (int ii = 0; ii < num_channels; ++ii) {
    pyramid_makers_[ii]->WriteToStream(output_stream);
  }
}

void NormalizedSpatialPyramidMaker::WriteToFile(
  const char* output_filename) const {
  ofstream output_stream(output_filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}

void NormalizedSpatialPyramidMaker::ReadFromStream(istream& input_stream) {
  assert(input_stream.good());
  for (int ii = 0; ii < (int)pyramid_makers_.size(); ++ii) {
    assert(pyramid_makers_[ii] != NULL);
    delete pyramid_makers_[ii];
  }
  pyramid_makers_.clear();

  int32_t num_channels;
  input_stream.read((char *)&num_channels, sizeof(int32_t));
  assert(num_channels == channelizer_.channel_size());

  for (int ii = 0; ii < num_channels; ++ii) {
    NormalizedUniformPyramidMaker* pyramid_maker =
      new NormalizedUniformPyramidMaker(num_levels_);
    pyramid_maker->ReadFromStream(input_stream);
    pyramid_makers_.push_back(pyramid_maker);
  }
}

void NormalizedSpatialPyramidMaker::ReadFromFile(const char* input_filename) {
  ifstream input_stream(input_filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

void NormalizedSpatialPyramidMaker::Preprocess(int num_channels) {
  for (int ii = 0; ii < num_channels; ++ii) {
    NormalizedUniformPyramidMaker* maker =
      new NormalizedUniformPyramidMaker(num_levels_);
    pyramid_makers_.push_back(maker);
  }
}
vector<MultiResolutionHistogram*> NormalizedSpatialPyramidMaker::MakePyramid(
  const vector<PointRef>& image_features,
  const vector<PointRef>& image_locs) const {
  assert(image_features.size() == image_locs.size());

  vector<MultiResolutionHistogram*> output;

  // Channelize each point in this image. This vector contains, for each
  // point in the image, which channel it will fall into.
  vector<int> channels;
  for (int ii = 0; ii < (int)image_features.size(); ++ii) {
    channels.push_back(channelizer_.Channelize(image_features[ii].point()));
  }

  for (int ii = 0; ii < channelizer_.channel_size(); ++ii) {
    vector<PointRef> this_channel;
    for (int jj = 0; jj < (int)image_locs.size(); ++jj) {
      if (channels[jj] == ii) {
        this_channel.push_back(image_locs[jj]);
      }
    }

    output.push_back(pyramid_makers_[ii]->MakePyramid(this_channel));
  }
  return output;
}
}  // end namespace spatial_pmk
