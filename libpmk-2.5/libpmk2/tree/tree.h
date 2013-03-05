// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef TREE_TREE_H
#define TREE_TREE_H

#include <list>
#include <stack>
#include <ext/hash_map>
#include "tree/tree-node.h"

using namespace std;
using namespace __gnu_cxx;

namespace libpmk {

// TODO: clarify behavior when tree is empty.
// Tree always has a root with ID 0.

/// \brief A data structure for representing trees containing
/// TreeNode objects.
/**
 *
 * This implementation stores nodes indexed by ID (a single
 * integer). The root will always have ID 0, but the IDs of other
 * nodes are not in any particular order. The tree structure is
 * obtained by asking any particular node what the IDs of its parent
 * or children are.
 *
 * Access to the tree is simple: give the Tree a node ID and it will
 * return a pointer to the node with that ID.
 *
 * <b>** If you have build problems:</b> remember that since Tree is a
 * class template, when using your own TreeNodes in a Tree, you'll run
 * into linker errors if you don't do an explicit instantiation of
 * Tree<YourTreeNode>. To deal with this, you will need to do two things:
 * <ul>
 * <li> #include "tree/tree.cc"  (yes, tree.cc, not tree.h)
 * <li> Add the line "template class Tree<YourTreeNode>;" in your code.
 * </ul>
 *
 * If you are still having problems:
 * <ul>
 * <li> Make sure to implement two required <tt>TreeNode</tt>
 * constructors: the default one, which takes no arguments, and the
 * one which takes an int (ID).
 * <li> If you are getting segmentation faults, it could be that you
 * are trying to copy trees. Make sure that if you are doing so, and
 * you have implemented a copy constructor for your Node class, be sure
 * that it calls TreeNode's copy constructor during initialization. For
 * example:<br><tt>
 * MyNode::MyNode(const MyNode& other) <b>: TreeNode(other)</b> {<br>
 * // your copy code<br>
 * }
 * </tt>
 * <li> Remember that <tt>WriteData</tt> is a <tt>const</tt> function.
 * </ul>
 * \sa TreeNode
 */
template <class T>
class Tree {
public:
  Tree();
  ~Tree();

  /// \brief Copy constructor. It will do a deep copy of all of the
  /// TreeNodes.
  Tree(const Tree<T>& other);

  class Iterator;
  class PreorderIterator;
  class PostorderIterator;
  class BreadthFirstIterator;

  /// Return the tree to a state as if it were just constructed by Tree().
  void clear();

  /// Get a pointer to the root node.
  T* root() { return tree_[0]; }
  const T* root() const { return node(0); }

  /// Get the total number of nodes in the tree.
  int size() const { return tree_.size(); }

  /// Get a node specified by the given index.
  /**
   * Returns NULL if the node is not found.
   */
  T* node(int id);
  const T* node(int id) const;

  /// Insert a copy of the given node into the tree.
  /**
   * Returns a pointer to the newly-added node in the tree.
   *
   * If the new node's ID is uninitialized or invalid, it will be
   * assigned a new unique ID. The returned pointer is a pointer to
   * the newly-added node, so the user should call set_parent() and
   * so forth in order to link the new node to the tree.
   *
   * If the new node's ID is valid, but the tree does not contain a
   * node with that ID, a new node will be created. The user still
   * needs to link it to the tree manually via set_parent() and
   * set_child().
   *
   * If there already is a node in the tree with the same ID as
   * new_node.id() (call it N), this will call N.Combine(new_node).
   * In this case, the already-existing tree structure will be used
   * (no need to call set_parent() and so forth, since the node is
   * already there).
   *
   * If you are having problems with this, i.e., you are calling
   * add_node() but the data seems to not be copied into the tree,
   * make sure that you have provided a copy constructor for your
   * TreeNode.
   */
  T* add_node(const T& new_node);

  /// Insert a blank node into the tree.
  /**
   * Returns a pointer to the newly added node. The resulting
   * node will have no parent/child pointers, so it is not
   * actually linked to the tree. It is up to the caller to
   * link it to the tree by calling TreeNode::add_child() and
   * TreeNode::set_parent().
   */
  T* add_node();


