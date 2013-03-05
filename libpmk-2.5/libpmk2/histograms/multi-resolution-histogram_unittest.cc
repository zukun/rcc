// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <sstream>
#include "histograms/multi-resolution-histogram.h"
#include "histograms/bin.h"

//TODO: add unit test for bin(a,b) and add_bin(a,b)

using namespace std;
using namespace libpmk;

// Test to see if <input> and <read> are equivalent histograms.
void TestHistogramEquality(MultiResolutionHistogram& input,
                           MultiResolutionHistogram& read) {
  assert(input.size() == read.size());
  vector<Bin*> input_stack;
  vector<Bin*> read_stack;

  // DFS-traverse both trees, checking every element every time
  // we pop one.
  input_stack.push_back(input.root());
  read_stack.push_back(read.root());

  while (!input_stack.empty()) {
    // Process the bins at the top of the stacks.
    Bin* current_input = input_stack.back();
    Bin* current_read = read_stack.back();

    // Equality testing.
    assert(current_input->index() == current_read->index());
    assert(current_input->size() == current_read->size());
    assert(current_input->count() == current_read->count());

    // Pop the stacks.
    input_stack.pop_back();
    read_stack.pop_back();

    // Push both bin's children to the top of their stacks, making
    // sure that the children even exist.
    if (current_input->has_child()) {
      assert(current_read->has_child());
      Bin* input_child = (Bin*)(current_input->first_child());
      Bin* read_child = (Bin*)(current_read->first_child());
      while (input_child != NULL) {
        assert(read_child != NULL);
        input_stack.push_back(input_child);
        read_stack.push_back(read_child);
        input_child = (Bin*)(input_child->next_sibling());
        read_child = (Bin*)(read_child->next_sibling());
      }
    }
  }
}

// Write <input> to a file, then read it and check to make sure
// the link structure and data match.
void TestReadAndWrite(MultiResolutionHistogram& input) {
  vector<MultiResolutionHistogram*> vec;
  vec.clear();
  vec.push_back(&input);

  ostringstream out_stream(ios::binary);
  MultiResolutionHistogram::WriteToStream(out_stream, vec);
  string s = out_stream.str();

  istringstream in_stream(s, ios::binary);
  vector<MultiResolutionHistogram*> out =
    MultiResolutionHistogram::ReadFromStream(in_stream);

  assert(out.size() == 1);
  MultiResolutionHistogram& read = *out[0];
  TestHistogramEquality(input, read);

}

// Test whether a multi resolution histogram's:
// (a) siblings are properly sorted.
// (b) bins at the nth level have indices of size exactly n.
// (c) parent bin indices are prefixes of all children's indices.
void TestIndices(const MultiResolutionHistogram& input) {
  vector<Bin*> todo;
  todo.push_back(input.root());

  while (!todo.empty()) {
    Bin* current = todo.back();

    if (current->has_next_sibling()) {
      assert(current->index().size() ==
             current->next_sibling()->index().size());
      int x = current->index().size();
      for (int ii = 0; ii < x - 1; ++ii) {

        assert(current->index()[ii] ==
               current->next_sibling()->index()[ii]);
      }
      assert(current->index()[x-1] <
             current->next_sibling()->index()[x-1]);
    }

    // Are we at the root bin?
    if (current->index().size() == 0) {
      assert(current == input.root());
      assert(!current->has_parent());
      assert(!current->has_next_sibling());
    } else {
      assert(current->has_parent());
      Bin* parent = (Bin*)(current->parent());
      assert((parent->index().size() + 1) == current->index().size());
      for (int ii = 0; ii < (int)parent->index().size(); ++ii) {
        assert(parent->index()[ii] == current->index()[ii]);
      }
    }

    todo.pop_back();

    if (current->has_child()) {
      Bin* child = (Bin*)(current->first_child());
      while (child != NULL) {
        todo.push_back(child);
        child = (Bin*)(child->next_sibling());
      }
    }
  }
}

void TestConsistency(MultiResolutionHistogram& input) {
  TestReadAndWrite(input);
  TestIndices(input);
}

void TestBinPresence(MultiResolutionHistogram &input,
                     const Bin& bin) {
  Bin* b = input.bin(bin.index());
  assert(b != NULL);
  assert(b->size() == bin.size());
  assert(b->index() == bin.index());
  assert(b->count() == bin.count());
}

void TestBinNonPresence(MultiResolutionHistogram &input,
                        const Bin& bin) {
  assert(input.bin(bin.index()) == NULL);
}

// This tests creates a tree that looks like this:
/*
       A
     /   \
    B     C
        / | \
       D  E  F
          |
          G
*/
// The indices are as follows:
// A: (none)
// B: 6
// C: 17
// D: 17 1
// E: 17 42
// F: 17 10531
// G: 17 42 1
//
// Sizes and counts:
// A: 2 8
// B: 5 9
// C: 199 3112
// D: 0.2 491
// E: 142.1 31
// F: 1337 7331
// G: 31 337
// They are created in the following order: A C D F E G B
// Every step along the way, we test for consistency within the internal
// structure of the tree.

