// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a PSL, and a bunch of class labels, will select just those
// and make a PSL out of it.

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include "point_set/point-set.h"
#include "point_set/mutable-point-set-list.h"

using namespace libpmk;
using namespace std;

void usage(const char* exec) {
  cout << "Usage: " << exec << " <input.psl> <labels.txt> <output.psl> <label1> <label2> ... <labelN>" << endl;
}

vector<int> ReadLabels(string filename) {
  FILE* f = fopen(filename.c_str(), "r");
  int rows;
  assert(fscanf(f, "%d", &rows) != 0);

  vector<int> labels;
  for (int ii = 0; ii < rows; ++ii) {
    int value;
    assert(fscanf(f, "%d", &value) != 0);
    labels.push_back(value);
  }
  return labels;
}

int main(int argc, char** argv) {
  if (argc < 5) {
    usage(argv[0]);
    exit(0);
  }

  string input_psl_name = argv[1];
  string labels_name = argv[2];
  string output_psl_name = argv[3];
  int num_labels = argc - 4;

  vector<int> labels = ReadLabels(labels_name);

  cout << "Reading " << input_psl_name << "... " << flush;
  MutablePointSetList input_psl;
  input_psl.ReadFromFile(input_psl_name.c_str());
  cout << "done." << endl;

  vector<bool> keep_psl(input_psl.point_set_size(), false);
  int num_kept = 0;

  cout << "Filtering point sets... " << flush;
  for (int ii = 0; ii < num_labels; ++ii) {
    int label = atoi(argv[4 + ii]);
    for (int jj = 0; jj < input_psl.point_set_size(); ++jj) {
      if (labels[jj] == label) {
        if (!keep_psl[jj]) {
          keep_psl[jj] = true;
          ++num_kept;
        }
      }
    }
  }

  MutablePointSetList output_psl;
  for (int jj = 0; jj < input_psl.point_set_size(); ++jj) {
    if (keep_psl[jj]) {
      output_psl.add_point_set(input_psl[jj]);
    }
  }
  cout << "done. Kept " << num_kept << " point sets." << endl;
  
  cout << "Writing " << output_psl_name << "... " << flush;
  output_psl.WriteToFile(output_psl_name.c_str());
  cout << "done." << endl;

  return 0;
}
