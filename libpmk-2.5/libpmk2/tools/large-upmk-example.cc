// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a file with Pyramids, computes the pyramid match kernel.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "histograms/multi-resolution-histogram.h"
#include "pyramids/pyramid-matcher.h"
#include "pyramids/uniform-pyramid-maker.h"
#include "kernel/kernel-matrix.h"
#include "point_set/on-disk-point-set-list.h"

using namespace std;
using namespace libpmk;
using namespace libpmk_util;

void usage(const char* exec_name) {
   cerr << "Usage: " << exec_name << " input.psl output.kern\n\n";
   cerr << "<input.psl>:  Input point sets\n";
   cerr << "<output.kern>: Where to write the resulting kernel to\n";
}

string GetPyramidFilename(int ii) {
   ostringstream filename;
   filename << "pyramid_" << ii << ".mrh";   
   return filename.str();
}

int main(int argc, char** argv) {
   if (argc != 3) {
      usage(argv[0]);
      exit(0);
   }

   string input_file(argv[1]);
   string output_file(argv[2]);

   int num_pieces = 10;
   int piece_size;

   // Set the random seed for translations
   srand(time(NULL));

   auto_ptr<OnDiskPointSetList>
      disk_point_sets(new OnDiskPointSetList(input_file, 1, 100));

   int kernel_size = disk_point_sets->GetNumPointSets();
   KernelMatrix km(kernel_size);
   
   UniformPyramidMaker upm;
   upm.Preprocess(*disk_point_sets, 5, 2, -3, false, false);
   
   piece_size = disk_point_sets->GetNumPointSets() / num_pieces;
   for (int ii = 0; ii < num_pieces; ++ii) {
      cout << "Creating pyramids: piece " << ii+1
           << " of " << num_pieces << endl;
      
      int start = ii * piece_size;
      int end = start + piece_size - 1;
      vector<MultiResolutionHistogram*> pyramids;
      for (int jj = start; jj <= end; ++jj) {
         pyramids.push_back(upm.MakePyramid((*disk_point_sets)[jj]));
      }
      
      ofstream output_stream(GetPyramidFilename(ii).c_str(),
                             ios::binary | ios::trunc);
      MultiResolutionHistogram::WriteToStream(output_stream, pyramids);
      output_stream.close();
      
      // Delete the pyramids:
      for (int jj = 0; jj < (int)pyramids.size(); ++jj) {
         delete pyramids[jj];
      }
      pyramids.clear();
   }

   // Free the memory
   disk_point_sets.reset(NULL);

   cout << "Done creating pyramids." << endl;
   for (int ii = 0; ii < num_pieces; ++ii) {
      ifstream input_stream1(GetPyramidFilename(ii).c_str(), ios::binary);
      vector<MultiResolutionHistogram*> pyramids1 =
         MultiResolutionHistogram::ReadFromStream(input_stream1);
      input_stream1.close();
      int offset1 = ii * piece_size;

      for (int jj = 0; jj <= ii; ++jj) {
         ifstream input_stream2(GetPyramidFilename(jj).c_str(), ios::binary);
         vector<MultiResolutionHistogram*> pyramids2 =
            MultiResolutionHistogram::ReadFromStream(input_stream2);
         input_stream2.close();
         cout << "Computing kernel: pieces " << ii << " and " << jj << endl;
         int offset2 = jj * piece_size;
         
         for (int r1 = 0; r1 < piece_size; ++r1) {
            for (int r2 = 0; r2 < piece_size; ++r2) {
               int ind1 = r1 + offset1;
               int ind2 = r2 + offset2;
               km.at(ind1, ind2) = PyramidMatcher::
                  GetPyramidMatchSimilarity(*pyramids1[r1], *pyramids2[r2],
                                            BIN_WEIGHT_GLOBAL);
            }
         }

         for (int kk = 0; kk < (int)pyramids2.size(); ++kk) {
            delete pyramids2[kk];
         }
         pyramids2.clear();
      }

      for (int kk = 0; kk < (int)pyramids1.size(); ++kk) {
         delete pyramids1[kk];
      }
      pyramids1.clear();
   }

   km.Normalize();
   ofstream output_stream(output_file.c_str(), ios::binary | ios::trunc);
   km.WriteToStream(output_stream);
   output_stream.close();
   return 0;
}
