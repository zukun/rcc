// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <sstream>
#include "histograms/histogram.h"
#include "histograms/bin.h"

using namespace std;
using namespace libpmk;

void TestReadAndWrite(Histogram& input) {
  vector<Histogram*> vec_in;
  vec_in.push_back(&input);

  ostringstream out_stream(ios::binary);
  Histogram::WriteToStream(out_stream, vec_in);
  string s = out_stream.str();

  istringstream in_stream(s, ios::binary);
  vector<Histogram*> vec_out =
    Histogram::ReadFromStream(in_stream);
  assert((int)vec_out.size() == 1);

  Histogram* next = vec_out[0];
  assert(next->size() == input.size());
  for (int ii = 0; ii < input.size(); ++ii) {
    const Bin* first = input.bin(ii);
    const Bin* second = next->bin(ii);
    assert(first->index() == second->index());
    assert(first->size() == second->size());
    assert(first->count() == second->count());
  }
  delete next;
}

void RunAllTests() {
  LargeIndex ind, ind2, ind3;
  ind.push_back(1); ind.push_back(3);
  ind2.push_back(1); ind2.push_back(2);
  ind3.push_back(2);

  Bin bin1(ind);
  Bin bin2(ind2);
  Bin bin3(ind3);
  Bin bin4(ind3);

  bin1.set_size(1);
  bin2.set_size(1);
  bin3.set_size(1);
  bin4.set_size(0.5);

  bin1.set_count(7);
  bin2.set_count(4);
  bin3.set_count(1);
  bin4.set_count(1);

  Histogram h;
  h.add_bin(bin1);
  h.add_bin(bin4);
  h.add_bin(bin2);
  h.add_bin(bin3);

  assert(h.bin(0)->index() == ind2);
  assert(h.bin(1)->index() == ind);
  assert(h.bin(2)->index() == ind3);
  assert(h.bin(0)->count() == 4);
  assert(h.bin(1)->count() == 7);
  assert(h.bin(2)->count() == 2);
  TestReadAndWrite(h);

  Histogram h2;
  bin3.set_count(2);
  bin1.set_count(10);
  h2.add_bin(bin3);
  h2.add_bin(bin1);
  TestReadAndWrite(h2);

  double intersect = Histogram::ComputeIntersection(&h, &h2);
  assert(intersect == 9);
}

int main(int argc, char** argv) {
  RunAllTests();
  cout << "******\nPASS\n******" << endl;
  return 0;
}
