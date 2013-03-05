// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
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
       << " input.psl output.chan num_clusters\n\n";
  cerr << "<input.psl>: The PointSetList to cluster\n";
  cerr << "<output.chan>: A KMeansChannelizer preprocessed with input.psl\n";
  cerr << "<num_clusters>: Number of K-means clusters to use\n";
}

int main(int argc, char** argv) {
  if (argc != 4) {
    usage(argv[0]);
    exit(0);
  }

  string point_set_file(argv[1]);
  string output_file(argv[2]);
  int num_clusters(atoi(argv[3]));


  cout << "Reading " << point_set_file << "... " << flush;
  MutablePointSetList psl;
  psl.ReadFromFile(point_set_file.c_str());
  cout << "done. Read " << psl.point_set_size() << " point sets." << endl;

  L2DistanceComputer distance_computer;
  KMeansChannelizer channelizer(num_clusters, 100, distance_computer);
  //SimplePointChannelizer channelizer;

  vector<PointRef> point_refs;
  psl.GetPointRefs(&point_refs);

  cout << "Setting up channels... " << flush;
  channelizer.InitializeChannels(point_refs);
  cout << "done." << endl;

  cout << "Writing channels to " << output_file << "... " << flush;
  channelizer.WriteToFile(output_file.c_str());
  cout << "done." << endl;

  return 0;
}
