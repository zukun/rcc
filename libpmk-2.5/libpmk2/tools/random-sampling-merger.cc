// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Usage: random-sampling-merger <X> <output_labels> <output_ptsets> <N>
//             <psl1> <psl2> ... <pslN>
// Takes N input point-set-list files, randomly samples X (0 < X <= 1)
// points in each input point set. It will then combine all the point
// sets into one file and generate a label file.

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "point_set/mutable-point-set-list.h"

using namespace std;
using namespace libpmk;

void usage(const char* argv) {
  cerr << "Usage: " << argv << " <X> <output_labels> <output_ptsets> <N> "
       << "<psl1> ... <pslN>" << endl;
  cerr << "The value of <X> must be between 0 and 1." << endl;
}


int main(int argc, char** argv) {
  if (argc < 5) {
    usage(argv[0]);
    exit(0);
  }

  double percentage(atof(argv[1]));
  string label_file(argv[2]);
  string output_file(argv[3]);
  int num_inputs(atoi(argv[4]));

  if (percentage < 0 || percentage > 1) {
    usage(argv[0]);
    exit(0);
  }

  if (argc != 5 + num_inputs) {
    usage(argv[0]);
    exit(0);
  }

  srand(time(NULL));

  MutablePointSetList all_point_sets;
  vector<int> labels;

  for (int ii = 5; ii < argc; ++ii) {
    int current_label = ii - 4;
    cout << "Sampling set " << current_label
         << " (" << argv[ii] << ")... " << flush;
    MutablePointSetList point_sets;
    point_sets.ReadFromFile(argv[ii]);

    for (int zz = 0; zz < point_sets.point_set_size(); ++zz) {
      int num_points = point_sets.point_set(zz).size();
      int num_to_pick = (int)ceil(percentage * num_points);

      // This bit array contains "true" for features we want to keep
      // and "false" for the rest.
      vector<bool> include_set(num_points, false);
      int num_included = 0;
      while (num_included < num_to_pick) {
        int candidate = rand() % num_points;
        if (include_set.at(candidate) == false) {
          include_set.at(candidate) = true;
          num_included++;
        }
      }

      for (int jj = num_points - 1; jj >= 0; --jj) {
        if (include_set[jj] == false) {
          point_sets.mutable_point_set(zz)->remove_point(jj);
        }
      }

      // Now that we're done sampling, copy this point set into the
      // global <all_point_sets>.
      all_point_sets.add_point_set(point_sets.point_set(zz));
      labels.push_back(current_label);
    }

    cout << "done." << endl;
  }


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
