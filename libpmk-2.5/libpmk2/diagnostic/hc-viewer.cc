// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdio.h>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "util/sparse-vector.h"
#include "point_set/point-set-list.h"
#include "clustering/hierarchical-clusterer.h"

using namespace std;
using namespace libpmk;

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <input_hc_file>\n", argv[0]);
    exit(0);
  }

  HierarchicalClusterer hc;
  string input_file(argv[1]);

  ifstream input_stream(input_file.c_str(), ios::binary);
  hc.ReadFromStream(input_stream);
  input_stream.close();

  const Tree<PointTreeNode>& centers = hc.centers();

  Tree<PointTreeNode>::PreorderIterator iter = centers.BeginPreorder();
  while (iter != centers.EndPreorder()) {
    printf("Center: [node ID: %d] [value: %f]\n",
           iter.id(), centers.node(iter.id())->point()[0]);
    ++iter;
  }

  int num_leaves = 0;
  iter = centers.BeginPreorder();
  while (iter != centers.EndPreorder()) {
    const PointTreeNode* node = centers.node(*iter);
    if (!node->has_child()) {
      ++num_leaves;
    }
    ++iter;
  }

  // Maps leaf node ID -> # of elements in that leaf.
  map<int, int> membership_map;
  for (int ii = 0; ii < hc.membership_size(); ++ii) {
    int leaf_id = hc.membership(ii);
    ++membership_map[leaf_id];
  }

  cout << "Number of leaves: " << num_leaves << endl;
  cout << "Membership map size: " << (int)membership_map.size() << endl;

  cout << "Points per leaf, total avg: "
       << (double)hc.membership_size() / num_leaves << endl;
  cout << "Points per leaf, not counting 0s: "
       << (double)hc.membership_size() / membership_map.size() << endl;

  return 0;
}
