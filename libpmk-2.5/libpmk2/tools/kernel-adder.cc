// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <cstdio>
#include <string>
#include <fstream>
#include <cstdlib>
#include "kernel/kernel-matrix.h"

using namespace std;
using namespace libpmk_util;

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: %s <output_kern_file> <input_kern_files>\n", argv[0]);
    exit(0);
  }

  string output_file(argv[1]);


  KernelMatrix km;

  for (int ii = 2; ii < argc; ++ii) {
    cout << "Reading file " << argv[ii] << "... " << flush;
    KernelMatrix temp;
    temp.ReadFromFile(argv[ii]);
    km.Resize(temp.size());
    for (int ii = 0; ii < km.size(); ++ii) {
      for (int jj = 0; jj <= ii; ++jj) {
        km.at(ii, jj) += temp.at(ii, jj);
      }
    }
    cout << "done." << endl;
  }

  km.Normalize();
  km.WriteToFile(output_file.c_str());

  return 0;
}
