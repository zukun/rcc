// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This tests hierarchical clustering on a simple 1D data set.
// We'll test 3 levels with a branch factor of 2.
// The top level cluster center should be 0.
// The 2nd level should give + and - 50.
// The 3rd level should give -53.5, -46.5, 46.5, and 53.5.
//
// Sample output (we don't assert this in the code below, since
// HierarchicalClusterer provides no requirement on the node IDs
// or ordering of children):
// Center: [node ID: 0] [value: 0.000000]
// Center: [node ID: 1] [value: 50.000000]
// Center: [node ID: 2] [value: -50.000000]
// Center: [node ID: 3] [value: 53.500000]
// Center: [node ID: 4] [value: 46.500000]
// Center: [node ID: 5] [value: -46.500000]
// Center: [node ID: 6] [value: -53.500000]
//
// Membership table
// Point -54.000000:
//  * belongs to node ID 6
//  * has ID path [ 0 2 6 ]
//  * has tree path [ 1 1 ]
// Point -53.000000:
//  * belongs to node ID 6
//  * has ID path [ 0 2 6 ]
//  * has tree path [ 1 1 ]
// Point -47.000000:
//  * belongs to node ID 5
//  * has ID path [ 0 2 5 ]
//  * has tree path [ 1 0 ]
// Point -46.000000:
//  * belongs to node ID 5
//  * has ID path [ 0 2 5 ]
//  * has tree path [ 1 0 ]
// Point 46.000000:
//  * belongs to node ID 4
//  * has ID path [ 0 1 4 ]
//  * has tree path [ 0 1 ]
// Point 47.000000:
//  * belongs to node ID 4
//  * has ID path [ 0 1 4 ]
//  * has tree path [ 0 1 ]
// Point 53.000000:
//  * belongs to node ID 3
//  * has ID path [ 0 1 3 ]
//  * has tree path [ 0 0 ]
// Point 54.000000:
//  * belongs to node ID 3
//  * has ID path [ 0 1 3 ]
//  * has tree path [ 0 0 ]
//

#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <sstream>
#include "clustering/hierarchical-clusterer.h"
#include "point_set/point-set.h"
#include "point_set/mutable-point-set-list.h"
#include "point_set/point-ref.h"
#include "point_set/point.h"

using namespace std;
using namespace libpmk;

void TestReadAndWrite(HierarchicalClusterer& c1) {
  ostringstream out_stream(ios::binary);
  c1.WriteToStream(out_stream);
  string s = out_stream.str();

  istringstream in_stream(s, ios::binary);
  HierarchicalClusterer c2;
  c2.ReadFromStream(in_stream);

  // Check that the membership tables are equal:
  assert(c1.membership_size() == 8);
  assert(c2.membership_size() == 8);
  for (int ii = 0; ii < c1.membership_size(); ++ii) {
    assert(c1.membership(ii) == c2.membership(ii));
  }


  // Check that the tree contents are equal:
  const Tree<PointTreeNode> p1 = c1.centers();
  const Tree<PointTreeNode> p2 = c2.centers();
  assert(p1.size() == p2.size());
  assert(p1.size() == 7);

  Tree<PointTreeNode>::PreorderIterator iter1 = p1.BeginPreorder();
  Tree<PointTreeNode>::PreorderIterator iter2 = p2.BeginPreorder();
  while (iter1 != p1.EndPreorder()) {
    // Check that they have the same ID:
    assert(iter1.id() == iter2.id());

    // Check that the nodes have the same point:
    const PointTreeNode* node1 = p1.node(iter1.id());
    const PointTreeNode* node2 = p2.node(iter2.id());
    const Point& point1 = node1->point();
    const Point& point2 = node2->point();
    assert(point1.size() == point2.size());
    assert(point1.size() == 1);
    for (int jj = 0; jj < point1.size(); ++jj) {
      assert(point1[jj] == point2[jj]);
    }
    ++iter1;
    ++iter2;
  }
}

void InsertPoint(PointSet& p, int value) {
  Point f(1);
  f[0] = value;
  p.add_point(f);
}

void RunAllTests() {
  PointSet p(1);

  InsertPoint(p, -54);
  InsertPoint(p, -53);

  InsertPoint(p, -47);
  InsertPoint(p, -46);

  InsertPoint(p, 46);
  InsertPoint(p, 47);

  InsertPoint(p, 53);
  InsertPoint(p, 54);

  MutablePointSetList psl;
  psl.add_point_set(p);
  vector<PointRef> points;
  for (int ii = 0; ii < 8; ++ii) {
    PointRef new_ref(&psl, ii);
    points.push_back(new_ref);
  }

  int NUMLEVELS = 3;

  L2DistanceComputer dist_computer;
  HierarchicalClusterer clusterer;
  clusterer.Cluster(NUMLEVELS, 2, points, dist_computer);

  const Tree<PointTreeNode>& centers = clusterer.centers();

  Tree<PointTreeNode>::BreadthFirstIterator iter = centers.BeginBreadthFirst();
  while (iter != centers.EndBreadthFirst()) {
    printf("Center: [node ID: %d] [value: %f]\n",
           iter.id(), centers.node(iter.id())->point()[0]);
    ++iter;
  }

  printf("\nMembership table\n");
  for (int ii = 0; ii < 8; ++ii) {
    list<int> id_path, tree_path;
    clusterer.IdentifyMemberIDPath(ii, &id_path);
    clusterer.IdentifyMemberTreePath(ii, &tree_path);

    printf("Point %f:\n", psl.point(ii)[0]);
    printf(" * belongs to node ID %d\n", clusterer.membership(ii));
    printf(" * has ID path [ ");
    for (list<int>::iterator iter = id_path.begin();
         iter != id_path.end(); ++iter) {
      printf("%d ", *iter);
    }
    printf("]\n");

    printf(" * has tree path [ ");
    for (list<int>::iterator iter = tree_path.begin();
         iter != tree_path.end(); ++iter) {
      printf("%d ", *iter);
    }
    printf("]\n");
  }

  TestReadAndWrite(clusterer);
}

int main(int argc, char** argv) {
  RunAllTests();
  cout << "*******\nPASS\n*******\n";
  return 0;
}
