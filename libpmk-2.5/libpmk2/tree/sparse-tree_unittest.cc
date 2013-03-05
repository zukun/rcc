// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <sstream>
#include "tree/sparse-tree-node.h"
#include "tree/sparse-tree.cc"

using namespace libpmk;

class MyNode : public SparseTreeNode {
public:
  MyNode() : SparseTreeNode() {}
  MyNode(const LargeIndex& index) : SparseTreeNode(index), data_(0) {}
  MyNode(int data, const LargeIndex& index) :
    SparseTreeNode(index), data_(data) {}
  virtual ~MyNode() { }
  virtual void Combine(const SparseTreeNode& other) {
    data_ += (static_cast<const MyNode&>(other)).data_;
  }
  int data() const {
    return data_;
  }

protected:
  virtual void ReadData(istream& input_stream) {
    input_stream.read((char *)&data_, sizeof(int));
  }

  virtual void WriteData(ostream& output_stream) const {
    output_stream.write((char *)&data_, sizeof(int));
  }

private:
  int data_;
};

template class SparseTree<MyNode>;

void TestReadAndWrite(SparseTree<MyNode>& tree) {
  ostringstream out_stream(ios::binary);
  tree.WriteToStream(out_stream);
  string s = out_stream.str();

  SparseTree<MyNode> tree2;
  istringstream in_stream(s, ios::binary);
  tree2.ReadFromStream(in_stream);

  SparseTree<MyNode>::PreorderIterator iter1 = tree.BeginPreorder();
  SparseTree<MyNode>::PreorderIterator iter2 = tree2.BeginPreorder();

  while (iter1 != tree.EndPreorder()) {
    assert(iter1->data() == iter2->data());
    ++iter1;
    ++iter2;
  }
}

void TestCopy(SparseTree<MyNode>& original) {
  // Serialize it first and read it back, to make sure we get a true
  // copy and to make sure that <tree> isn't modified.
  ostringstream out_stream(ios::binary);
  original.WriteToStream(out_stream);
  string s = out_stream.str();

  SparseTree<MyNode> serialized;
  istringstream in_stream(s, ios::binary);
  serialized.ReadFromStream(in_stream);


  SparseTree<MyNode> copied(original);
  SparseTree<MyNode>::PreorderIterator iter1 = original.BeginPreorder();
  SparseTree<MyNode>::PreorderIterator iter2 = serialized.BeginPreorder();
  SparseTree<MyNode>::PreorderIterator iter3 = copied.BeginPreorder();

  while (iter1 != original.EndPreorder()) {
    // Make sure the data is all the same
    assert(iter1->data() == iter2->data());
    assert(iter2->data() == iter3->data());
    assert(iter1->index() == iter2->index());
    assert(iter2->index() == iter3->index());

    // But also make sure that the pointers themselves are different,
    // i.e., they are distinct in memory.
    assert(iter1.get() != iter2.get());
    assert(iter2.get() != iter3.get());
    ++iter1;
    ++iter2;
    ++iter3;
  }
}

// Checks to see that two trees have the same content but different pointers.
void TestEquivalent(SparseTree<MyNode>& tree1, SparseTree<MyNode>& tree2) {
  SparseTree<MyNode>::PreorderIterator iter1 = tree1.BeginPreorder();
  SparseTree<MyNode>::PreorderIterator iter2 = tree2.BeginPreorder();

  while (iter1 != tree1.EndPreorder()) {
    // Make sure the data is all the same
    assert(iter1->data() == iter2->data());
    assert(iter1->index() == iter2->index());
    assert(iter1.get() != iter2.get());
    ++iter1;
    ++iter2;
  }
}

void TestMerge(SparseTree<MyNode>& tree1, SparseTree<MyNode>& tree2) {
  SparseTree<MyNode> fast_merged(tree1, tree2);

  // Naively merge the two trees:
  SparseTree<MyNode> slow_merged;
  SparseTree<MyNode>::PreorderIterator iter1 = tree1.BeginPreorder();
  while (iter1 != tree1.EndPreorder()) {
    slow_merged.add_node(*(iter1.get()));
    ++iter1;
  }

  SparseTree<MyNode>::PreorderIterator iter2 = tree2.BeginPreorder();
  while (iter2 != tree2.EndPreorder()) {
    slow_merged.add_node(*(iter2.get()));
    ++iter2;
  }


  vector<int> depth_first_order;
  depth_first_order.push_back(8);
  depth_first_order.push_back(5);
  depth_first_order.push_back(1);
  depth_first_order.push_back(9);
  depth_first_order.push_back(5);
  depth_first_order.push_back(1);
  depth_first_order.push_back(9);
  depth_first_order.push_back(8);
  depth_first_order.push_back(7);
  depth_first_order.push_back(6);
  depth_first_order.push_back(4);

  int count = 0;
  SparseTree<MyNode>::PreorderIterator iter3 = fast_merged.BeginPreorder();
  while (iter3 != fast_merged.EndPreorder()) {
    assert(iter3->data() == depth_first_order[count]);
    ++iter3;
    ++count;
  }

  TestEquivalent(fast_merged, slow_merged);

}