void RunCreateTest() {
  MultiResolutionHistogram mrh;
  LargeIndex index;

  Bin A(index);
  A.set_size(2); A.set_count(8);

  index.push_back(6);
  Bin B(index);
  Bin B2(index);
  Bin B3(index);
  B.set_size(5); B.set_count(9);
  B2.set_size(4); B.set_count(1);
  B3.set_size(6); B.set_count(2);

  index.clear();
  index.push_back(17);
  Bin C(index);
  C.set_size(199); C.set_count(3112);

  index.push_back(1);
  Bin D(index);
  D.set_size(0.2); D.set_count(491);

  index.clear(); index.push_back(17); index.push_back(42);
  Bin E(index);
  E.set_size(142.1); E.set_count(31);

  index.clear(); index.push_back(17); index.push_back(10531);
  Bin F(index);
  F.set_size(1337); F.set_count(7331);

  index.clear(); index.push_back(17); index.push_back(42); index.push_back(1);
  Bin G(index);
  G.set_size(31); G.set_count(337);

  // Now do the actual testing:
  assert(mrh.size() == 1);
  assert(mrh.root()->size() == 0);
  assert(mrh.root()->count() == 0);

  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinNonPresence(mrh, B);
  TestBinNonPresence(mrh, C);
  TestBinNonPresence(mrh, D);
  TestBinNonPresence(mrh, E);
  TestBinNonPresence(mrh, F);
  TestBinNonPresence(mrh, G);

  mrh.root()->set_size(A.size());
  mrh.root()->set_count(A.count());
  assert(mrh.size() == 1);
  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinPresence(mrh, A);
  TestBinNonPresence(mrh, B);
  TestBinNonPresence(mrh, C);
  TestBinNonPresence(mrh, D);
  TestBinNonPresence(mrh, E);
  TestBinNonPresence(mrh, F);
  TestBinNonPresence(mrh, G);

  mrh.add_bin(C);
  assert(mrh.size() == 2);
  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinPresence(mrh, A);
  TestBinNonPresence(mrh, B);
  TestBinPresence(mrh, C);
  TestBinNonPresence(mrh, D);
  TestBinNonPresence(mrh, E);
  TestBinNonPresence(mrh, F);
  TestBinNonPresence(mrh, G);

  mrh.add_bin(D);
  assert(mrh.size() == 3);
  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinPresence(mrh, A);
  TestBinNonPresence(mrh, B);
  TestBinPresence(mrh, C);
  TestBinPresence(mrh, D);
  TestBinNonPresence(mrh, E);
  TestBinNonPresence(mrh, F);
  TestBinNonPresence(mrh, G);

  mrh.add_bin(F);
  assert(mrh.size() == 4);
  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinPresence(mrh, A);
  TestBinNonPresence(mrh, B);
  TestBinPresence(mrh, C);
  TestBinPresence(mrh, D);
  TestBinNonPresence(mrh, E);
  TestBinPresence(mrh, F);
  TestBinNonPresence(mrh, G);

  mrh.add_bin(E);
  assert(mrh.size() == 5);
  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinPresence(mrh, A);
  TestBinNonPresence(mrh, B);
  TestBinPresence(mrh, C);
  TestBinPresence(mrh, D);
  TestBinPresence(mrh, E);
  TestBinPresence(mrh, F);
  TestBinNonPresence(mrh, G);


  mrh.add_bin(G);
  assert(mrh.size() == 6);
  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinPresence(mrh, A);
  TestBinNonPresence(mrh, B);
  TestBinPresence(mrh, C);
  TestBinPresence(mrh, D);
  TestBinPresence(mrh, E);
  TestBinPresence(mrh, F);
  TestBinPresence(mrh, G);

  mrh.add_bin(B);
  assert(mrh.size() == 7);
  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinPresence(mrh, A);
  TestBinPresence(mrh, B);
  TestBinPresence(mrh, C);
  TestBinPresence(mrh, D);
  TestBinPresence(mrh, E);
  TestBinPresence(mrh, F);
  TestBinPresence(mrh, G);

  // Now mutate a bin and check for consistency.
  mrh.add_bin(B2);
  assert(mrh.size() == 7);
  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinPresence(mrh, A);
  TestBinPresence(mrh, C);
  TestBinPresence(mrh, D);
  TestBinPresence(mrh, E);
  TestBinPresence(mrh, F);
  TestBinPresence(mrh, G);
  Bin* B2_test = mrh.bin(B2.index());
  assert(B2_test != NULL);
  // Since B2 is smaller than B, make sure the size didn't change.
  assert(B2_test->size() == B.size());
  assert(B2_test->index() == B.index());
  assert(B2_test->count() == B.count() + B2.count());

  mrh.add_bin(B3);
  assert(mrh.size() == 7);
  TestReadAndWrite(mrh);
  TestConsistency(mrh);
  TestBinPresence(mrh, A);
  TestBinPresence(mrh, C);
  TestBinPresence(mrh, D);
  TestBinPresence(mrh, E);
  TestBinPresence(mrh, F);
  TestBinPresence(mrh, G);
  Bin* B3_test = mrh.bin(B3.index());
  assert(B3_test != NULL);
  // Since B3 is larger than B, make sure the size changed.
  assert(B3_test->size() == B3.size());
  assert(B3_test->index() == B.index());
  assert(B3_test->count() == B.count() + B2.count() + B3.count());
}

int main(int argc, char** argv) {
  RunCreateTest();
  cout << "******\nPASS\n******" << endl;
  return 0;
}
