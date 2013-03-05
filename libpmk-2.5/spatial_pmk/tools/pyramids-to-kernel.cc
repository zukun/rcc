// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a set of files with Pyramids, computes the PMK for each of
// them and accumulates (sums up) their results. This does NOT normalize
// the resulting kernel.

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

void PrintBin(Bin* bin) {
  printf("S:%f\tC:%f\t", bin->size(), bin->count());
  printf("[ ");
  for (int ii = 0; ii < (int)bin->index().size(); ++ii) {
    printf(" %d ", bin->index()[ii]);
  }
  printf("]\n");
}

vector<int> ReadSizes(string filename) {
   FILE* f = fopen(filename.c_str(), "r");
   int rows;
   assert(fscanf(f, "%d", &rows) != 0);
   
   vector<int> sizes;
   for (int ii = 0; ii < rows; ++ii) {
      int value;
      assert(fscanf(f, "%d", &value) != 0);
      sizes.push_back(value);
   }
   return sizes;
}
void usage(const char* exec_name) {
  cerr << "Usage: " << exec_name << " output.kern sizes input1.mrh input2.mrh ..\n";
  cerr << "<ouput.kern>:  Where to write unnormalized kernel to\n";
  cerr << "<inputs.mrh>: input pyramids\n";
}

int main(int argc, char** argv) {
  if (argc < 4) {
    usage(argv[0]);
    exit(0);
  }

  string output_file(argv[1]);
  //string sizes_file(argv[2]);

  //vector<int> sizes = ReadSizes(sizes_file);

  KernelMatrix km;
  for (int ii = 3; ii < argc; ++ii) {
    string input_file(argv[ii]);
    cout << "Reading pyramids from " << input_file << "... " << flush;
    vector<MultiResolutionHistogram*>
      vec(MultiResolutionHistogram::ReadFromFile(input_file.c_str()));
    cout << "done." << endl;

    for (int ii = 0; ii < (int)vec.size(); ++ii) {
//        cout << "BEFORE NORMALIZING" << endl;
//        MultiResolutionHistogram* pyramid = vec[ii];

//        vector<Bin*> todo;
//        todo.push_back(pyramid->root());
//        while (!todo.empty()) {
//          Bin* this_bin = todo.back();
//          todo.pop_back();
//          PrintBin(this_bin);
//          Bin* child = (Bin*)(this_bin->first_child());
//          while (child != NULL) {
//            todo.push_back(child);
//            child = (Bin*)(child->next_sibling());
//          }
//        }

//      vec[ii]->Normalize(sizes[ii]);
//      cout << "Normalizing by " << sizes[ii] << endl;
//        todo.clear();
//        cout << "AFTER NORMALIZING" << endl;
//        todo.push_back(pyramid->root());
//        while (!todo.empty()) {
//          Bin* this_bin = todo.back();
//          todo.pop_back();
//          PrintBin(this_bin);
//          Bin* child = (Bin*)(this_bin->first_child());
//          while (child != NULL) {
//            todo.push_back(child);
//            child = (Bin*)(child->next_sibling());
//          }
//        }
    }


    km.Resize((int)vec.size());
    for (int ii = 0; ii < (int)vec.size(); ++ii) {

      for (int jj = 0; jj <= ii; ++jj) {
        km.at(ii, jj) +=
          PyramidMatcher::GetPyramidMatchSimilarity(*vec[ii], *vec[jj],
                                                    BIN_WEIGHT_GLOBAL);
      }
    }

    for (int ii = 0; ii < (int)vec.size(); ++ii) {
      delete vec[ii];
    }
  }
  
  cout << "Writing kernel to " << output_file << "... " << flush;
  km.WriteToFile(output_file.c_str());
  cout << "done." << endl;
  
  return 0;
}
