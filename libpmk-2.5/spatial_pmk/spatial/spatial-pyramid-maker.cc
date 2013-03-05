// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <memory>
#include <vector>

#include "spatial/spatial-pyramid-maker.h"

#include "point_set/point-ref.h"
#include "point_set/point-set-list.h"
#include "pyramids/uniform-pyramid-maker.h"
#include "spatial/channelizer.h"

using namespace std;
using namespace libpmk;

namespace spatial_pmk {

SpatialPyramidMaker::SpatialPyramidMaker(const Channelizer& channelizer) :
  channelizer_(channelizer), preprocessed_(false) { }

SpatialPyramidMaker::~SpatialPyramidMaker() {
  for (int ii = 0; ii < (int)pyramid_makers_.size(); ++ii) {
    assert(pyramid_makers_[ii] != NULL);
    delete pyramid_makers_[ii];
  }
}

void SpatialPyramidMaker::Preprocess(const vector<PointRef>& locations,
                                     double finest_side_length,
                                     double side_factor,
                                     int discretize_factor,
                                     bool do_translations,
                                     bool global_translations) {
  pyramid_makers_.clear();

  assert(channelizer_.channel_size() > 0);
  for (int ii = 0; ii < channelizer_.channel_size(); ++ii) {
    UniformPyramidMaker* pyramid_maker = new UniformPyramidMaker();

    // This is a list of point refs that are in this channel:
    vector<PointRef> local_point_refs;
    for (int jj = 0; jj < (int)locations.size(); ++jj) {
      if (channelizer_.channel(jj) == ii) {
        local_point_refs.push_back(locations[jj]);
      }
    }

    assert((int)local_point_refs.size() > 0);
    pyramid_maker->Preprocess(local_point_refs, finest_side_length,
                              side_factor, discretize_factor, do_translations,
                              global_translations);
    pyramid_makers_.push_back(pyramid_maker);
  }

  preprocessed_ = true;
}

void SpatialPyramidMaker::Preprocess(const PointSetList& locations,
                                     double finest_side_length,
                                     double side_factor,
                                     int discretize_factor,
                                     bool do_translations,
                                     bool global_translations) {
  vector<PointRef> point_refs;
  locations.GetPointRefs(&point_refs);
  Preprocess(point_refs, finest_side_length, side_factor,
             discretize_factor, do_translations, global_translations);
}
  
void SpatialPyramidMaker::WriteToStream(ostream& output_stream) const {
  assert(preprocessed_);
  assert(output_stream.good());
  int32_t num_channels = channelizer_.channel_size();
  output_stream.write((char*)&num_channels, sizeof(int32_t));

  for (int ii = 0; ii < num_channels; ++ii) {
    pyramid_makers_[ii]->WriteToStream(output_stream);
  }
}

void SpatialPyramidMaker::WriteToFile(const char* output_filename) const {
  ofstream output_stream(output_filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}

void SpatialPyramidMaker::ReadFromStream(istream& input_stream) {
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
    UniformPyramidMaker* pyramid_maker = new UniformPyramidMaker();
    pyramid_maker->ReadFromStream(input_stream);
    pyramid_makers_.push_back(pyramid_maker);
  }

  preprocessed_ = true;
}

void SpatialPyramidMaker::ReadFromFile(const char* input_filename) {
  ifstream input_stream(input_filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

vector<MultiResolutionHistogram*> SpatialPyramidMaker::MakePyramid(
  const vector<PointRef>& image_features,
  const vector<PointRef>& image_locs) const {
  assert(preprocessed_);
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

void SpatialPyramidMaker::GetNumLevels(vector<int>* out_levels) const {
  assert(NULL != out_levels);
  out_levels->clear();
  for (int ii = 0; ii < (int)pyramid_makers_.size(); ++ii) {
    assert(pyramid_makers_[ii] != NULL);
    out_levels->push_back(pyramid_makers_[ii]->num_levels());
  }
}
}  // end namespace spatial_pmk
