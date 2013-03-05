// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include "kernel/kernel-matrix.h"

using namespace std;
using namespace libpmk_util;

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <input_kern_file>\n", argv[0]);
    exit(0);
  }

  string input_file(argv[1]);
  KernelMatrix km;

  ifstream input_stream(input_file.c_str(), ios::binary);
  km.ReadFromStream(input_stream);
  input_stream.close();

  for (int ii = 0; ii < (int)km.size(); ++ii) {
    for (int jj = 0; jj < (int)km.size(); ++jj) {
      printf("%f ", km.at(ii, jj));
    }
    printf("\n");
  }

  return 0;
}
