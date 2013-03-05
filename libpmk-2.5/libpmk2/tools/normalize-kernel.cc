// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Normalizes a kernel matrix.

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "kernel/kernel-matrix.h"

using namespace std;
using namespace libpmk_util;

void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name << " input.mrh output.kern\n\n";
  cerr << "<input.kern>:  Input kernel\n";
  cerr << "<output.kern>: Where to write the resulting kernel to\n";
}

int main(int argc, char** argv) {
  if (argc != 3) {
    usage(argv[0]);
    exit(0);
  }

  string input_file(argv[1]);
  string output_file(argv[2]);

  KernelMatrix km;
  km.ReadFromFile(input_file.c_str());
  km.Normalize();

  cout << "Writing kernel to " << output_file << "... " << flush;
  km.WriteToFile(output_file.c_str());
  cout << "done." << endl;

  return 0;
}
