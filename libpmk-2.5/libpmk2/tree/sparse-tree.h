// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef TREE_SPARSE_TREE_H
#define TREE_SPARSE_TREE_H

#include <list>
#include <stack>
#include "tree/sparse-tree-node.h"

using namespace std;

namespace libpmk {

/// \brief A data structure for sparsely representing trees containing
/// SparseTreeNode objects.
/**
 * This implementation allows one to quickly get the parent, child, or
 * siblings of any node. All data put into a Tree must be a subclass
 * of SparseTreeNode. SparseTreeNode will automatically manage all of
 * the parent/child/sibling pointers as well as the LargeIndexes for
 * you.  For more information on how to implement a SparseTreeNode,
 * see that page.
 *
 * There are two ways to access nodes in these data structures: either
 * by looking at the root node and traversing the tree manually by
 * using the parent/child/sibling relationships, or by specifying a
 * LargeIndex describing a path down the tree. These indices are set
 * up and maintained such that at the nth level of the tree, the
 * LargeIndex has size n (the root node has size 0 which is an empty
 * LargeIndex).  The ith element of the LargeIndex specifies the link
 * traversed to get from the (i-1)st level to the ith level.
 *
 * The sibling pointers are managed such that GetNextSibling() always
 * returns a Bin with the next highest index in that branch of the
 * tree. This multi-resolution histogram is stored sparsely: if you
 * consider the Bins referred to by [0 1] and [0 3], it is possible
 * that [0 3] is the sibling after [0 1] (as long as there is no such
 * bin [0 2]).
 *
 * Note that using LargeIndex <i>and</i> the link structure between
 * Bins is redundant, as a LargeIndex specifies a path down the
 * tree. When reading and writing the SparseTree to disk, we only
 * persist the LargeIndexes-- the link structure is inferred from the
 * LargeIndex values upon reading from disk. The purpose of the link
 * structure is to speed up computations and lookups.
 *
 * <b>** If you have build problems:</b> remember that since
 * SparseTree is a class template, when using your own SparseTreeNodes
 * in a SparseTree, you'll run into linker errors if you don't do an
 * explicit instantiation of SparseTree<YourSparseTreeNode>. To deal
 * with this, you will need to do two things:
 * <ul>
 * <li> #include "tree/sparse-tree.cc" (yes, sparse-tree.cc, not sparse-tree.h)
 * <li> Add the line "template class Tree<YourTreeNode>;" in your code.
 * </ul>
 *
 * If you are still having problems:
 * <ul>
 * <li> Make sure to implement two required <tt>SparseTreeNode</tt>
 * constructors: the default one, which takes no arguments, and the
 * one which takes a <tt>const LargeIndex&</tt>.
 * <li> If you are getting segmentation faults, it could be that you
 * are trying to copy trees. Make sure that if you are doing so, and
 * you have implemented a copy constructor for your Node class, be sure
 * that it calls TreeNode's copy constructor during initialization. For
 * example:<br><tt>
 * MyNode::MyNode(const MyNode& other) <b>: SparseTreeNode(other)</b> {<br>
 * // your copy code<br>
 * }
 * </tt>
 * <li> Remember that <tt>WriteData</tt> is a <tt>const</tt> function.
 * </ul>
 * \sa SparseTreeNode
 */
template <class T>
class SparseTree {
public:
  SparseTree();
  ~SparseTree();

  /// Copy constructor.
  SparseTree(const SparseTree<T>& other);
  SparseTree(const SparseTree<T>& one, const SparseTree<T>& two);

  class Iterator;
  class PreorderIterator;
  class PostorderIterator;
  class BreadthFirstIterator;

  /// Get a pointer to the root node.
  T* root() { return root_node_; }

  /// Get a pointer to the root node (const version).
  T* const root() const { return root_node_; }

  /// Get the total number of nodes in the tree.
  int size() const { return num_nodes_; }

  /// Get a node specified by the given index.
  /**
   * Returns NULL if the node is not found.
   */
  T* node(const LargeIndex& index);

  /// Get a pointer to the node with the specified index.
  /**
   * Same as GetNode(LargeIndex), but localizes the search to the
   * subtree given by <finger>. Returns NULL if there is no such node
   * in a subtree of <finger>.
   */
  T* node(const LargeIndex& index, SparseTreeNode* finger);



  /// Insert a copy of the given node into the tree.
  /**
   * Returns a pointer to the newly-added node in the tree. This
   * function completely ignores any parent/child/sibling pointers in
   * <new_node>.
   *
   * This function requires the parent bin to already exist in the
   * tree. It will NOT create new bins along the way (it will abort
   * if there is no parent node, i.e., a node whose index is a prefix
   * of that of <new_node> and whose index size is exactly 1 less than
   * the new node's index size. The insertion happens such that the
   * sibling relationships remain sorted by index.
   *
   * If there is already is a node with the given index (call it N),
   * we call N.Combine(new_node), and no new node is inserted into
   * the tree. This applies to the root node as well.
   *
   */
  T* add_node(const T& new_node);

