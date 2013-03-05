// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdint.h>
#include <iostream>
#include "tree/tree-node.h"

using namespace std;
using namespace libpmk;

TreeNode::TreeNode() : id_(kInvalidNodeID), parent_(kInvalidNodeID) { }

TreeNode::TreeNode(const TreeNode& other) :
  id_(other.id_), parent_(other.parent_), children_(other.children_) { }

TreeNode::TreeNode(int id) : id_(id), parent_(kInvalidNodeID) { }

void TreeNode::remove_child(int child_index) {
  children_.erase(children_.begin() + child_index);
}


void TreeNode::ReadFromStream(istream& input_stream) {
  input_stream.read((char *)&id_, sizeof(int32_t));
  input_stream.read((char *)&parent_, sizeof(int32_t));

  int32_t child_size;
  input_stream.read((char *)&child_size, sizeof(int32_t));

  children_.clear();
  for (int ii = 0; ii < child_size; ++ii) {
    int32_t child_element;
    input_stream.read((char *)&child_element, sizeof(int32_t));
    children_.push_back(child_element);
  }

  ReadData(input_stream);
}

void TreeNode::WriteToStream(ostream& output_stream) const {
  output_stream.write((char *)&id_, sizeof(int32_t));
  output_stream.write((char *)&parent_, sizeof(int32_t));

  int32_t child_size = children_.size();
  output_stream.write((char *)&child_size, sizeof(int32_t));
  for (int ii = 0; ii < child_size; ++ii) {
    int32_t element = children_[ii];
    output_stream.write((char *)&element, sizeof(int32_t));
  }

  WriteData(output_stream);
}
