// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef TREE_POINT_TREE_NODE_H
#define TREE_POINT_TREE_NODE_H

#include <iostream>
#include <memory>
#include "tree/tree-node.h"
#include "point_set/point.h"

using namespace std;

namespace libpmk {

/// A TreeNode containing a Point.
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
class PointTreeNode : public TreeNode {
public:
  PointTreeNode();
  PointTreeNode(int node_id);

  /// A copy constructor, which copies the ID and parent/child info.
  PointTreeNode(const PointTreeNode& other);

  const Point& point() const { return *(point_.get()); }
  Point* mutable_point() { return point_.get(); }
  void set_point(const Point& point) { point_.reset(new Point(point)); }
  bool has_point() const { return point_.get() != NULL;  }

  virtual ~PointTreeNode() { }

  /// \brief This should not happen for PointTreeNodes, so this method
  /// will always fail.
  virtual void Combine(const TreeNode& other);

protected:
  /// Read the data, excluding the index, from a stream.
  virtual void ReadData(istream& input_stream);

  /// Write the data, excluding the index, to a stream.
  virtual void WriteData(ostream& output_stream) const;

private:
  auto_ptr<Point> point_;
};
}  // namespace libpmk

#endif  // TREE_TREE_NODE_H
