// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include <sstream>
#include "kernel/matrix.h"

using namespace std;
using namespace libpmk_util;

void RunAllTests() {
  Matrix matrix(4, 3);
  assert(matrix.num_rows() == 4);
  assert(matrix.num_cols() == 3);
  for (int ii = 0; ii < 4; ++ii) {
    for (int jj = 0; jj < 3; ++jj) {
      assert(matrix.at(ii, jj) == 0);
    }
  }

  matrix.at(1, 2) = 4;
  assert(matrix.at(1, 2) == 4);
  assert(matrix.at(2, 1) == 0);

  ostringstream output_stream(ios::binary);
  matrix.WriteToStream(output_stream);

  string s = output_stream.str();
  istringstream in_stream(s, ios::binary);
  Matrix read_matrix;
  read_matrix.ReadFromStream(in_stream);

  assert(read_matrix.num_rows() == 4);
  assert(read_matrix.num_cols() == 3);

  for (int ii = 0; ii < 4; ++ii) {
    for (int jj = 0; jj < 3; ++jj) {
      if (ii == 1 && jj == 2) {
        assert(read_matrix.at(ii, jj) == 4);
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
