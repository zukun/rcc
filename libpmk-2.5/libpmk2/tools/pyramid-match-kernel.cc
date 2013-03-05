// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a file with Pyramids, computes the pyramid match kernel.

#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "histograms/multi-resolution-histogram.h"
#include "pyramids/pyramid-matcher.h"
#include "kernel/kernel-matrix.h"
#include "util/bin-weight-scheme.h"

using namespace std;
using namespace libpmk;
using namespace libpmk_util;

void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name << " input.mrh output.kern\n\n";
  cerr << "<input.mrh>:  Input pyramids\n";
  cerr << "<output.kern>: Where to write the resulting kernel to\n";
}

int main(int argc, char** argv) {
  if (argc != 3) {
    usage(argv[0]);
    exit(0);
  }

  string input_file(argv[1]);
  string output_file(argv[2]);

  cout << "Reading pyramids from " << input_file << "... " << flush;
  vector<MultiResolutionHistogram*>
    vec(MultiResolutionHistogram::ReadFromFile(input_file.c_str()));
  cout << "done." << endl;
  KernelMatrix km(vec.size());

  for (int ii = 0; ii < (int)vec.size(); ++ii) {
    clock_t time_start = clock();
    for (int jj = 0; jj <= ii; ++jj) {
      km.at(ii, jj) =
        PyramidMatcher::GetPyramidMatchSimilarity(*vec[ii], *vec[jj],
                                                  BIN_WEIGHT_INPUT_SPECIFIC);
    }
    clock_t time_end = clock();

    double row_time = (double)(time_end - time_start) / CLOCKS_PER_SEC;
    double mean_time = row_time / (ii+1);

    cout.precision(6);
    cout << "Row " << ii << " took \t" << fixed << row_time << " seconds: \t"
         << fixed << mean_time << " sec per example" << endl;
  }

  //cout << "Normalizing kernel... " << flush;
  //km.Normalize();
  //cout << "done." << endl;

  cout << "Writing kernel to " << output_file << "... " << flush;
  km.WriteToFile(output_file.c_str());
  cout << "done." << endl;

  return 0;
}
