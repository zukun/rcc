// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This program takes point sets and runs hierarchical k-means on them,
// outputting the data to a specified output file. It uses L2 distance
// to do the clustering.

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "point_set/mutable-point-set-list.h"
#include "point_set/on-disk-point-set-list.h"
#include "point_set/point-ref.h"
#include "clustering/hierarchical-clusterer.h"
#include "util/distance-computer.h"

using namespace std;
using namespace libpmk;

void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name << " input.psl output.hc levels branch\n\n";
  cerr << "<input.psl>: A PointSetList file, where each PointSet contains\n"
       << "             features for one image\n";
  cerr << "<output.hc>: The result of running HierarchicalClusterer on the "
       << "input\n";
  cerr << "<levels>:    (int) Number of levels in the cluster tree\n";
  cerr << "<branch>:    (int) The branch factor of the cluster tree\n";
}


int main(int argc, char** argv) {
  if (argc != 5) {
    usage(argv[0]);
    exit(0);
  }

  // Set the random seed for hierarchical clustering
  srand(time(NULL));

  string input_file(argv[1]);
  string output_file(argv[2]);
  int num_levels = atoi(argv[3]);
  int branch_factor = atoi(argv[4]);

  cout << "Reading " << input_file << "... " << flush;
  MutablePointSetList psl;
  psl.ReadFromFile(input_file.c_str());
  cout << "done. Read " << psl.point_set_size() << " point sets." << endl;

  vector<PointRef> point_refs;
  psl.GetPointRefs(&point_refs);

  HierarchicalClusterer clusterer;
  L2DistanceComputer dist_computer;

  cout << "Clustering... " << flush;
  clusterer.Cluster(num_levels, branch_factor, point_refs, dist_computer);
  cout << "done." << endl;

  cout << "Writing clusters to " << output_file << "... " << flush;
  clusterer.WriteToFile(output_file.c_str());
  cout << "done." << endl;

  return 0;
}
