// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// For each input PSL, chooses n random images from it, and then
// aggregates them into an output PSL.

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>
#include "point_set/point-set.h"
#include "point_set/mutable-point-set-list.h"

using namespace libpmk;
using namespace std;

void usage(const char* exec) {
  cout << "Usage: " << exec << " <sample_size> <output_psl> <psl1> ... <pslN>" << endl;
}

int main(int argc, char** argv) {
  if (argc < 4) {
    usage(argv[0]);
    exit(0);
  }

  srand(time(NULL));

  int sample_size = atoi(argv[1]);
  string output_file(argv[2]);
  int num_files = argc - 3;

  MutablePointSetList sample;

  for (int ii = 3; ii < argc; ++ii) {
    int psl_index = ii - 3;
    cout << "Processing " << psl_index << " of " << num_files << "..."
         << flush;
    MutablePointSetList temp;
    temp.ReadFromFile(argv[ii]);

    set<int> chosen_indices;    
    while ((int)chosen_indices.size() != sample_size) {
      chosen_indices.insert(rand() % temp.point_set_size());
    }
    
    for (set<int>::iterator iter = chosen_indices.begin();
         iter != chosen_indices.end(); ++iter) {
      int index = *iter;
      sample.add_point_set(temp.point_set(index));
    }
    cout << "done." << endl;
  }

  cout << "Using sample with " << sample.point_set_size()
       << " images and " << sample.point_size()
       << " total points." << endl;

  cout << "Writing output to " << output_file << "..." << flush;
  sample.WriteToFile(output_file.c_str());
  cout << "done." << endl;
  return 0;
}
