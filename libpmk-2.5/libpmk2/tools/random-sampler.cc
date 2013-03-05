// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Takes a point set list. For each point set, it keeps <percentage>%
// random points and removes the rest. It then writes the new reduced
// point set list to the specified output file.

#include <cstdlib>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include "point_set/mutable-point-set-list.h"

using namespace std;
using namespace libpmk;

void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name << " <input_point_set_list> <output_file> "
       << "<percentage>\n";
  cerr << "The value of <percentage> must be between 0 and 1." << endl;
}

int main(int argc, char** argv) {
  if (argc != 4) {
    usage(argv[0]);
    exit(0);
  }

  string input_file(argv[1]);
  string output_file(argv[2]);
  double percentage = atof(argv[3]);

  if (percentage < 0 || percentage > 1) {
    usage(argv[0]);
    exit(0);
  }

  cout << "Reading point sets from " << input_file << "... " << flush;
  MutablePointSetList point_sets;
  point_sets.ReadFromFile(input_file.c_str());
  cout << "done." << endl;

  for (int ii = 0; ii < point_sets.point_set_size(); ++ii) {
    cout << "Sampling point set " << ii+1 << " of "
         << point_sets.point_set_size() << "... ";
    int num_points = point_sets.point_set(ii).size();
    int num_to_pick = (int)ceil(percentage * num_points);

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
        point_sets.mutable_point_set(ii)->remove_point(jj);
      }
    }

    cout << "done. Kept " << num_to_pick << " of "
         << num_points << "points." << endl;
  }

  printf("Writing a total of %d points.\n", point_sets.point_size());
  cout << "Writing a total of " << point_sets.point_set_size()
       << " points in " << point_sets.point_set_size() << " point sets to "
       << output_file << "... " << flush;
  point_sets.WriteToFile(output_file.c_str());
  cout << "done." << endl;

  return 0;

}
