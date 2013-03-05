// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Cleans, in place, a PSL file.


#include <cmath>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>
#include "point_set/point-set.h"
#include "point_set/mutable-point-set-list.h"

using namespace libpmk;
using namespace std;

void usage(const char* exec) {
  cout << "Usage: " << exec << " <feature_psl>" << endl;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    usage(argv[0]);
    exit(0);
  }

  string feature_psl = argv[1];

  MutablePointSetList features;
  features.ReadFromFile(feature_psl.c_str());

  int num_removed = 0;
  for (int ii = 0; ii < features.point_set_size(); ++ii) {
    PointSet* feature_ps = features.mutable_point_set(ii);

    for (int jj = feature_ps->size() - 1; jj >= 0; --jj) {
      const Point& feature = feature_ps->point(jj);
      if (isinf(feature[0]) || isnan(feature[0]) ||
          feature[0] < -100000) {
        feature_ps->remove_point(jj);
        ++num_removed;
      }
    }
  }

  cout << "Removed " << num_removed << " points.";

  cout << "Writing " << feature_psl << "... " << flush;
  features.WriteToFile(feature_psl.c_str());
  cout << "done." << endl;

  return 0;
}
