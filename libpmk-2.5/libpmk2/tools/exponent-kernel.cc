// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a kernel, will apply exp(-value/sigma) to each entry.

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include "kernel/kernel-matrix.h"

using namespace std;
using namespace libpmk_util;

int main(int argc, char** argv) {
  if (argc != 4) {
    printf("Usage: %s <input1.kern> <sigma> <output.kern>\n", argv[0]);
    exit(0);
  }

  string input_file(argv[1]);
  double sigma = atof(argv[2]);
  string output_file(argv[3]);

  KernelMatrix km;
  KernelMatrix output_km;

  km.ReadFromFile(input_file.c_str());
  output_km.Resize(km.size());

  for (int ii = 0; ii < km.size(); ++ii) {
    for (int jj = 0; jj <= ii; ++jj) {
      output_km.at(ii, jj) = exp(-km.at(ii, jj) / sigma);
    }
  }

  cout << "Writing kernel " << output_file << "... " << flush;
  output_km.WriteToFile(output_file.c_str());
  cout << "done." << endl;

  return 0;
}
