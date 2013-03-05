// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// The input to this program is 2N psl files.
// The first N psl files are the features; the next N psl files are the
// corresponding locs in the same order.
//
// For each feature psl (call it "name.psl"), it will use the given
// channelizer to split the feature points into that many channels,
// placing them in OUTPUT_LOCS_BASE/[CHANNEL_NUM]/[name].psl.
//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "util/distance-computer.h"
#include "point_set/mutable-point-set-list.h"
#include "point_set/point-ref.h"
#include "spatial/channelizer.h"

using namespace libpmk;
using namespace spatial_pmk;
using namespace std;

void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name
       << " <input_channelizer> <loc_base> <FEAT>*N <LOC>*n\n";
}

// Given a path like "/csail/blah/blah/stuff.psl", return "stuff.psl".
// This assumes the input contains "/stuff.psl" at the end of it.
string GetPSLBase(const char* psl) {
  string str(psl);
  int slash_index = str.rfind('/', str.size() - 1);
  return str.substr(slash_index + 1, string::npos);
}

int main(int argc, char** argv) {
  if (argc < 5) {
    usage(argv[0]);
    exit(0);
  }

  string channelizer_file(argv[1]);
  string output_loc_base(argv[2]);

  assert((argc - 3) % 2 == 0);
  int num_point_sets = (argc - 3) / 2;

  L2DistanceComputer distance_computer;
  KMeansChannelizer channelizer(200, 100, distance_computer);
  cout << "Reading " << channelizer_file << "... " << flush;
  channelizer.ReadFromFile(channelizer_file.c_str());
  cout << "done. Read " << channelizer.channel_size() << "channels." << endl;

  for (int ii = 0; ii < num_point_sets; ++ii) {
    const char* feature_file = argv[ii + 3];
    const char* loc_file = argv[ii + 3 + num_point_sets];
    MutablePointSetList features;
    MutablePointSetList locs;
    
    cout << "Reading " << feature_file << "... " << flush;
    features.ReadFromFile(feature_file);
    cout << "done." << endl;

    cout << "Reading " << loc_file << "... " << flush;
    locs.ReadFromFile(loc_file);
    cout << "done." << endl;

    int loc_dim = locs.point_dim();
    assert (loc_dim == 2);

    // For each point in features, this stores the channel ID of that
    // point.
    vector<int> channels;

    int points_written = 0;
    for (int jj = 0; jj < features.point_size(); ++jj) {
      const Point& point = features.point(jj);
      channels.push_back(channelizer.Channelize(point));
    }

    // For each channel, write a features and locs file.
    for (int jj = 0; jj < channelizer.channel_size(); ++jj) {
      MutablePointSetList out_locs;

      int point_index = 0;
      for (int kk = 0; kk < features.point_set_size(); ++kk) {
        const PointSet& feature_point_set = features.point_set(kk);
        const PointSet& loc_point_set = locs.point_set(kk);
        assert(feature_point_set.size() == loc_point_set.size());
        PointSet* out_loc_point_set = out_locs.add_point_set(loc_dim);
        
        for (int pp = 0; pp < feature_point_set.size(); ++pp) {
          // If this point is in the given channel, add it to the point set.
          if (channels[point_index] == jj) {
            out_loc_point_set->add_point(loc_point_set.point(pp));
            ++points_written;
          }
          ++point_index;
        }
      }

      char loc_path[2056];
      string base = GetPSLBase(feature_file);
      sprintf(loc_path, "%s/%03d/%s",
              output_loc_base.c_str(), jj, base.c_str());
      cout << "Writing channel " << jj << "... " << flush;
      out_locs.WriteToFile(loc_path);
      cout << "done." << endl;
    }
    assert(points_written == features.point_size());
  }

  return 0;
}
