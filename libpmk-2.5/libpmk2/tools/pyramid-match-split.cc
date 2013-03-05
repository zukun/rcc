// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a file with Pyramids, computes the pyramid match kernel.
// It makes a num_test x num_train unnormalized matrix.

#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "histograms/multi-resolution-histogram.h"
#include "pyramids/pyramid-matcher.h"
#include "kernel/matrix.h"
#include "util/bin-weight-scheme.h"

using namespace std;
using namespace libpmk;
using namespace libpmk_util;

void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name << " train.mrh test.mrh output.mtx\n\n";
  cerr << "<train.mrh>:  Train pyramids\n";
  cerr << "<test.mrh>:  Test pyramids\n";
  cerr << "<output.kern>: Where to write the resulting matrix to\n";
}

int main(int argc, char** argv) {
  if (argc != 4) {
    usage(argv[0]);
    exit(0);
  }

  string train_file(argv[1]);
  string test_file(argv[2]);
  string output_file(argv[3]);

  cout << "Reading pyramids from " << train_file << "... " << flush;
  vector<MultiResolutionHistogram*>
    train(MultiResolutionHistogram::ReadFromFile(train_file.c_str()));
  cout << "done." << endl;
  
  cout << "Reading pyramids from " << test_file << "... " << flush;
  vector<MultiResolutionHistogram*>
    test(MultiResolutionHistogram::ReadFromFile(test_file.c_str()));
  cout << "done." << endl;
  
  Matrix matrix((int)test.size(), (int)train.size());

  for (int ii = 0; ii < (int)test.size(); ++ii) {
    clock_t time_start = clock();
    for (int jj = 0; jj < (int)train.size(); ++jj) {
      matrix.at(ii, jj) =
        PyramidMatcher::GetPyramidMatchSimilarity(*test[ii], *train[jj],
                                                  BIN_WEIGHT_INPUT_SPECIFIC);
    }
    clock_t time_end = clock();

    double row_time = (double)(time_end - time_start) / CLOCKS_PER_SEC;
    double mean_time = row_time / (ii+1);

    cout.precision(6);
    cout << "Row " << ii << " took \t" << fixed << row_time << " seconds: \t"
         << fixed << mean_time << " sec per example" << endl;
  }

  cout << "Writing kernel to " << output_file << "... " << flush;
  matrix.WriteToFile(output_file.c_str());
  cout << "done." << endl;

  return 0;
}
