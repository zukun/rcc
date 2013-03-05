// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given the original features, and their spatial information, and the
// output of a channelizer, output spatial pyramids. If there are n
// channels, n files will be created, one for each channel. Each file
// is a .mrh file, containing the pyramids for all images in the point
// sets, for a given channel.

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "histograms/multi-resolution-histogram.h"
#include "util/distance-computer.h"
#include "point_set/mutable-point-set-list.h"
#include "point_set/on-disk-point-set-list.h"
#include "point_set/point-ref.h"
#include "spatial/channelizer.h"
#include "spatial/normalized-spatial-pyramid-maker.h"

using namespace std;
using namespace spatial_pmk;
using namespace libpmk;

void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name << " features.psl locs.psl channels.chan output_base levels\n\n";
  cerr << "<features.psl>: The PointSetList of features\n";
  cerr << "<locs.psl>: The PointSetList of 2D points\n";
  cerr << "<channels.chan>: Channels file\n";
  cerr << "<output_base>: Spatial pyramids written to <base>n.mrh\n";
  cerr << "<n>:        (int) the number of pyramid levels\n";
}

int main(int argc, char** argv) {
  if (argc != 6) {
    usage(argv[0]);
    exit(0);
  }

  string features_file(argv[1]);
  string point_set_file(argv[2]);
  string channel_file(argv[3]);
  string output_base(argv[4]);
  int num_levels = atoi(argv[5]);

  cout << "Reading " << features_file << "... " << flush;
  MutablePointSetList features; //(features_file, 1, 5);
  features.ReadFromFile(features_file.c_str());
  cout << "done. Read " << features.point_set_size() << " point sets." << endl;

  cout << "Reading " << point_set_file << "... " << flush;
  MutablePointSetList psl;
  psl.ReadFromFile(point_set_file.c_str());
  cout << "done. Read " << psl.point_set_size() << " point sets." << endl;

  vector<PointRef> point_refs;
  psl.GetPointRefs(&point_refs);

  cout << "Reading " << channel_file << "... " << flush;
  L2DistanceComputer distance_computer;
  //SimplePointChannelizer channelizer;
  KMeansChannelizer channelizer(1, 100, distance_computer);
  channelizer.ReadFromFile(channel_file.c_str());
  cout << "done. Read " << channelizer.channel_size()
       << " channels." << endl;


  cout << "Initializing pyramid maker... " << flush;
  NormalizedSpatialPyramidMaker spm(channelizer, num_levels);
  spm.Preprocess(channelizer.channel_size());
  cout << "done. Levels per channel: " << num_levels << endl;
  //vector<int> num_levels;
  //spm.GetNumLevels(&num_levels);
  //for (int ii = 0; ii < (int)num_levels.size(); ++ii) {
    //cout << "Channel " << ii << ": " << num_levels[ii] << endl;
//}

  // For each channel, write a .mrh file.
  vector<ofstream*> channel_streams;

  for (int ii = 0; ii < channelizer.channel_size(); ++ii) {
    char output_file[255];
    sprintf(output_file, "%s%.4d.pyr", output_base.c_str(), ii);
    ofstream* output_stream = new ofstream(output_file,
                                           ios::binary | ios::trunc);
    MultiResolutionHistogram::WriteHeaderToStream(*output_stream,
                                                  psl.point_set_size());
    channel_streams.push_back(output_stream);
  }

  cout << "Making pyramids... " << flush;
  for (int ii = 0; ii < psl.point_set_size(); ++ii) {
    cout << ii << " of " << psl.point_set_size() << endl;
    cout << psl.point_size() << " features total" << endl;
    vector<PointRef> image_locs;
    vector<PointRef> image_features;
    psl.GetPointRefs(ii, &image_locs);
    features.GetPointRefs(ii, &image_features);
    vector<MultiResolutionHistogram*> spatial_pyramid =
      spm.MakePyramid(image_features, image_locs);

    assert(channelizer.channel_size() == (int)spatial_pyramid.size());
    for (int jj = 0; jj < channelizer.channel_size(); ++jj) {
      MultiResolutionHistogram::WriteSingleHistogramToStream(
        *(channel_streams[jj]), spatial_pyramid[jj]);
    }

    for (int jj = 0; jj < (int)spatial_pyramid.size(); ++jj) {
      delete spatial_pyramid[jj];
    }
    spatial_pyramid.clear();
  }
  cout << "done." << endl;

  for (int ii = 0; ii < (int)channel_streams.size(); ++ii) {
    channel_streams[ii]->close();
    delete channel_streams[ii];
  }

  return 0;
}
