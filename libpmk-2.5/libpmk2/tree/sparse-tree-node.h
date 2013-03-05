// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef TREE_SPARSE_TREE_NODE_H
#define TREE_SPARSE_TREE_NODE_H

#include <iostream>
#include "util/sparse-vector.h"

using namespace std;

namespace libpmk {

/// An indexed node, used by SparseTree.
/**
 * A SparseTreeNode has an identifier of type LargeIndex (a vector of
 * ints). SparseTreeNode also contains pointers to parents, children,
 * and siblings, but makes no effort to maintain about them, and as
 * such, we make no guarantees on it. For example, the "sibling"
 * pointer can point to an arbitrary bin which may or may not be the
 * real sibling. Tree handles the getting/setting of these pointers.
 *
 * To augment a SparseTreeNode with your own data, there are three functions
 * you need to provide: (1) reading the data from a stream, (2)
 * writing the data to a stream, and (3) how to combine two nodes. The
 * definition of "combine" will vary depending on what you're doing,
 * but the general idea is that sometimes you will have two SparseTreeNodes
 * with the same LargeIndex that you want to insert into a tree.  It
 * is advisable that you make the output of Combine() is symmetric,
 * since there are no guarantees on what Tree does with
 * duplicate-index bins other than calling Combine().
 *
 * Remember that if you implement a copy constructor for your SparseTreeNode,
 * you should have your copy constructor call the base class's copy
 * constructor: <br>
 * <tt>
 * MyNode::MyNode(const MyNode& other) <b>: SparseTreeNode(other)</b> {<br>
 * // your copy code<br>
 * }
 * </tt>
 */
class SparseTreeNode {
public:
  /// Create a new node with an empty index.
  SparseTreeNode();

  /// Create a new node with the given index.
  SparseTreeNode(const LargeIndex& index);

  /// A 'copy' constructor, which only copies over the index.
  /**
   * The pointers to data will NOT be copied over; they will be NULL.
   */
  SparseTreeNode(const SparseTreeNode& other);

  virtual ~SparseTreeNode();

  const LargeIndex& index() const { return index_; }
  SparseTreeNode* parent() { return parent_; }
  SparseTreeNode* prev_sibling() { return previous_sibling_; }
  SparseTreeNode* next_sibling() { return next_sibling_; }
  SparseTreeNode* first_child() { return first_child_; }
  SparseTreeNode* last_child() { return last_child_; }

  void set_parent(SparseTreeNode* parent) { parent_ = parent; }
  void set_prev_sibling(SparseTreeNode* sibling) {
    previous_sibling_ = sibling;
  }
  void set_next_sibling(SparseTreeNode* sibling) { next_sibling_ = sibling; }
  void set_first_child(SparseTreeNode* child) { first_child_ = child; }
  void set_last_child(SparseTreeNode* child) { last_child_ = child; }
  void set_index(const LargeIndex& index) { index_ = index; }

  bool has_parent() const { return parent_ != NULL; }
  bool has_prev_sibling() const { return previous_sibling_ != NULL; }
  bool has_next_sibling() const { return next_sibling_ != NULL; }
  bool has_child() const { return first_child_ != NULL; }
   
  /// Read the data, including the index, from a stream.
  /**
   * Calling this will override the node's current index.
   * The format is:
   * <ul>
   * <li>(int32_t) index_size</li>
   * <li>(index_size * int32_t) the index</li>
   * </ul>
   * It will then call ReadData().
   * \sa ReadData()
   */
  void ReadFromStream(istream& input_stream);

  /// Write the data, including the index, to a stream.
  /**
   * This will simply write the index to the stream, followed by
   * WriteData().
   * \sa WriteData().
   */
  void WriteToStream(ostream& output_stream) const;

  /// Add the data from <other> to self.
  virtual void Combine(const SparseTreeNode& other) = 0;

  /// Returns true if this node's index is smaller than that of <other>.
  bool operator<(const SparseTreeNode& other) const;

  /// Returns true if <one>'s index is smaller than that of <two>.
  static bool CompareNodes(const SparseTreeNode* one,
                           const SparseTreeNode* two);

protected:
  /// Read the data, excluding the index, from a stream.
  virtual void ReadData(istream& input_stream) = 0;

  /// Write the data, excluding the index, to a stream.
  virtual void WriteData(ostream& output_stream) const = 0;
private:
  LargeIndex index_;
  SparseTreeNode* parent_;
  SparseTreeNode* next_sibling_;
  SparseTreeNode* previous_sibling_;
  SparseTreeNode* first_child_;
  SparseTreeNode* last_child_;
};
}  // namespace libpmk

#endif  // TREE_SPARSE_TREE_NODE_H
