// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <sstream>
#include "tree/tree-node.h"
#include "tree/tree.cc"

using namespace libpmk;

class MyNode : public TreeNode {
public:
  MyNode() : TreeNode() {}
  MyNode(int id) : TreeNode(id), data_(0) {}
  MyNode(int data, int id) : TreeNode(id), data_(data) {}
  virtual ~MyNode() { }
  virtual void Combine(const TreeNode& other) {
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

template class Tree<MyNode>;

void TestReadAndWrite(Tree<MyNode>& tree) {
  ostringstream out_stream(ios::binary);
  tree.WriteToStream(out_stream);
  string s = out_stream.str();

  Tree<MyNode> tree2;
  istringstream in_stream(s, ios::binary);
  tree2.ReadFromStream(in_stream);

  Tree<MyNode>::PreorderIterator iter1 = tree.BeginPreorder();
  Tree<MyNode>::PreorderIterator iter2 = tree2.BeginPreorder();

  while (iter1 != tree.EndPreorder()) {
    MyNode* node1 = tree.node(iter1.id());
    MyNode* node2 = tree.node(iter2.id());
    assert(node1->data() == node2->data());
    ++iter1;
    ++iter2;
  }
}


void TestCopy(Tree<MyNode>& original) {
  // Serialize it first and read it back, to make sure we get a true
  // copy and to make sure that <tree> isn't modified.
  ostringstream out_stream(ios::binary);
  original.WriteToStream(out_stream);
  string s = out_stream.str();

  Tree<MyNode> serialized;
  istringstream in_stream(s, ios::binary);
  serialized.ReadFromStream(in_stream);


  Tree<MyNode> copied(original);
  Tree<MyNode>::PreorderIterator iter1 = original.BeginPreorder();
  Tree<MyNode>::PreorderIterator iter2 = serialized.BeginPreorder();
  Tree<MyNode>::PreorderIterator iter3 = copied.BeginPreorder();

  while (iter1 != original.EndPreorder()) {
    // Make sure the data is all the same
    MyNode* node1 = original.node(*iter1);
    MyNode* node2 = serialized.node(*iter2);
    MyNode* node3 = copied.node(*iter3);

    assert(node1->data() == node2->data());
    assert(node2->data() == node3->data());
    assert(node1->id() == node2->id());
    assert(node2->id() == node3->id());

    // But also make sure that the pointers themselves are different,
    // i.e., they are distinct in memory.
    assert(node1 != node2);
    assert(node2 != node3);
    assert(node1 != node3);

    ++iter1;
    ++iter2;
    ++iter3;
  }
}



// Checks to see that two trees have the same content but different pointers.
void TestEquivalent(Tree<MyNode>& tree1, Tree<MyNode>& tree2) {
  Tree<MyNode>::PreorderIterator iter1 = tree1.BeginPreorder();
  Tree<MyNode>::PreorderIterator iter2 = tree2.BeginPreorder();

  while (iter1 != tree1.EndPreorder()) {
    // Make sure the data is all the same
    MyNode* node1 = tree1.node(iter1.id());
    MyNode* node2 = tree2.node(iter2.id());
    assert(node1->data() == node2->data());
    assert(node1->id() == node2->id());
    assert(node1 != node2);
    ++iter1;
    ++iter2;
  }
}


void TestDelete(Tree<MyNode>& tree) {
  // In the tree, the root (data=5) has ID 0, and the rest have
  // ID = data.
  MyNode* root  = tree.node(0);
  MyNode* node3 = tree.node(3);
  MyNode* node2 = tree.node(2);

  assert(tree.size() == 7);
  tree.DeleteNode(9);
  assert(tree.size() == 6);
  assert(node3->child_size() == 1);
  assert(node3->child(0) == 1);

  tree.DeleteNode(6);
  assert(tree.size() == 5);
  assert(!node2->has_child());
  assert(node2->child_size() == 0);

  // This deletes both nodes 3 and 1.
  tree.DeleteNode(3);
  assert(tree.size() == 3);
  assert(root->child_size() == 2);
  assert(root->child(0) == 2);
  assert(root->child(1) == 4);
}


// Create a tree that looks like:
/*
      5
     /|\
    3 2 4
   /\ \
  1  9 6

*/
// TODO(jjl): test adding nodes that have invalid IDs.
void RunTests() {
  MyNode five(5, 0);  // root
  MyNode three(3, 3);
  MyNode two(2, 2);
  MyNode four(4, 4);
  MyNode one(1, 1);
  MyNode nine(9, 9);
  MyNode six(6, 6);

  Tree<MyNode> tree1;
  MyNode* root  = tree1.add_node(five);
  MyNode* node3 = tree1.add_node(three);
  MyNode* node2 = tree1.add_node(two);
  MyNode* node4 = tree1.add_node(four);
  MyNode* node1 = tree1.add_node(one);
  MyNode* node9 = tree1.add_node(nine);
  MyNode* node6 = tree1.add_node(six);

  assert(root->id() == 0);

  root->add_child(node3->id());
  root->add_child(node2->id());
  root->add_child(node4->id());

  node3->add_child(node1->id());
  node3->add_child(node9->id());
  node3->set_parent(root->id());

  node1->set_parent(node3->id());
  node9->set_parent(node3->id());

  node2->add_child(node6->id());
  node2->set_parent(root->id());

  node6->set_parent(node2->id());
  node4->set_parent(root->id());


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

  Tree<MyNode>::PreorderIterator iter = tree1.BeginPreorder();

  int count = 0;
  while (iter != tree1.EndPreorder()) {
    MyNode* node = tree1.node(*iter);
    assert(node->data() == depth_first_order[count]);
    ++count;
    ++iter;
  }

  Tree<MyNode>::BreadthFirstIterator iter2 = tree1.BeginBreadthFirst();
  count = 0;
  while (iter2 != tree1.EndBreadthFirst()) {
    MyNode* node = tree1.node(*iter2);
    assert(node->data() == breadth_first_order[count]);
    ++count;
    ++iter2;
  }

  Tree<MyNode>::PostorderIterator iter3 = tree1.BeginPostorder();
  count = 0;
  while (iter3 != tree1.EndPostorder()) {
    MyNode* node = tree1.node(*iter3);
    assert(node->data() == postorder_order[count]);
    ++count;
    ++iter3;
  }

  TestReadAndWrite(tree1);
  TestCopy(tree1);
  TestDelete(tree1);
}

int main(int argc, char** argv) {
  RunTests();
  cout << "*****\nPASS\n*****\n";
  return 0;
}