  /// Reads a tree from the stream.
  /**
   * File format: <ul>
   * <li> (int32) The total number of nodes, including the root
   * <li> (num_nodes * TreeNode) The nodes, in no particular order.
   *
   * Aborts if the stream is bad. Recall that the root of the tree
   * always has ID 0.
   */
  void ReadFromStream(istream& input_stream);

  /// Write the tree to a stream.
  /**
   * Aborts if the stream is bad. See ReadFromStream
   * for the format. \sa ReadFromStream
   */
  void WriteToStream(ostream& output_stream) const;


  /// Frees a node, and all of its children, from the tree.
  void DeleteNode(int node_id);

  /// Start of a preorder depth-first traversal.
  PreorderIterator BeginPreorder() const;

  /// End of a preorder depth-first traversal.
  const PreorderIterator& EndPreorder() const;

  /// Start of a postorder depth-first traversal.
  PostorderIterator BeginPostorder() const;

  /// End of a postorder depth-first traversal.
  const PostorderIterator& EndPostorder() const;

  /// Start of a breadth-first traversal.
  BreadthFirstIterator BeginBreadthFirst() const;

  /// End of a breadth-first traversal.
  const BreadthFirstIterator& EndBreadthFirst() const;

  /// An iterator for Trees.
  class Iterator {
  public:
    /// Creates a new iterator pointing at the given ID.
    Iterator(int node_id, const Tree<T>* source_tree) :
      node_id_(node_id), source_tree_(source_tree) { }

    virtual ~Iterator() { }

    /// Copy assignment operator
    Iterator& operator=(const Iterator& other) {
      node_id_ = other.node_id_;
      return *this;
    }

    /// Returns true iff <other> points to the same node.
    bool operator==(const Iterator& other) {
      return node_id_ == other.node_id_;
    }

    /// Returns true iff <other> points to a different node.
    bool operator!=(const Iterator& other) {
      return node_id_ != other.node_id_;
    }

    int operator*() {
      return node_id_;
    }

    int id() {
      return node_id_;
    }

    virtual Iterator& operator++() = 0;

  protected:
    int node_id_;
    const Tree<T>* source_tree_;
  };


  // Internally, we let <node_id_> be the current position. <todo_>
  // contains the IDs of all remaining nodes, but the nodes in <todo_>
  // may have yet to be expanded.
  /// Preorder depth-first iterator for Trees.
  class PreorderIterator : public Iterator {
  public:
    /// \brief Creates a new iterator rooted at <node_id>. The
    /// resulting traversal will ignore any parents of <node_id>.
    PreorderIterator(int node_id, const Tree<T>* tree) :
      Iterator(node_id, tree) { }
    virtual ~PreorderIterator() {}
    virtual Iterator& operator++();

  private:
    list<int> todo_;
  };

  class PostorderIterator : public Iterator {
  public:
    /// \brief Creates a new iterator rooted at <node_id>. The
    /// resulting traversal will ignore any parents of <node_id>.
    PostorderIterator(int node_id, const Tree<T>* tree);
    virtual ~PostorderIterator() {}
    virtual Iterator& operator++();

  private:
    stack<int> todo_;
    stack<bool> visited_;
  };

  /// Breadth-first iterator for Trees.
  class BreadthFirstIterator : public Iterator {
  public:
    /// Creates a new iterator rooted at <node_id>. The resulting
    /// traversal will ignore any parents of <node_id>.
    BreadthFirstIterator(int node_id, const Tree<T>* tree) :
      Iterator(node_id, tree) {}
    virtual ~BreadthFirstIterator() {}
    virtual Iterator& operator++();
  private:
    list<int> todo_;
  };

private:
  hash_map<int, T*> tree_;
  int last_used_id_;

  // Block the copy assignment operator
  void operator=(const Tree<T>&);

  // Pre-create some end-iterators so we don't have to always create
  // them. This is useful for loops like:
  //   while (iter != tree.EndPreorder())
  // because calling this iterators generally involves instantiating
  // an STL stack, vector, or list.
  const PreorderIterator end_preorder_;
  const PostorderIterator end_postorder_;
  const BreadthFirstIterator end_breadth_first_;
};
}  // namespace libpmk

#endif  // TREE_TREE_H
