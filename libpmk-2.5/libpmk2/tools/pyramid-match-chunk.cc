// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a pyramid file, and a list of pyramid files, computes some rows
// of a kernel matrix. It will compare each example in the "main" pyramid
// file to each example in the list, outputting a text file.

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
  cerr << "<output.txt>\n";
  cerr << "<input.mrh>:  Input pyramids\n";
  cerr << "<output.kern>: Where to write the resulting kernel to\n";
}

int main(int argc, char** argv) {
  if (argc <= 3) {
    usage(argv[0]);
    exit(0);
  }

  string output_file(argv[1]);
  string main_mrh(argv[2]);

  cout << "Reading pyramids from " << main_mrh << "... " << flush;
  vector<MultiResolutionHistogram*>
    main_pyramids(MultiResolutionHistogram::ReadFromFile(main_mrh.c_str()));
  cout << "done." << endl;

  vector<vector<double> > matrix;
  matrix.resize((int)main_pyramids.size());

  int chunk_offset = 0;
  for (int ii = 3; ii < argc; ++ii) {
    cout << "Reading pyramids from " << argv[ii] << flush;
    vector<MultiResolutionHistogram*>
      chunk_pyramids(MultiResolutionHistogram::ReadFromFile(argv[ii]));
    cout << "done." << endl;

    for (int jj = 0; jj < (int)main_pyramids.size(); ++jj) {
      int row = jj;
      matrix[row].resize(chunk_offset + (int)chunk_pyramids.size());
      for (int kk = 0; kk < (int)chunk_pyramids.size(); ++kk) {
        int col = chunk_offset + kk;
        matrix[row][col] =
          PyramidMatcher::GetPyramidMatchSimilarity(*main_pyramids[jj],
                                                    *chunk_pyramids[kk],
                                                    BIN_WEIGHT_INPUT_SPECIFIC);
      }
    }
    chunk_offset += chunk_pyramids.size();
    for (int kk = 0; kk < (int)chunk_pyramids.size(); ++kk) {
      delete chunk_pyramids[kk];
    }
  }


  FILE* f = fopen(output_file.c_str(), "w+");
  for (int ii = 0; ii < (int)matrix.size(); ++ii) {
    for (int jj = 0; jj < (int)matrix[ii].size(); ++jj) {
      fprintf(f, "%f ", 100000*matrix[ii][jj]);
    }
    fprintf(f, "\n");
  }
  fclose(f);
  return 0;
}
