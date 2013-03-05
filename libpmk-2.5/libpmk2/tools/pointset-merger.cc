// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Takes N input point-set-list files, where each of the files represents
// one class, and merges them into one large PointSetList. This will
// also generate a text file in the format:
//
// (int) P, the number of point sets
// (int) class of point set 1
// (int) class of point set 2
// ...
// (int) class of point set P

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "point_set/mutable-point-set-list.h"

using namespace std;
using namespace libpmk;

void usage(const char* argv) {
  cerr << "Usage: " << argv
       << "<output_labels> <output_ptsets> <N> <psl1> ... <pslN>" << endl;
}


int main(int argc, char** argv) {
  if (argc < 4) {
    usage(argv[0]);
    exit(0);
  }

  string label_file(argv[1]);
  string output_file(argv[2]);
  int num_inputs = atoi(argv[3]);

  if (argc != 4 + num_inputs) {
    usage(argv[0]);
    exit(0);
  }

  MutablePointSetList all_point_sets;
  vector<int> labels;

  for (int ii = 4; ii < argc; ++ii) {
    int current_label = ii - 3;
    cout << "Reading set " << current_label
         << " (" << argv[ii] << ")... " << flush;
    MutablePointSetList point_sets;
    point_sets.ReadFromFile(argv[ii]);
    cout << "done." << endl;

    for (int jj = 0; jj < point_sets.point_set_size(); ++jj) {
      all_point_sets.add_point_set(point_sets.point_set(jj));
      labels.push_back(current_label);
    }
  }

  assert(all_point_sets.point_set_size() == (int)labels.size());

  cout << "Writing a total of " << all_point_sets.point_size()
       << " points in " << all_point_sets.point_set_size()
       << " point sets to " << output_file << "... " << flush;
  all_point_sets.WriteToFile(output_file.c_str());
  cout << "done." << endl;

  cout << "Writing labels to " << label_file << "... " << flush;
  FILE* f = fopen(label_file.c_str(), "w");
  fprintf(f, "%d\n", (int)labels.size());
  for (int ii = 0; ii < (int)labels.size(); ++ii) {
    fprintf(f, "%d\n", labels[ii]);
  }
  fclose(f);
  cout << "done." << endl;

  return 0;

}
