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
#include "histograms/bin.h"
#include "histograms/multi-resolution-histogram.h"

using namespace std;
using namespace libpmk;

void PrintBin(Bin* bin) {
  printf("S:%f\tC:%f\t", bin->size(), bin->count());
  printf("[ ");
  for (int ii = 0; ii < (int)bin->index().size(); ++ii) {
    printf(" %d ", bin->index()[ii]);
  }
  printf("]\n");
}


int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <input_mrh_file>\n", argv[0]);
    exit(0);
  }

  string input_file(argv[1]);

  ifstream input_stream(input_file.c_str(), ios::binary);
  vector<MultiResolutionHistogram*> pyramids =
    MultiResolutionHistogram::ReadFromStream(input_stream);
  input_stream.close();

  for (int ii = 0; ii < (int)pyramids.size(); ++ii) {
    MultiResolutionHistogram* pyramid = pyramids[ii];
    printf("PYRAMID %d OF %d (%d bins)\n", ii + 1, (int)pyramids.size(),
           pyramid->size());

    vector<Bin*> todo;
    todo.push_back(pyramid->root());
    while (!todo.empty()) {
      Bin* this_bin = todo.back();
      todo.pop_back();
      PrintBin(this_bin);
      Bin* child = (Bin*)(this_bin->first_child());
      while (child != NULL) {
        todo.push_back(child);
        child = (Bin*)(child->next_sibling());
      }
    }


    printf("\n\n");
  }

  for (int ii = 0; ii < (int)pyramids.size(); ++ii) {
    delete pyramids[ii];
  }
  pyramids.clear();
  return 0;
}
