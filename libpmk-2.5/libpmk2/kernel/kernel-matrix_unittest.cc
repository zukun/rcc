// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <sstream>
#include "kernel/kernel-matrix.h"

using namespace std;
using namespace libpmk_util;

void RunAllTests() {
  KernelMatrix matrix(4);
  assert(matrix.size() == 4);
  for (int ii = 0; ii < 4; ++ii) {
    for (int jj = 0; jj < 4; ++jj) {
      if (ii == jj) {
        assert(matrix.at(ii, jj) == 1);
      } else {
        assert(matrix.at(ii, jj) == 0);
      }
    }
  }

  matrix.at(1, 3) = 4;
  assert(matrix.at(1, 3) == 4);
  assert(matrix.at(3, 1) == 4);

  matrix.at(0, 0) = 1;
  matrix.at(1, 1) = 2;
  matrix.at(2, 2) = 3;
  matrix.at(3, 3) = 4;
  matrix.Normalize();
  assert(matrix.size() == 4);
  for (int ii = 0; ii < 4; ++ii) {
    assert(matrix.at(ii, ii) == 1);
  }

  matrix.Resize(6);
  assert(matrix.size() == 6);
  for (int ii = 0; ii < 6; ++ii) {
    assert(matrix.at(ii, ii) == 1);
  }
  assert(matrix.at(1, 3) > 0);
  matrix.Resize(4);
  for (int ii = 0; ii < 4; ++ii) {
    assert(matrix.at(ii, ii) == 1);
  }
  assert(matrix.at(1, 3) > 0);

  matrix.at(1, 3) = 3;
  matrix.at(0, 0) = 1;
  matrix.at(1, 1) = 2;
  matrix.at(2, 2) = 3;
  matrix.at(3, 3) = 4;

  ostringstream output_stream(ios::binary);
  matrix.WriteToStream(output_stream);

  string s = output_stream.str();
  istringstream in_stream(s, ios::binary);
  KernelMatrix read_matrix;
  read_matrix.ReadFromStream(in_stream);

  assert(read_matrix.size() == 4);

  for (int ii = 0; ii < 4; ++ii) {
    for (int jj = 0; jj < 4; ++jj) {
      if (ii == jj) {
        assert(read_matrix.at(ii, jj) == ii+1);
      } else if (ii == 1 && jj == 3) {
        assert(read_matrix.at(ii, jj) == 3);
      } else if (ii == 3 && jj == 1) {
        assert(read_matrix.at(ii, jj) == 3);
      } else {
        assert(read_matrix.at(ii, jj) == 0);
      }
    }
  }
}


int main(int argc, char** argv) {
  RunAllTests();
  cout << "******\nPASS\n******" << endl;
  return 0;
}
