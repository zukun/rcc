// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef TREE_TREE_NODE_H
#define TREE_TREE_NODE_H

#include <iostream>
#include <vector>

using namespace std;

namespace libpmk {

/// An indexed node, used by Tree.
/**
 * A TreeNode has an integer identifier. It also contains the IDs of
 * its parent and children, but makes no effort to maintain them.
 *
 * To augment a TreeNode with your own data, there are three functions
 * you need to provide: (1) reading the data from a stream, (2)
 * writing the data to a stream, and (3) how to combine two nodes. The
 * definition of "combine" will vary depending on what you're doing,
 * but the general idea is that sometimes you will have two TreeNodes
 * with the same ID that you want to insert into a tree.  It is
 * advisable that you make the output of Combine() symmetric, since
 * there are no guarantees on what Tree does with duplicate-ID bins
 * other than calling Combine().
 *
 * Remember that if you implement a copy constructor for your TreeNode,
 * you should have your copy constructor call the base class's copy
 * constructor: <br>
 * <tt>
 * MyNode::MyNode(const MyNode& other) <b>: TreeNode(other)</b> {<br>
 * // your copy code<br>
 * }
 * </tt>
 */
class TreeNode {
public:
  static const int kInvalidNodeID = -1;

  TreeNode();
  TreeNode(int node_id);

  /// A copy constructor, which copies the ID and parent/child info.
  TreeNode(const TreeNode& other);

  virtual ~TreeNode() { }

  int id() const { return id_; }
  int parent() const { return parent_; }
  int child(int child_index) const { return children_.at(child_index); }

  int child_size() const { return (int)children_.size(); }
  bool has_parent() const { return parent_ != kInvalidNodeID; }
  bool has_child() const { return child_size() > 0; }

  void set_id(int id) { id_ = id; }
  void set_parent(int id) { parent_ = id; }

  /// Adds a child to the end of the child list.
  /**
   * This does not check for dupes, i.e., it is possible for a node to
   * have two children with the same ID using this, so be careful!
   */
  void add_child(int id) { children_.push_back(id); }

  /// Removes the <child_index>th child.
  void remove_child(int child_index);


  /// Read the data, including the index, from a stream.
  /**
   * Calling this will override the node's current index.
   * The format is:
   * <ul>
   * <li>(int32_t) id</li>
   * <li>(int32_t) parent_id</li>
   * <li>(int32_t) num_children</li>
   * <li>(num_children * int32_t) child IDs</li>
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
  virtual void Combine(const TreeNode& other) = 0;

protected:
  /// Read the data, excluding the index, from a stream.
  virtual void ReadData(istream& input_stream) = 0;

  /// Write the data, excluding the index, to a stream.
  virtual void WriteData(ostream& output_stream) const = 0;

private:
  int id_;
  int parent_;
  vector<int> children_;
};
}  // namespace libpmk

#endif  // TREE_TREE_NODE_H
