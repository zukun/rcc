// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Takes a point set list and the output of HierarchicalClusterer, to create
// a vocab-guided pyramid.

#include <iostream>
#include <fstream>
#include "pyramids/input-specific-vg-pyramid-maker.h"
#include "pyramids/global-vg-pyramid-maker.h"
#include "clustering/hierarchical-clusterer.h"
#include "histograms/multi-resolution-histogram.h"
#include "util/distance-computer.h"
#include "util/bin-weight-scheme.h"
#include "point_set/mutable-point-set-list.h"
#include "point_set/on-disk-point-set-list.h"

using namespace std;
using namespace libpmk;

void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name << " data.psl clusters.hc "
       << "output.mrh\n\n";
  cerr << "<data.psl>:      The PointSetList we want to make pyramids for.\n";
  cerr << "<clusters.hc>:   The result of running HierarchicalClusterer\n";
  cerr << "<output.mrh>:    Where to write the pyramids to\n";
}

int main(int argc, char** argv) {
  if (argc != 4) {
    usage(argv[0]);
    exit(0);
  }

  string point_set_file(argv[1]);
  string clusters_file(argv[2]);
  string output_file(argv[3]);

  cout << "Reading " << point_set_file << "... " << flush;
  OnDiskPointSetList psl(point_set_file, 1, 200);
  cout << "done. Read " << psl.point_set_size() << " point sets." << endl;

  cout << "Reading " << clusters_file << "... " << flush;
  HierarchicalClusterer clusterer;
  clusterer.ReadFromFile(clusters_file.c_str());
  cout << "done." << endl;

  cout << "Initializing pyramid maker... " << flush;
  L2DistanceComputer dist;
  InputSpecificVGPyramidMaker vgpm(clusterer, dist);
  cout << "done." << endl;

  cout << "Making pyramids... " << flush;
  vector<MultiResolutionHistogram*> vec = vgpm.MakePyramids(psl);
  cout << "done." << endl;

  cout << "Writing pyramids to " << output_file << "... " << flush;
  MultiResolutionHistogram::WriteToFile(output_file.c_str(), vec);
  cout << "done." << endl;

  return 0;
}
