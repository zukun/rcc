// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdint.h>
#include <assert.h>
#include <iostream>
#include "tree/point-tree-node.h"
#include "point_set/point.h"

using namespace std;

namespace libpmk {

PointTreeNode::PointTreeNode() : TreeNode(), point_(NULL) { }
PointTreeNode::PointTreeNode(int node_id) : TreeNode(node_id),
                                            point_(NULL) { }

PointTreeNode::PointTreeNode(const PointTreeNode& other) :
  TreeNode(other) {
  point_.reset(new Point(other.point()));
}

void PointTreeNode::Combine(const TreeNode& other) {
  assert(false);
}

void PointTreeNode::ReadData(istream& input_stream) {
  int32_t point_dim;
  input_stream.read((char *)&point_dim, sizeof(int32_t));
  point_.reset(NULL);

  if (point_dim > 0) {
    point_.reset(new Point(point_dim));
    point_->ReadFromStream(input_stream);
  }
}

void PointTreeNode::WriteData(ostream& output_stream) const {
  int32_t point_dim = 0;
  if (has_point()) {
    point_dim = point_->size();
  }
  output_stream.write((char *)&point_dim, sizeof(int32_t));

  if (has_point()) {
    point_->WriteToStream(output_stream);
  }
}
}  // end namespace libpmk
