// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
#ifndef HISTOGRAMS_BIN_H
#define HISTOGRAMS_BIN_H

#include <iostream>
#include "util/sparse-vector.h"
#include "tree/sparse-tree-node.h"

namespace libpmk {
/// Encapsulates a histogram bin.
/**
 * A Bin is implemented as a SparseTreeNode with two data members: a size
 * and a count.
 */
class Bin : public SparseTreeNode {
public:
  Bin();
  Bin(const LargeIndex& index);
  virtual ~Bin() { }

  /// Sets the size of the bin.
  void set_size(double size) { size_ = size; }

  /// Get the size of the bin.
  double size() const { return size_; }

  /// Set the count.
  void set_count(double count) { count_ = count; }

  /// Get the current count.
  double count() const { return count_; }

  virtual void ReadData(istream& input_stream);
  virtual void WriteData(ostream& output_stream) const;

  /// Combine data from two Bins.
  /**
   * To combine bins, we take the max of the sizes, and add the
   * counts.
   */
  virtual void Combine(const SparseTreeNode& other);

private:
  double size_;
  double count_;
};
}  // namespace libpmk
#endif  // HISTOGRAMS_BIN_H
