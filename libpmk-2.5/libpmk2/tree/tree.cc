// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef TREE_TREE_CC
#define TREE_TREE_CC

#include <assert.h>
#include <list>
#include <stack>
#include <ext/hash_map>
#include "tree/tree.h"
#include "tree/tree-node.h"
#include "util/sparse-vector.h"

using namespace __gnu_cxx;

namespace libpmk {

template <class T> Tree<T>::Tree() :
  last_used_id_(0),
  end_preorder_(PreorderIterator(TreeNode::kInvalidNodeID, this)),
  end_postorder_(PostorderIterator(TreeNode::kInvalidNodeID, this)),
  end_breadth_first_(BreadthFirstIterator(TreeNode::kInvalidNodeID, this)) {
  tree_.clear();
  tree_[0] = new T(0);
}

template <class T>
Tree<T>::~Tree() {
  clear();

  // clear will actually create a new root, so delete that
  // here.
  delete tree_[0];
  tree_.clear();
}

template <class T>
Tree<T>::Tree(const Tree<T>& other) :
  end_preorder_(PreorderIterator(TreeNode::kInvalidNodeID, this)),
  end_postorder_(PostorderIterator(TreeNode::kInvalidNodeID, this)),
  end_breadth_first_(BreadthFirstIterator(TreeNode::kInvalidNodeID, this)) {
  tree_.clear();
  for (typename hash_map<int, T*>::const_iterator iter = other.tree_.begin();
       iter != other.tree_.end(); ++iter) {
    tree_[iter->first] = new T(*(iter->second));
  }

  last_used_id_ = other.last_used_id_;
}

template <class T>
T* Tree<T>::node(int id) {
  typename hash_map<int, T*>::iterator iter = tree_.find(id);
  if (iter == tree_.end()) {
    return NULL;
  }

  return iter->second;
}

template <class T>
const T* Tree<T>::node(int id) const {
  typename hash_map<int, T*>::const_iterator iter = tree_.find(id);
  if (iter == tree_.end()) {
    return NULL;
  }

  return iter->second;
}

template <class T>
void Tree<T>::clear() {
  for (typename hash_map<int, T*>::iterator iter = tree_.begin();
       iter != tree_.end(); ++iter) {
    delete iter->second;
  }
  tree_.clear();
  last_used_id_ = 0;
  tree_[0] = new T(0);
}

template <class T>
void Tree<T>::DeleteNode(int node_id) {
  if (node_id == 0) {
    clear();
  } else {
    // Tell the node's parent that it lost a child. This will
    // unlink the node from the tree.
    TreeNode* node = this->node(node_id);
    assert(node->has_parent());

    bool found_child = false;
    int parent_id = node->parent();
    TreeNode* parent = this->node(parent_id);
    for (int ii = 0; ii < parent->child_size(); ++ii) {
      if (parent->child(ii) == node_id) {
        found_child = true;
        parent->remove_child(ii);
        break;
      }
    }
    assert(found_child);

    // Traverse this severed subtree, collect all of the IDs to
    // delete, and delete them all at once.
    list<int> todo;
    todo.push_back(node_id);

    list<int> ids_to_delete;
    while (!todo.empty()) {
      TreeNode* current = this->node(todo.back());
      ids_to_delete.push_back(todo.back());
      todo.pop_back();

      if (current->has_child()) {
        for (int ii = 0; ii < current->child_size(); ++ii) {
          todo.push_back(current->child(ii));
        }
      }
    }

    // Delete all of the selected nodes.
    for (list<int>::iterator iter = ids_to_delete.begin();
         iter != ids_to_delete.end(); ++iter) {
      typename hash_map<int, T*>::iterator finger = tree_.find(*iter);
      delete finger->second;
      tree_.erase(finger);
    }
  }
}

template<class T>
T* Tree<T>::add_node() {
  ++last_used_id_;
  T* node = new T();
  node->set_id(last_used_id_);
  assert(tree_.find(last_used_id_) == tree_.end());
  tree_[last_used_id_] = node;
  return node;
}

template <class T>
T* Tree<T>::add_node(const T& new_node) {
  // If the new node has an uninitialized ID, we'll create a new one
  // for it. Remember that TreeNode's copy constructor also copies
  // over parent and child IDs.
  if (new_node.id() == TreeNode::kInvalidNodeID) {
    ++last_used_id_;
    T* node_copy = new T(new_node);
    node_copy->set_id(last_used_id_);
    assert(tree_.find(last_used_id_) == tree_.end());
    tree_[last_used_id_] = node_copy;
    return node_copy;
  } else {
    // If the node has an ID we have never seen before, insert it into
    // the tree. Recall that it will not be linked to anything, so
    // manual intervention is requried. This is generally dangerous to
    // do; it will usually be better to let Tree assign IDs. This is
    // usually only used by Tree::ReadFromStream() which assumes that
    // the structure is already present in the tree.
    typename hash_map<int, T*>::iterator iter = tree_.find(new_node.id());
    if (iter == tree_.end()) {
      // Update the last_used_id_ to make sure that it always reflects
      // the highest number.
      if (new_node.id() > last_used_id_) {
        last_used_id_ = new_node.id();
      }

      T* node_copy = new T(new_node);
      tree_[new_node.id()] = node_copy;
      return node_copy;
    } else {
      // Otherwise, if there already is a node in this tree with this
      // ID, then just combine this node with that one.
      (iter->second)->Combine(new_node);
      return iter->second;
    }
  }
}


template <class T>
void Tree<T>::WriteToStream(ostream& output_stream) const {
  assert(output_stream.good());
  int32_t num_nodes = size();
  output_stream.write((char *)&num_nodes, sizeof(int32_t));

  for (typename hash_map<int, T*>::const_iterator iter = tree_.begin();
       iter != tree_.end(); ++iter) {
    (iter->second)->WriteToStream(output_stream);
  }
}


template <class T>
void Tree<T>::ReadFromStream(istream& input_stream) {
  assert(input_stream.good());
  clear();

  int32_t size = 0;
  input_stream.read((char *)&size, sizeof(int32_t));

  for (int ii = 0; ii < size; ++ii) {
    T temp;
    temp.ReadFromStream(input_stream);
    // The special case is the root. An empty Tree, in this
    // representation, has a root with ID 0. When we read the root
    // from the stream (it will have ID 0), we shouldn't be combining
    // that root with the existing one, which is what add_node would
    // do; we should be replacing it.
    if (temp.id() == 0) {
      assert(tree_[0] != NULL);
      delete tree_[0];
      tree_[0] = new T(temp);
    } else {
      add_node(temp);
    }
  }
}



template <class T>
typename Tree<T>::Iterator& Tree<T>::PreorderIterator::operator++() {
  // If we're not already at the end, try to increment:
  if (Iterator::node_id_ != TreeNode::kInvalidNodeID) {

    // First, see if there are children to expand.
    // We need to add children in reverse order, so that the
    // first child is at the end of <todo_>.
    const TreeNode* node = Iterator::source_tree_->node(Iterator::node_id_);
    if (node->has_child()) {
      for (int ii = node->child_size() - 1; ii >= 0; --ii) {
        todo_.push_back(node->child(ii));
      }
    }

    // This is where the actual incrmenting happens; we set <node_id_>
    // to the next position in the DFS traversal.
    if (!todo_.empty()) {
      Iterator::node_id_ = todo_.back();
      todo_.pop_back();
    } else {
      Iterator::node_id_ = TreeNode::kInvalidNodeID;
    }
  }
  return *this;
}

template <class T>
Tree<T>::PostorderIterator::PostorderIterator(int node_id, const Tree<T>* tree)
  : Iterator(node_id, tree) {
  if (node_id == TreeNode::kInvalidNodeID) {
    Iterator::node_id_ = TreeNode::kInvalidNodeID;
  } else {
    todo_.push(node_id);
    visited_.push(false);
    while (visited_.top() == false) {
      visited_.top() = true;

      int to_expand = todo_.top();
      const TreeNode* node_to_expand = Iterator::source_tree_->node(to_expand);
      if (node_to_expand->has_child()) {
        // Add children in reverse order, so that the first child
        // is at the end of todo_.
        for (int ii = node_to_expand->child_size() - 1; ii >= 0; --ii) {
          todo_.push(node_to_expand->child(ii));
          visited_.push(false);
        }
      }
    }
    Iterator::node_id_ = todo_.top();
  }
}

template <class T>
typename Tree<T>::Iterator& Tree<T>::PostorderIterator::operator++() {
  // If we're not already at the end, try to increment:
  if (Iterator::node_id_ != TreeNode::kInvalidNodeID) {
    visited_.pop();
    todo_.pop();

    if (todo_.empty()) {
      Iterator::node_id_ = TreeNode::kInvalidNodeID;
      return *this;
    }

    // If new top is not visited, visit and expand, repeat.
    while (visited_.top() == false) {
      visited_.top() = true;

      const TreeNode* next_node = Iterator::source_tree_->node(todo_.top());
      if (next_node->has_child()) {
        for (int ii = next_node->child_size() - 1; ii >= 0; --ii) {
          todo_.push(next_node->child(ii));
          visited_.push(false);
        }
      }
    }

    // if it is visited, do nothing (it's the new node_)
    Iterator::node_id_ = todo_.top();
  }
  return *this;
}

template <class T>
typename Tree<T>::Iterator& Tree<T>::BreadthFirstIterator::operator++() {
  if (Iterator::node_id_ != TreeNode::kInvalidNodeID) {

    const TreeNode* node = Iterator::source_tree_->node(Iterator::node_id_);
    if (node->has_child()) {
      for (int ii = 0; ii < node->child_size(); ++ii) {
        todo_.push_back(node->child(ii));
      }
    }

    if (!todo_.empty()) {
      Iterator::node_id_ = todo_.front();
      todo_.pop_front();
    } else {
      Iterator::node_id_ = TreeNode::kInvalidNodeID;
    }
  }
  return *this;
}

template <class T>
typename Tree<T>::PreorderIterator Tree<T>::BeginPreorder() const {
  return PreorderIterator(0, this);
}

template <class T>
const typename Tree<T>::PreorderIterator& Tree<T>::EndPreorder() const {
  return end_preorder_;
}

template <class T>
typename Tree<T>::PostorderIterator Tree<T>::BeginPostorder() const {
  return PostorderIterator(0, this);
}

template <class T>
const typename Tree<T>::PostorderIterator& Tree<T>::EndPostorder() const {
  return end_postorder_;
}

template <class T>
typename Tree<T>::BreadthFirstIterator Tree<T>::BeginBreadthFirst() const {
  return BreadthFirstIterator(0, this);
}

template <class T>
const typename Tree<T>::BreadthFirstIterator&
Tree<T>::EndBreadthFirst() const {
  return end_breadth_first_;
}

}  // namespace libpmk

#endif  // TREE_TREE_CC
