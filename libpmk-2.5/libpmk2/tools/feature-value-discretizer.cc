// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Usage: feature_value_discretizer <input_psl> <order> <output_psl>
//
// Multiplies all of the values (not weights) in <input_psl> by 10^order
// and writes the output to <output_psl>.

#include <assert.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "point_set/mutable-point-set-list.h"

using namespace std;
using namespace libpmk;

void usage(const char* argv) {
   fprintf(stderr, "Usage: %s <input_psl> <order> <output_psl>\n", argv);
}


int main(int argc, char** argv) {
   if (argc != 4) {
      usage(argv[0]);
      exit(0);
   }

   string input_file(argv[1]);
   double order = atof(argv[2]);
   string output_file(argv[3]);

   MutablePointSetList point_sets;
   ifstream input_stream(input_file.c_str(), ios::binary);
   point_sets.ReadFromStream(input_stream);
   input_stream.close();
   
   double factor = pow(10, order);
   double min_raw_value = 999999;
   double max_raw_value = -999999;
   int count = 0;

   for (int ii = 0; ii < point_sets.GetNumPointSets(); ++ii) {
      for (int jj = 0; jj < point_sets[ii].GetNumFeatures(); ++jj) {
         for (int kk = 0; kk < point_sets[ii].GetFeatureDim(); ++kk) {
            if (point_sets[ii][jj][kk] < min_raw_value) {
               min_raw_value = point_sets[ii][jj][kk];
            }

            if (point_sets[ii][jj][kk] > max_raw_value) {
               max_raw_value = point_sets[ii][jj][kk];
            }

            point_sets[ii][jj][kk] *= factor;
            ++count;
         }
      }      
   }

   printf("%d feature values scaled.\n", count);
   printf("Min raw value: %f (scaled to %f)\n", min_raw_value,
          min_raw_value * factor);
   printf("Max raw value: %f (scaled to %f)\n", max_raw_value,
          max_raw_value * factor);

   printf("Writing %s to disk...\n", output_file.c_str());
   ofstream output_stream(output_file.c_str(), ios::binary | ios::trunc);
   point_sets.WriteToStream(output_stream);
   output_stream.close();
   
   return 0;

}
