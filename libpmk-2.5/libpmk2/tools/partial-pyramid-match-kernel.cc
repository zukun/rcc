// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a file with Pyramids, computes the pyramid match kernel.
// This will output rows <start> through <finish> of the kernel,
// inclusive.  The top row of the kernel is row 0. The overall matrix
// is a lower triangle, so row n will have n+1 elements.
// The output is written in ASCII, one line per row.

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
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
  cerr << "<output.txt>: Where to write the resulting kernel to\n";
  cerr << "<start_row>: (Row 0 is the top row)\n";
  cerr << "<end_row>: (Row 0 is the top row)\n";
  cerr << "<scale_factor>: Multiply all output by this number\n";
}

int main(int argc, char** argv) {
  if (argc != 6) {
    usage(argv[0]);
    exit(0);
  }

  string input_file(argv[1]);
  string output_file(argv[2]);
  int start_row = atoi(argv[3]);
  int end_row = atoi(argv[4]);
  int multiply_factor = atoi(argv[5]);

  cout << "Reading pyramids from " << input_file << "... " << flush;
  vector<MultiResolutionHistogram*>
    vec(MultiResolutionHistogram::ReadFromFile(input_file.c_str()));
  cout << "done." << endl;

  FILE* f = fopen(output_file.c_str(), "w");

  for (int ii = start_row; ii <= end_row; ++ii) {
    for (int jj = 0; jj <= ii; ++jj) {
      double value = multiply_factor *
        PyramidMatcher::GetPyramidMatchSimilarity(*vec[ii], *vec[jj],
                                                  BIN_WEIGHT_INPUT_SPECIFIC);
      fprintf(f, "%f ", value);
    }
    fprintf(f, "\n");
  }
  fclose(f);
  return 0;
}
