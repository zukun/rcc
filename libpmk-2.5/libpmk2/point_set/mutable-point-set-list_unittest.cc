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
#include "point_set/mutable-point-set-list.h"

using namespace std;
using namespace libpmk;

void AddSamplePointSet(MutablePointSetList& psl) {
  PointSet x(10);
  Point f(10);
  f[0] = 100;
  f[5] = 1337;
  x.add_point(f);
  psl.add_point_set(x);
}


void TestWritePointers() {
  MutablePointSetList psl;

  PointSet* ps = psl.add_point_set(10);
  Point* f = ps->add_point();
  f->set_feature(0, 100);
  f->set_feature(5, 1337);

  assert(psl.point_set_size() == 1);
  assert(psl.point_dim() == 10);

  ps->add_point();
  assert(psl.point_set_size() == 1);
  assert(psl.point_dim() == 10);
  assert(psl.point_size() == 2);

  // Get a mutable point set, mutate it, and see if it's
  // reflected in the const version of it.
  PointSet* mut = psl.mutable_point_set(0);
  assert(mut == ps);
  mut->mutable_point(0)->set_feature(0, 99);
  assert(mut->mutable_point(0) == f);
  assert(psl.point_set(0).point(0).feature(0) == 99);
}


// psl is a working copy of the entire point set that we want to compare to.
void TestReadSelection(MutablePointSetList& psl, int start, int size) {
  MutablePointSetList segment;
  ifstream input_stream("testdata/test.ptset", ios::binary);
  segment.ReadSelectionFromStream(input_stream, start, size);
  assert(segment.point_set_size() == size);

  for (int jj = 0; jj < size; ++jj) {
    const PointSet& ps = psl[jj + start];
    const PointSet& ps2 = segment.point_set(jj);
    assert(ps2.size() == ps.size());
    assert(ps2.point_dim() == ps.point_dim());
    for (int ii = 0; ii < ps.size(); ++ii) {
      const Point& f = ps.point(ii);
      const Point& f2 = ps2.point(ii);
      assert(f.weight() == f2.weight());
      for (int kk = 0; kk < ps.point_dim(); ++kk) {
        assert(f.feature(kk) == f2.feature(kk));
        assert(f[kk] == f2[kk]);
      }
    }
  }

  input_stream.close();
}
void RunAllTests() {
  MutablePointSetList psl;

  ifstream input_stream("testdata/test.ptset", ios::binary);
  psl.ReadFromStream(input_stream);
  assert(psl.point_set_size() == 400);

  for (int jj = 0; jj < psl.point_set_size(); ++jj) {
    const PointSet& ps = psl[jj];
    for (int ii = 0; ii < ps.size(); ++ii) {
      assert(ps.point(ii).weight() == 1);
    }
  }

  ostringstream output_stream(ios::binary);
  psl.WriteToStream(output_stream);

  string s = output_stream.str();

  // Re-read what we just wrote and compare it to the original
  MutablePointSetList psl2;
  istringstream input_stream2(s, ios::binary);
  psl2.ReadFromStream(input_stream2);

  assert(psl.point_set_size() == psl2.point_set_size());
  for (int jj = 0; jj < psl.point_set_size(); ++jj) {
    const PointSet& ps = psl[jj];
    const PointSet& ps2 = psl2[jj];
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

  // Test ReadSelection.
  // segment1: just the first point set
  // segment2: the entire point set
  // segment3: something in the middle of size 50
  // segment4: something starting from 70 and going to the end
  // segment5: just the last point set
  TestReadSelection(psl, 0, 1);
  TestReadSelection(psl, 0, 400);
  TestReadSelection(psl, 50, 50);
  TestReadSelection(psl, 70, 330);
  TestReadSelection(psl, 399, 1);


  // Create a point set, call AddPointSet, and then let that point set
  // go out of scope; then make sure this didn't affect the newly added
  // point set.

  AddSamplePointSet(psl);
  assert(psl.point_set_size() == 401);

  const PointSet& testps(psl[400]);
  assert(testps.size() == 1);
  assert(testps[0][0] == 100);
  assert(testps[0][5] == 1337);


  TestWritePointers();
}

int main(int argc, char** argv) {
  RunAllTests();
  cout << "******\nPASS\n******" << endl;
  return 0;
}
