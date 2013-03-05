// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include "point_set/point-set.h"
#include "point_set/point.h"

using namespace std;
using namespace libpmk;

void CheckDefaultFeature(const Point& f) {
   for (int i = 0; i < 128; ++i) {
      assert(f[i] == i);
   }
}

void RunAllTests() {
   PointSet p(128);
   assert(p.size() == 0);
   assert(p.point_dim() == 128);

   Point f(128);
   for (int i = 0; i < 128; ++i) {
      f[i] = i;
   }

   p.add_point(f);
   assert(p.size() == 1);

   p.add_point(f);
   assert(p.size() == 2);

   Point* added = p.add_point();
   added->CopyFrom(f);
   assert(p.size() == 3);

   CheckDefaultFeature(p.point(0));
   CheckDefaultFeature(p.point(1));
   CheckDefaultFeature(p.point(2));


   // Test to make sure certain operations copy, rather than mutate
   // the data
   Point g = p.point(1);
   g[0] = 394;
   CheckDefaultFeature(p.point(1));

   p.mutable_point(0)->CopyFrom(g);
   CheckDefaultFeature(p.point(1));
   CheckDefaultFeature(p.point(2));

   const Point& h = p.point(0);
   assert(h[0] == 394);
   for (int i = 1; i < 128; ++i) {
      assert(h[i] == i);
   }   
}

int main(int argc, char** argv) {
   RunAllTests();
   cout << "*******\nPASS\n*******\n";
   return 0;
}
