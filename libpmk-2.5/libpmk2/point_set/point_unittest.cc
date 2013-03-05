// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include "point_set/point.h"

using namespace std;
using namespace libpmk;

void RunAllTests() {
  Point f(100);
  assert(f.size() == 100);
   
  for (int ii = 0; ii < 100; ++ii) {
    assert(f[ii] == 0);
    assert(f.feature(ii) == 0);
  }

  f[3] = 100;
  for (int i = 0; i < 100; ++i) {
    if(i == 3) {
      assert(f[i] == 100);
      assert(f.feature(i) == 100);
    } else {
      assert(f[i] == 0);
      assert(f.feature(i) == 0);
    }
  }

  f.set_feature(3, 2);
  for (int i = 0; i < 100; ++i) {
    if(i == 3) {
      assert(f[i] == 2);
      assert(f.feature(i) == 2);
    } else {
      assert(f[i] == 0);
      assert(f.feature(i) == 0);
    }
  }

  assert(f.weight() == 1);
  f.set_weight(153);
  assert(f.weight() == 153);
  assert(f.size() == 100);
}

int main(int argc, char** argv) {
  RunAllTests();
  cout << "*******\nPASS\n*******\n";
  return 0;
}
