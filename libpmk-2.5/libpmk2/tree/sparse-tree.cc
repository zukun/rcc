// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef TREE_SPARSE_TREE_CC
#define TREE_SPARSE_TREE_CC

#include <stdint.h>
#include <assert.h>
#include <list>
#include <stack>
#include "tree/sparse-tree.h"
#include "tree/sparse-tree-node.h"
#include "util/sparse-vector.h"

namespace libpmk {

// Initialize the static "end" iterators:
template <class T>
typename SparseTree<T>::PreorderIterator SparseTree<T>::end_preorder_ =
typename SparseTree<T>::PreorderIterator(NULL);

template <class T>
typename SparseTree<T>::PostorderIterator SparseTree<T>::end_postorder_ =
typename SparseTree<T>::PostorderIterator(NULL);

template <class T>
typename SparseTree<T>::BreadthFirstIterator
SparseTree<T>::end_breadth_first_ =
typename SparseTree<T>::BreadthFirstIterator(NULL);


template <class T>
SparseTree<T>::SparseTree() : num_nodes_(1) {
  root_node_ = new T(LargeIndex());
}

template <class T>
SparseTree<T>::~SparseTree() {
  FreeAllNodes();
  delete root_node_;
}

template <class T>
SparseTree<T>::SparseTree(const SparseTree<T>& other) {
  root_node_ = new T(*(other.root_node_));
  num_nodes_ = 1;
  CopySubtree(&root_node_, other.root_node_);
}

template <class T>
void SparseTree<T>::CopySubtree(T** my_node, T* other_node) {
  stack<T*> todo;
  stack<T*> todo_copy;

  // Variable naming: the '_copy' suffix indicates that it
  // it is a copy of a node in <other> (i.e., it belongs to
  // this tree).
  todo.push(other_node);
  todo_copy.push(*my_node);

  while (!todo.empty()) {
    T* current = todo.top();
    T* current_copy = todo_copy.top();
    todo.pop();
    todo_copy.pop();

    if (current->has_child()) {
      T* child = (T*)(current->first_child());
      T* first_child_copy = new T(*child);
      ++num_nodes_;

      todo.push(child);
      todo_copy.push(first_child_copy);
      current_copy->set_first_child(first_child_copy);
      first_child_copy->set_parent(current_copy);

      T* previous_child_copy = first_child_copy;
      child = (T*)(child->next_sibling());
      while (child != NULL) {
        T* child_copy = new T(*child);
        ++num_nodes_;

        todo.push(child);
        todo_copy.push(child_copy);
        child_copy->set_parent(current_copy);
        previous_child_copy->set_next_sibling(child_copy);
        child_copy->set_prev_sibling(previous_child_copy);
        previous_child_copy = child_copy;
        child = (T*)(child->next_sibling());
      }
      current_copy->set_last_child(previous_child_copy);
    }
  }
}

template <class T>
SparseTree<T>::SparseTree(const SparseTree<T>& one, const SparseTree<T>& two) {
  root_node_ = new T(*(one.root_node_));
  num_nodes_ = 1;
  CopySubtree(&root_node_, one.root_node_);

  // <todo> keeps a list of things in <two> that need to be copied.
  // <parents> is a parallel stack which keeps track of the parent
  //   *in the current copy*, of the corresponding <todo> node.
  stack<T*> todo;
  stack<T*> parents;
  root_node_->Combine(*(two.root_node_));
  T* child = (T*)(two.root_node_->first_child());
  while (child != NULL) {
    todo.push(child);
    parents.push(root_node_);
    child = (T*)(child->next_sibling());
  }

  while (!todo.empty()) {
    T* two_current = todo.top();
    T* copy_parent = parents.top();
    todo.pop();
    parents.pop();
    T* copy_current = node(two_current->index(), copy_parent);

    // Try to find if a counterpart to two_current exists.
    // If it doesn't exist, add the node, and deep copy the subtree.
    // If it does exist, combine the nodes and add the children to
    //  <todo>.
    if (copy_current == NULL) {
      T* new_child = add_node(*two_current, copy_parent);
      CopySubtree(&new_child, two_current);
    } else {
      copy_current->Combine(*two_current);
      T* two_child = (T*)(two_current->first_child());
      while (two_child != NULL) {
        todo.push(two_child);
        parents.push(copy_current);
        two_child = (T*)(two_child->next_sibling());
      }
    }
  }
}

template <class T>
T* SparseTree<T>::node(const LargeIndex& index) {
  return node(index, root());
}

template <class T>
T* SparseTree<T>::node(const LargeIndex& index, SparseTreeNode* finger) {
  LargeIndex desired_index;
  SparseTreeNode* current_node = finger;
  LargeIndex finger_index = finger->index();

  // finger's index should be a prefix of index.
  if (index.size() < finger_index.size()) {
    return NULL;
  }
  for (int ii = 0; ii < (int)finger_index.size(); ++ii) {
    if (index[ii] != finger_index[ii]) {
      return NULL;
    }
    desired_index.push_back(finger_index[ii]);
  }

  for (int current_level = finger_index.size(); ; current_level++) {
    // This means we're at the node we want
    if (current_level == (int)index.size()) {
      return (T*)current_node;
    }

    // Otherwise, search the next level for the appropriate node,
    // and set current_node to that.
    desired_index.push_back(index[current_level]);

    bool found = false;
    for (SparseTreeNode* next_node = current_node->first_child();
         next_node != NULL; next_node = next_node->next_sibling()) {
      if (next_node->index().back() == desired_index.back()) {
        current_node = next_node;
        found = true;
        break;
      }
    }

    // Quit early if we didn't find that a parent for this index
    // does not exist (and therefore that node does not exist)
    if (!found) {
      return NULL;
    }
  }

  // This should never happen.
  assert(false);
  return NULL;
}

template <class T>
void SparseTree<T>::FreeAllNodes() {
  vector<SparseTreeNode*> todo;
  todo.push_back(root());

  while (!todo.empty()) {
    SparseTreeNode* current = todo.back();
    if (current->has_child()) {
      todo.push_back(current->first_child());
    } else {
      todo.pop_back();
      DeleteLeafNode(current);
    }
  }
  root_node_ = new T(LargeIndex());
  num_nodes_ = 1;
}

template <class T>
void SparseTree<T>::remove_node(SparseTreeNode* node) {
  if (node == root()) {
    FreeAllNodes();
  } else {
    vector<SparseTreeNode*> todo;
    todo.push_back(node);

    while (!todo.empty()) {
      SparseTreeNode* current = todo.back();
      if (current->has_child()) {
        todo.push_back(current->first_child());
      } else {
        todo.pop_back();
        DeleteLeafNode(current);
      }
    }
  }
}

template <class T>
void SparseTree<T>::DeleteLeafNode(SparseTreeNode* node) {
  assert(!node->has_child());

  // If it's the root, just delete and quit.
  if (!node->has_parent()) {
    assert(node == root_node_);
    delete node;
    --num_nodes_;
    assert(num_nodes_ == 0);
    return;
  }

  if (node->parent()->first_child() == node) {
    // If I was the first child, tell my parent to reassign the first
    // child to my next sibling.
    node->parent()->set_first_child(node->next_sibling());
    if (node->has_next_sibling()) {
      node->next_sibling()->set_prev_sibling(NULL);
    }
  }

  if(node->parent()->last_child() == node) {
    // If I was the last child, tell my parent to reassign the last
    // child to my previous sibling.
    node->parent()->set_last_child(node->prev_sibling());
    if (node->has_prev_sibling()) {
      node->prev_sibling()->set_next_sibling(NULL);
    }
  }

  // If I was not the first child or last child, I have to tell my
  // previous sibling that its next sibling is my next sibling,
  // not me. Clearly, since I'm not the first or last child, this
  // means that I have a PreviousSibling and a NextSibling, so I
  // just notify those two nodes that they should be joined.
  if (node->has_prev_sibling()){
    node->prev_sibling()->set_next_sibling(node->next_sibling());
  }
  if (node->has_next_sibling()) {
    node->next_sibling()->set_prev_sibling(node->prev_sibling());
  }


  // I am now a dangling node, ready to be deleted.
  delete node;
  --num_nodes_;
}


// Automatically increments num_bins_.
template <class T>
T* SparseTree<T>::add_node(const T& new_node) {
  // Special case: if new_node is the root node.
  if (new_node.index().size() == 0) {
    root_node_->Combine(new_node);
    return root_node_;
  }

  // Try to find the parent node.
  LargeIndex parent_index(new_node.index());
  parent_index.pop_back();
  T* parent = node(parent_index);
  assert(parent != NULL);
  return add_node(new_node, parent);
}

template<class T>
T* SparseTree<T>::add_node(const T& new_node, SparseTreeNode* parent) {
  LargeIndex new_index = new_node.index();
  new_index.pop_back();
  if (new_index != parent->index()) {
    return NULL;
  }

  // Try to see if a node with this index already exists.
  bool found = false;
  SparseTreeNode* child = parent->first_child();
  while (child != NULL) {
    // For equality testing of indices, we only need to check the last
    // element, because we are guaranteed that the parent node is
    // a prefix of the new bin.
    if (new_node.index().back() == child->index().back()) {
      found = true;
      break;
    }
    child = child->next_sibling();
  }

  if (found) {
    // If such a node exists, Combine it with the new one.
    child->Combine(new_node);
    return (T*)child;
  } else {
    // Otherwise, create a copy of new_node and update the link structure.
    T* added = new T(new_node);
    added->set_parent(parent);

    if (parent->first_child() == NULL) {
      // Case 1: the parent was a leaf
      parent->set_first_child(added);
      parent->set_last_child(added);
      ++num_nodes_;
      return added;
    } else {
      SparseTreeNode* iter = parent->first_child();
      if (iter->index().back() > added->index().back()) {
        // Case 2: added bin is the new first child
        parent->set_first_child(added);
        added->set_next_sibling(iter);
        iter->set_prev_sibling(added);
        ++num_nodes_;
        return added;
      }
      while (iter->has_next_sibling()) {
        // Case 3: added bin is somewhere in the middle
        SparseTreeNode* next = iter->next_sibling();
        if (next->index().back() > added->index().back()) {
          iter->set_next_sibling(added);
          added->set_next_sibling(next);
          added->set_prev_sibling(iter);
          next->set_prev_sibling(added);
          ++num_nodes_;
          return added;
        }
        iter = iter->next_sibling();
      }
      // Case 4: added bin is at the end
      iter->set_next_sibling(added);
      added->set_prev_sibling(iter);
      parent->set_last_child(added);
      ++num_nodes_;
      return added;
    }
  }
}

template <class T>
void SparseTree<T>::WriteToStream(ostream& output_stream) const {
  assert(output_stream.good());
  int32_t num_nodes = size();
  output_stream.write((char *)&num_nodes, sizeof(int32_t));

  PreorderIterator iter = BeginPreorder();
  while (iter != EndPreorder()) {
    iter->WriteToStream(output_stream);
    ++iter;
  }
}


template <class T>
void SparseTree<T>::ReadFromStream(istream& input_stream) {
  assert(input_stream.good());
  FreeAllNodes();

  int32_t num_bins = 0;
  input_stream.read((char *)&num_bins, sizeof(int32_t));
  root_node_->ReadFromStream(input_stream);

  // The first (root) bin always has an empty index.
  assert((int)root_node_->index().size() == 0);

  // (num_bins - 1) since we already read the root bin.
  for (int ii = 0; ii < num_bins - 1; ++ii) {
    // TODO(jjl): The reading and the AddBin() will double copy the
    // data.  We can make this quicker by only copying data once and
    // implementing a function allowing us to directly add a bin
    // rather than add a copy, since we know that the bin won't
    // already exist.
    T temp;
    temp.ReadFromStream(input_stream);
    add_node(temp);
  }
}



template <class T>
typename SparseTree<T>::Iterator&
SparseTree<T>::PreorderIterator::operator++() {
  // If we're not already at the end, try to increment:
  if (Iterator::node_ != NULL) {

    // First, see if there are children to expand.
    // We need to add children in reverse order, so that the
    // first child is at the end of <todo_>.
    if (Iterator::node_->has_child()) {
      SparseTreeNode* child = Iterator::node_->last_child();
      while (child != NULL) {
        todo_.push_back(child);
        child = child->prev_sibling();
      }
    }

    // This is where the actual incrmenting happens; we set <node_> to
    // the next position in the DFS traversal.
    if (!todo_.empty()) {
      Iterator::node_ = todo_.back();
      todo_.pop_back();
    } else {
      Iterator::node_ = NULL;
    }
  }
  return *this;
}

template <class T>
SparseTree<T>::PostorderIterator::PostorderIterator(T* node) : Iterator(node) {
  if (node == NULL) {
    Iterator::node_ = NULL;
  } else {
    todo_.push(node);
    visited_.push(false);
    while (visited_.top() == false) {
      visited_.top() = true;

      SparseTreeNode* to_expand = todo_.top();
      if (to_expand->has_child()) {
        SparseTreeNode* child = to_expand->last_child();
        while (child != NULL) {
          todo_.push(child);
          visited_.push(false);
          child = child->prev_sibling();
        }
      }

    }
    Iterator::node_ = todo_.top();
  }
}

template <class T>
typename SparseTree<T>::Iterator&
SparseTree<T>::PostorderIterator::operator++() {
  // If we're not already at the end, try to increment:
  if (Iterator::node_ != NULL) {
    visited_.pop();
    todo_.pop();

    if (todo_.empty()) {
      Iterator::node_ = NULL;
      return *this;
    }

    // If new top is not visited, visit and expand, repeat.
    while (visited_.top() == false) {
      visited_.top() = true;

      if (todo_.top()->has_child()) {
        SparseTreeNode* child = todo_.top()->last_child();
        while (child != NULL) {
          todo_.push(child);
          visited_.push(false);
          child = child->prev_sibling();
        }
      }
    }

    // if it is visited, do nothing (it's the new node_)
    Iterator::node_ = todo_.top();
  }
  return *this;
}

template <class T>
typename SparseTree<T>::Iterator&
SparseTree<T>::BreadthFirstIterator::operator++() {
  if (Iterator::node_ != NULL) {

    if (Iterator::node_->has_child()) {
      SparseTreeNode* child = Iterator::node_->first_child();
      while (child != NULL) {
        todo_.push_back(child);
        child = child->next_sibling();
      }
    }

    if (!todo_.empty()) {
      Iterator::node_ = todo_.front();

      // We need to add children in reverse order, so that the first
      // child is at the end of todo_.
      todo_.pop_front();
    } else {
      Iterator::node_ = NULL;
    }
  }
  return *this;
}

template <class T>
typename SparseTree<T>::PreorderIterator SparseTree<T>::BeginPreorder() const {
  return PreorderIterator(root_node_);
}

template <class T>
const typename SparseTree<T>::PreorderIterator& SparseTree<T>::EndPreorder() {
  return end_preorder_;
}

template <class T>
typename SparseTree<T>::PostorderIterator
SparseTree<T>::BeginPostorder() const {
  return PostorderIterator(root_node_);
}

template <class T>
const typename SparseTree<T>::PostorderIterator&
SparseTree<T>::EndPostorder() {
  return end_postorder_;
}

template <class T>
typename SparseTree<T>::BreadthFirstIterator
SparseTree<T>::BeginBreadthFirst() const {
  return BreadthFirstIterator(root_node_);
}

template <class T>
const typename SparseTree<T>::BreadthFirstIterator&
SparseTree<T>::EndBreadthFirst() {
  return end_breadth_first_;
}

}  // namespace libpmk

#endif  // TREE_SPARSE_TREE_CC
