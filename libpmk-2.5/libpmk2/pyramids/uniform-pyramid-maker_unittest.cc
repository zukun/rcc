// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//


// TODO: turn this into a real unit test
#include <math.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "histograms/bin.h"
#include "point_set/mutable-point-set-list.h"
#include "pyramids/uniform-pyramid-maker.h"

using namespace std;
using namespace libpmk;

int main(int argc, char** argv) {
   MutablePointSetList psl;

   ifstream input_stream("testdata/testpointsets.psl", ios::binary);
   psl.ReadFromStream(input_stream);

   UniformPyramidMaker upm;
   upm.Preprocess(psl, 5, 2, -3, false, false);
   vector<MultiResolutionHistogram*> vec = upm.MakePyramids(psl);

   ofstream output_stream("testdata/testpyramids.pyr",
                          ios::out | ios::binary | ios::trunc);
   MultiResolutionHistogram::WriteToStream(output_stream, vec);
   return 0;
}
