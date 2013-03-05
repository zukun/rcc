// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <math.h>
#include <assert.h>
#include <iostream>
#include "histograms/bin.h"

using namespace std;
using namespace libpmk;

void assert_double(double value, double desired) {
  assert(fabs(value - desired) < 0.001);
}

void RunAllTests() {
  LargeIndex index;
  index.push_back(1);
  index.push_back(2);
  index.push_back(3);
  index.push_back(4);

  Bin b(index);
  assert_double(b.size(), 0);
  assert_double(b.count(), 0);

  b.set_size(53);
  assert_double(b.size(), 53);

  b.set_count(b.count() + 1);
  assert_double(b.count(), 1);

  b.set_count(9823);
  assert_double(b.count(), 9823);

  b.set_count(b.count() - 1);
  assert_double(b.count(), 9822);
}

int main(int argc, char** argv) {
  RunAllTests();
  cout << "******\nPASS\n******" << endl;
  return 0;
}
