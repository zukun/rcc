// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <stdint.h>
#include <iostream>
#include "tree/sparse-tree-node.h"

using namespace std;


namespace libpmk {

SparseTreeNode::SparseTreeNode() :
  index_(LargeIndex()), parent_(NULL), next_sibling_(NULL),
  previous_sibling_(NULL), first_child_(NULL), last_child_(NULL) { }

SparseTreeNode::SparseTreeNode(const SparseTreeNode& other) :
  index_(other.index_), parent_(NULL), next_sibling_(NULL),
  previous_sibling_(NULL), first_child_(NULL), last_child_(NULL) { }

SparseTreeNode::SparseTreeNode(const LargeIndex& index) :
  index_(index), parent_(NULL), next_sibling_(NULL), previous_sibling_(NULL),
  first_child_(NULL), last_child_(NULL) { }

SparseTreeNode::~SparseTreeNode() {
   index_.clear();
}

bool SparseTreeNode::operator<(const SparseTreeNode& other) const {
  return index_ < other.index_;
}

bool SparseTreeNode::CompareNodes(const SparseTreeNode* a,
                                  const SparseTreeNode* b) {
  return a->index() < b->index();
}

void SparseTreeNode::ReadFromStream(istream& input_stream) {
   index_.clear();
   int32_t index_size;
   input_stream.read((char *)&index_size, sizeof(int32_t));
   for (int ii = 0; ii < index_size; ++ii) {
      int32_t index_element;
      input_stream.read((char *)&index_element, sizeof(int32_t));
      index_.push_back(index_element);
   }
   ReadData(input_stream);
}

void SparseTreeNode::WriteToStream(ostream& output_stream) const {
   int32_t index_size = index_.size();   
   output_stream.write((char *)&index_size, sizeof(int32_t));
   for (int ii = 0; ii < index_size; ++ii) {
      int32_t element = index_[ii];
      output_stream.write((char *)&element, sizeof(int32_t));
   }
   WriteData(output_stream);
}
}  // namespace libpmk
