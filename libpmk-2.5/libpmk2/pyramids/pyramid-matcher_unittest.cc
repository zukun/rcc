// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

// TODO: turn this into a real unit test

#include <assert.h>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include "histograms/multi-resolution-histogram.h"
#include "pyramids/pyramid-matcher.h"
#include "kernel/kernel-matrix.h"
#include "util/bin-weight-scheme.h"

using namespace std;
using namespace libpmk;
using namespace libpmk_util;

int main(int argc, char** argv) {
  ifstream input_stream("testdata/testpyramids.pyr",
                        ios::binary | ios::in);
  vector<MultiResolutionHistogram*> vec(
    MultiResolutionHistogram::ReadFromStream(input_stream));

  KernelMatrix km(vec.size());

  for (int ii = 0; ii < (int)vec.size(); ++ii) {
//      printf("row %d\n", ii);
    clock_t timestart = clock();
    for (int jj = 0; jj <= ii; ++jj) {
      km.at(ii, jj) = PyramidMatcher::GetPyramidMatchCost(*vec[ii], *vec[jj],
                                                          BIN_WEIGHT_INPUT_SPECIFIC);
//         double stuff = PyramidMatcher::MatchPyramidsSlow(*vec[ii], *vec[jj]);
//         printf("%f/%f ", km.at(ii,jj), stuff);
//         assert(fabs(km.at(ii,jj) - stuff) < 0.0001);
    }
//      printf("\n");
    clock_t timeend = clock();
    double rowTime = (double) (timeend - timestart) / CLOCKS_PER_SEC;
    double meanPerTime = rowTime / (double)(ii+1);
    fprintf(stderr, "Computed row %d of %d, took %lf seconds: on average %lf seconds per example\n", ii, (int)vec.size(), rowTime, meanPerTime);


  }


//  km.Normalize();
  for (int ii = 0; ii < (int)vec.size(); ++ii) {
    for (int jj = 0; jj < (int)vec.size(); ++jj) {
      printf("%f ", km.at(ii, jj));
    }
    printf("\n");
  }
  ofstream output_stream("testdata/testkernel.kern",
                         ios::binary | ios::out | ios::trunc);
  km.WriteToStream(output_stream);

  return 0;
}
