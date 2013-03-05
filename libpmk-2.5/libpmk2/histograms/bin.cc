// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include "histograms/bin.h"
#include "util/sparse-vector.h"

namespace libpmk {

Bin::Bin() : SparseTreeNode(), size_(0), count_(0) { }
Bin::Bin(const LargeIndex& index) :
  SparseTreeNode(index), size_(0), count_(0) { }

void Bin::ReadData(istream& input_stream) {
  input_stream.read((char *)&size_, sizeof(double));
  input_stream.read((char *)&count_, sizeof(double));
}

void Bin::WriteData(ostream& output_stream) const {
  output_stream.write((char *)&size_, sizeof(double));
  output_stream.write((char *)&count_, sizeof(double));
}

// To combine Bins, we'll keep the max of the two sizes and add the
// counts.
void Bin::Combine(const SparseTreeNode& tree_other) {
  const Bin& other(static_cast<const Bin&>(tree_other));
  double max_size = other.size_ > size_ ? other.size_ : size_;
  size_ = max_size;
  count_ += other.count_;
}

}  // namespace libpmk