  /// Insert a copy of the given bin into the tree.
  /**
   * Same as AddBin(const Node&), except it starts the search for the
   * bin at <parent>. In this case, <parent> must be the direct
   * parent of the node we are inserting (it cannot be a higher-up
   * ancestor).
   */
  T* add_node(const T& new_node, SparseTreeNode* parent);

  /// Reads a tree from the stream.
  /**
   * File format: <ul>
   * <li> (int32) The total number of bins, including the root
   * <li> For each bin: <ul>
   *     <li> (int32) L, the size of its index (0 for the root, etc.)
   *     <li> (L * int32) the index
   *     <li> (sizeof(data)) the data, from T.WriteToStream().
   *
   * The ordering of the bins is a depth-first traversal of the tree.
   *
   * Aborts if the stream is bad.
   */
  void ReadFromStream(istream& input_stream);

  /// Write the tree to a stream.
  /**
   * Aborts if the stream is bad. See ReadFromStream
   * for the format. \sa ReadFromStream
   */
  void WriteToStream(ostream& output_stream) const;


  /// Removes a node, and all of its children, from the tree.
  /**
   * The node must be owned by this tree. SparseTree will not check for
   * that.
   */
  void remove_node(SparseTreeNode* node);

  /// Start of a preorder depth-first traversal.
  PreorderIterator BeginPreorder() const;

  /// End of a preorder depth-first traversal.
  static const PreorderIterator& EndPreorder();

  /// Start of a postorder depth-first traversal.
  PostorderIterator BeginPostorder() const;

  /// End of a postorder depth-first traversal.
  static const PostorderIterator& EndPostorder();

  /// Start of a breadth-first traversal.
  BreadthFirstIterator BeginBreadthFirst() const;

  /// End of a breadth-first traversal.
  static const BreadthFirstIterator& EndBreadthFirst();

  /// An iterator for SparseTrees.
  class Iterator {
  public:
    /// Creates a new iterator pointing at the given node.
    Iterator(SparseTreeNode* node) : node_(node) {}
    virtual ~Iterator() {}

    /// Copy assignment operator
    Iterator& operator=(const Iterator& other) {
      node_ = other.node_;
      return *this;
    }

    /// Returns true iff <other> points to the same node in memory.
    bool operator==(const Iterator& other) {
      return node_ == other.node_;
    }

    /// Returns true iff <other> points to a different node in memory.
    bool operator!=(const Iterator& other) {
      return node_ != other.node_;
    }

    /// Accesses the pointer to the SparseTreeNode.
    T* operator->() {
      return static_cast<T*>(node_);
    }

    /// Returns a pointer to the SparseTreeNode.
    T* get() {
      return static_cast<T*>(node_);
    }
    virtual Iterator& operator++() = 0;

  protected:
    SparseTreeNode* node_;
  };


  // Internally, we let <node_> be the current position. <todo_>
  // contains all remaining nodes, but the nodes in <todo_> may
  // have yet to be expanded.
  /// Preorder depth-first iterator for SparseTrees.
  class PreorderIterator : public Iterator {
  public:
    /// Creates a new iterator rooted at <node>. The resulting
    /// traversal will ignore any parents of <node>.
    PreorderIterator(T* node) : Iterator(node) {}
    virtual ~PreorderIterator() {}
    virtual Iterator& operator++();

  private:
    list<SparseTreeNode*> todo_;
  };


  class PostorderIterator : public Iterator {
  public:
    /// Creates a new iterator rooted at <node>. The resulting
    /// traversal will ignore any parents of <node>.
    PostorderIterator(T* node);
    virtual ~PostorderIterator() {}
    virtual Iterator& operator++();

  private:
    stack<SparseTreeNode*> todo_;
    stack<bool> visited_;
  };

  /// Breadth-first iterator for SparseTrees.
  class BreadthFirstIterator : public Iterator {
  public:
    /// Creates a new iterator rooted at <node>. The resulting
    /// traversal will ignore any parents of <node>.
    BreadthFirstIterator(T* node) : Iterator(node) {}
    virtual ~BreadthFirstIterator() {}
    virtual Iterator& operator++();
  private:
    list<SparseTreeNode*> todo_;
  };

private:
  T* root_node_;
  void DeleteLeafNode(SparseTreeNode* node);
  void FreeAllNodes();

  // Assumes my_node has no children (will not free any memory).
  // Makes my_node's children into a deep copy of other_node (copies
  // children and link structure as well). This will increment the
  // node count for each new bin added as well. It will NOT modify
  // my_node however.
  void CopySubtree(T** my_node, T* other_node);
  int num_nodes_;

  // Block the copy assignment operator
  void operator=(const SparseTree<T>&);

  // Static variable hacks so we don't have to always create end-iterators.
  static PreorderIterator end_preorder_;
  static PostorderIterator end_postorder_;
  static BreadthFirstIterator end_breadth_first_;
};
}  // namespace libpmk
#endif  // TREE_SPARSE_TREE_H