void TestDelete1(SparseTree<MyNode>& tree) {
  LargeIndex index;
  index.push_back(0);
  MyNode* three = tree.node(index);

  index.push_back(0);
  MyNode* one = tree.node(index);

  index[1] = 3;
  MyNode* nine = tree.node(index);

  tree.remove_node(nine);
  assert(three->first_child() == one);
  assert(three->last_child() == one);
  assert(one->prev_sibling() == NULL);
  assert(one->next_sibling() == NULL);
  assert(one->first_child() == NULL);
  assert(one->last_child() == NULL);
  assert(tree.size() == 6);

  LargeIndex second_branch;
  second_branch.push_back(1);
  MyNode* two = tree.node(second_branch);

  second_branch.push_back(4);
  MyNode* six = tree.node(second_branch);

  tree.remove_node(six);
  assert(tree.size() == 5);
  assert(two->first_child() == NULL);
  assert(two->last_child() == NULL);

  tree.remove_node(two);
  assert(tree.size() == 4);

  LargeIndex third_branch;
  third_branch.push_back(2);

  MyNode* four = tree.node(third_branch);
  assert(three->next_sibling() == four);
  assert(four->prev_sibling() == three);

}

// Create a tree that looks like:
/*
    5
   /|\
  3 2 4
  /\ \
 1  9 6
*/
void RunTests() {
  LargeIndex index;
  MyNode five(5, index);

  index.push_back(0);
  MyNode three(3, index);

  index[0] = 1;
  MyNode two(2, index);

  index[0] = 2;
  MyNode four(4, index);

  index[0] = 0;
  index.push_back(0);
  MyNode one(1, index);

  index[1] = 3;
  MyNode nine(9, index);

  index[0] = 1;
  index[1] = 4;
  MyNode six(6, index);


  SparseTree<MyNode> tree1, tree2;

  tree1.add_node(five);
  tree1.add_node(three);
  tree1.add_node(two);
  tree1.add_node(four);
  tree1.add_node(one);
  tree1.add_node(nine);
  tree1.add_node(six);


  vector<int> depth_first_order;
  depth_first_order.push_back(5);
  depth_first_order.push_back(3);
  depth_first_order.push_back(1);
  depth_first_order.push_back(9);
  depth_first_order.push_back(2);
  depth_first_order.push_back(6);
  depth_first_order.push_back(4);

  vector<int> breadth_first_order;
  breadth_first_order.push_back(5);
  breadth_first_order.push_back(3);
  breadth_first_order.push_back(2);
  breadth_first_order.push_back(4);
  breadth_first_order.push_back(1);
  breadth_first_order.push_back(9);
  breadth_first_order.push_back(6);

  vector<int> postorder_order;
  postorder_order.push_back(1);
  postorder_order.push_back(9);
  postorder_order.push_back(3);
  postorder_order.push_back(6);
  postorder_order.push_back(2);
  postorder_order.push_back(4);
  postorder_order.push_back(5);

  SparseTree<MyNode>::PreorderIterator iter = tree1.BeginPreorder();
  int count = 0;
  while (iter != tree1.EndPreorder()) {
    assert(iter->data() == depth_first_order[count]);
    ++count;
    ++iter;
  }

  SparseTree<MyNode>::BreadthFirstIterator iter2 = tree1.BeginBreadthFirst();
  count = 0;
  while (iter2 != tree1.EndBreadthFirst()) {
    assert(iter2->data() == breadth_first_order[count]);
    ++count;
    ++iter2;
  }

  SparseTree<MyNode>::PostorderIterator iter3 = tree1.BeginPostorder();
  count = 0;
  while (iter3 != tree1.EndPostorder()) {
    assert(iter3->data() == postorder_order[count]);
    ++count;
    ++iter3;
  }

  TestReadAndWrite(tree1);
  TestCopy(tree1);


  // Create a second tree that looks like:
  /*
       3
      / \
     2   3
        /
       1
      /|\
     9 8 7
  */

  LargeIndex index2;
  MyNode test = MyNode(3, index2);
  tree2.add_node(test);

  index2.push_back(0);
  test = MyNode(2, index2);
  tree2.add_node(test);

  index2[0] = 1;
  test = MyNode(3, index2);
  tree2.add_node(test);

  index2.push_back(0);
  test = MyNode(1, index2);
  tree2.add_node(test);

  index2.push_back(0);
  test = MyNode(9, index2);
  tree2.add_node(test);

  index2[2] = 1;
  test = MyNode(8, index2);
  tree2.add_node(test);

  index2[2] = 2;
  test = MyNode(7, index2);
  tree2.add_node(test);

  TestMerge(tree1, tree2);
  TestMerge(tree2, tree1);

  TestDelete1(tree1);
}

int main(int argc, char** argv) {
  RunTests();
  cout << "*****\nPASS\n*****\n";
  return 0;
}
