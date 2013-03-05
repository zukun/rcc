// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Generates a random point set with the given dim, # of features
// per image, and # of images.
//

#include <math.h>
#include <iostream>
#include <set>
#include <vector>
#include <stdlib.h>
#include "point_set/point-set.h"
#include "point_set/mutable-point-set-list.h"

using namespace libpmk;
using namespace std;

void usage(const char* exec) {
  cout << "Usage: " << exec << " dim num_features_per_image num_images feature_min feature_max output.psl" << endl;
}

int main(int argc, char** argv) {
  if (argc != 7) {
    usage(argv[0]);
    exit(0);
  }

  srand48(0);

  int feature_dim = atoi(argv[1]);
  int num_features_per_image = atoi(argv[2]);
  int num_images = atoi(argv[3]);
  double feature_min = atoi(argv[4]);
  double feature_max = atoi(argv[5]);
  string output_psl_name = argv[6];

  double diameter = feature_max - feature_min;

  MutablePointSetList output_psl;
  for (int ii = 0; ii < num_images; ++ii) {
    cout << "Adding image " << ii << endl;
    PointSet* added_point_set = output_psl.add_point_set(feature_dim);
    for (int jj = 0; jj < num_features_per_image; ++jj) {
      Point* added_point = added_point_set->add_point();
      for (int kk = 0; kk < feature_dim; ++kk) {
        double random = drand48();
        random = (random * diameter) - feature_min;
        added_point->set_feature(kk, random);
      }
    }
  }
  
  cout << "Writing " << output_psl_name << "... " << flush;
  output_psl.WriteToFile(output_psl_name.c_str());
  cout << "done." << endl;

  return 0;
}
