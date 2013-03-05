// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This program takes point sets and creates uniformly-sized pyramids.

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "histograms/bin.h"
#include "pyramids/uniform-pyramid-maker.h"
#include "point_set/on-disk-point-set-list.h"

using namespace libpmk;

void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name << " input.psl output.mrh f s d\n\n";
  cerr << "<input.psl>: A PointSetList file, where each PointSet contains\n"
       << "             features for one image\n";
  cerr << "<output.mrh>: The resulting pyramids\n";
  cerr << "<f>:        (double) The finest side length\n";
  cerr << "<s>:        (double) Side length factor\n";
  cerr << "<d>:        (int) Scale points by multiplying by 10^(d)\n";
}


int main(int argc, char** argv) {
  if (argc != 6) {
    usage(argv[0]);
    exit(0);
  }

  string input_file(argv[1]);
  string output_file(argv[2]);
  double finest_side_length = atof(argv[3]);
  double side_length_factor = atof(argv[4]);
  int discretize_order = atoi(argv[5]);

  // Set the random seed for translations
  srand(time(NULL));

  cout << "Reading point sets from " << input_file << "... " << flush;
  OnDiskPointSetList psl(input_file, 1, 600);
  cout << "done." << endl;

  cout << "Initializing pyramid maker... " << flush;
  UniformPyramidMaker upm;
  upm.Preprocess(psl, finest_side_length, side_length_factor,
                 discretize_order, true, true);
  cout << " created " << upm.num_levels() << " levels." << endl;

  cout << "Making pyramids... " << flush;
  ofstream output_stream(output_file.c_str(), ios::binary | ios::trunc);
  int32_t num = psl.point_set_size();
  output_stream.write((char *)(&num), sizeof(int32_t));

  for (int ii = 0; ii < num; ++ii) {
    if ((ii + 1) % 100 == 0) {
      cout << ii + 1 << " of " << num << endl;
    }
    MultiResolutionHistogram* pyramid = upm.MakePyramid(psl[ii]);
    MultiResolutionHistogram::WriteSingleHistogramToStream(output_stream,
                                                           pyramid);
    delete pyramid;
  }
  cout << "done." << endl;

  output_stream.close();
  return 0;
}
