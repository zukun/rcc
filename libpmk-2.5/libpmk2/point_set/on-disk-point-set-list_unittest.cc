// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "point_set/on-disk-point-set-list.h"
#include "point_set/mutable-point-set-list.h"

using namespace std;
using namespace libpmk;

// Compare an on-disk version with the in-memory version <psl> to verify.
void TestWithCache(const MutablePointSetList& psl,
                   int area_cache_size, int lru_cache_size) {
  OnDiskPointSetList psl2("testdata/test.ptset", area_cache_size,
                          lru_cache_size);

  assert(psl2.point_set_size() == 400);
  assert(psl2.point_size() == 102400);
  assert(psl2.point_dim() == 10);
  assert(psl.point_set_size() == psl2.point_set_size());

  for (int jj = 0; jj < psl.point_set_size(); ++jj) {
    const PointSet& ps = psl.point_set(jj);
    const PointSet& ps2 = psl2.point_set(jj);
    assert(ps2.size() == ps.size());
    assert(ps2.point_dim() == ps.point_dim());
    for (int ii = 0; ii < ps.size(); ++ii) {
      const Point& f = ps[ii];
      const Point& f2 = ps2[ii];
      assert(f.weight() == f2.weight());
      for (int kk = 0; kk < ps.point_dim(); ++kk) {
        assert(f.feature(kk) == f2.feature(kk));
        assert(f[kk] == f2[kk]);
      }
    }
  }
}

void RunAllTests() {
  MutablePointSetList psl;
  ifstream input_stream("testdata/test.ptset", ios::binary);
  psl.ReadFromStream(input_stream);

  cout << "Testing with cache sizes 2, 5" << endl;
  TestWithCache(psl, 2, 5);

  cout << "Testing with cache sizes 10, 3" << endl;
  TestWithCache(psl, 10, 3);

  cout << "Testing with cache sizes 17, 600" << endl;
  TestWithCache(psl, 17, 600);

  cout << "Testing with cache sizes 398, 1" << endl;
  TestWithCache(psl, 398, 1);

  cout << "Testing with cache sizes 399, 399" << endl;
  TestWithCache(psl, 399, 399);

  cout << "Testing with cache sizes 3000, 398" << endl;
  TestWithCache(psl, 3000, 398);

  cout << "Testing with cache sizes 100000, 17" << endl;
  TestWithCache(psl, 100000, 17);

}

int main(int argc, char** argv) {
  RunAllTests();
  cout << "******\nPASS\n******" << endl;
  return 0;
}
