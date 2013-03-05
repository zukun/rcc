// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "kernel/matrix.h"

namespace libpmk_util {

Matrix::Matrix(int rows, int cols) : matrix_(rows), num_rows_(rows),
                                     num_cols_(cols) {
  for (int ii = 0; ii < rows; ++ii) {
    matrix_[ii].resize(cols);
  }
}

void Matrix::Resize(int new_rows, int new_cols) {
  num_rows_ = new_rows;
  num_cols_ = new_cols;
  matrix_.resize(new_rows);
  for (int ii = 0; ii < new_rows; ++ii) {
    matrix_[ii].resize(new_cols);
  }
}

int Matrix::num_rows() const {
  return num_rows_;
}

int Matrix::num_cols() const {
  return num_cols_;
}

double& Matrix::at(int row, int col) {
  assert(row < num_rows());
  assert(col < num_cols());
  return matrix_[row][col];
}

double Matrix::at(int row, int col) const {
  assert(row < num_rows());
  assert(col < num_cols());
  return matrix_[row][col];
}

// File format:
// (int32_t) num rows
// (int32_t) num cols
// (double * C) row 0
// (double * C) row 1
// ...
// (double * C) row R-1
void Matrix::WriteToStream(ostream& output_stream) const {
  int32_t rows = num_rows();
  int32_t cols = num_cols();
  output_stream.write((char *)&rows, sizeof(int32_t));
  output_stream.write((char *)&cols, sizeof(int32_t));
  for (int ii = 0; ii < rows; ++ii) {
    for (int jj = 0; jj < cols; ++jj) {
      output_stream.write((char *)&matrix_[ii][jj], sizeof(double));
    }
  }
}

void Matrix::WriteToFile(const char* filename) const {
  ofstream output_stream(filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}

void Matrix::ReadFromStream(istream& input_stream) {
  int32_t rows = 0;
  int32_t cols = 0;
  input_stream.read((char *)&rows, sizeof(int32_t));
  input_stream.read((char *)&cols, sizeof(int32_t));

  Resize(rows, cols);
  for (int ii = 0; ii < rows; ++ii) {
    for (int jj = 0; jj < cols; ++jj) {
      input_stream.read((char *)&matrix_[ii][jj], sizeof(double));
    }
  }
}

void Matrix::ReadFromFile(const char* filename) {
  ifstream input_stream(filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

}  // namespace libpmk_util
