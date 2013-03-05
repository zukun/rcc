// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Reverse the order of PointSets in a PSL.
//

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
  cout << "Usage: " << exec << " <input.psl> <output.psl> "
       << endl;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    usage(argv[0]);
    exit(0);
  }

  string input_psl_name = argv[1];
  string output_psl_name = argv[2];

  cout << "Reading " << input_psl_name << "... " << flush;
  MutablePointSetList input_psl;
  input_psl.ReadFromFile(input_psl_name.c_str());
  cout << "done." << endl;

  MutablePointSetList output_psl;
  for (int jj = input_psl.point_set_size() - 1; jj >= 0; --jj) {
      output_psl.add_point_set(input_psl[jj]);
  }
  cout << "done." << endl;
  
  cout << "Writing " << output_psl_name << "... " << flush;
  output_psl.WriteToFile(output_psl_name.c_str());
  cout << "done." << endl;

  return 0;
}
