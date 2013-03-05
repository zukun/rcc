// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <sstream>
#include "clustering/rnn-clusterer.h"
#include "point_set/point-set.h"
#include "point_set/point-ref.h"
#include "point_set/point.h"
#include "point_set/mutable-point-set-list.h"


using namespace std;
using namespace libpmk;

void RunAllTests() {
  PointSet p(2);

  Point f1(2);
  Point f2(2);
  Point f3(2);
  Point f4(2);
  Point f5(2);
  Point f6(2);
  Point f7(2);
  Point f8(2);

  f1[0] = 0; f1[1] = 0;
  f2[0] = 0; f2[1] = 1;
  f3[0] = 1; f3[1] = 0;
  f4[0] = 1; f4[1] = 1;
  f5[0] = 100; f5[1] = 100;
  f6[0] = 101; f6[1] = 100;
  f7[0] = 100; f7[1] = 101;
  f8[0] = 101; f8[1] = 101;

  p.add_point(f1);
  p.add_point(f2);
  p.add_point(f3);
  p.add_point(f4);
  p.add_point(f5);
  p.add_point(f6);
  p.add_point(f7);
  p.add_point(f8);

  MutablePointSetList psl;
  psl.add_point_set(p);

  vector<PointRef> points;
  for (int ii = 0; ii < 8; ++ii) {
    PointRef new_ref(&psl, ii);
    points.push_back(new_ref);
  }

  RNNClusterer clusterer(-5000000);
  clusterer.Cluster(points);
  for (int ii = 0; ii < (int)clusterer.membership_size(); ++ii) {
    cout << clusterer.membership(ii) << endl;
  }

}

int main(int argc, char** argv) {
  RunAllTests();
  cout << "*******\nPASS\n*******\n";
  return 0;
}
