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
#include "kernel/kernel-matrix.h"

namespace libpmk_util {

KernelMatrix::KernelMatrix(int size) : matrix_(size) {
  for (int ii = 0; ii < size; ++ii) {
    matrix_[ii].resize(ii+1);
    matrix_[ii][ii] = 1;
  }
}

void KernelMatrix::Resize(int new_size) {
  int old_size = size();
  if (new_size > old_size) {
    matrix_.resize(new_size);
    for (int ii = old_size; ii < new_size; ++ii) {
      matrix_[ii].resize(ii+1);
      matrix_[ii][ii] = 1;
    }
  } else if (new_size < old_size) {
    matrix_.resize(new_size);
  }
}

int KernelMatrix::size() const {
  return matrix_.size();
}

void KernelMatrix::Normalize() {
  int size = this->size();
  vector<double> diag(size);
  for (int ii = 0; ii < size; ++ii) {
    diag[ii] = matrix_[ii][ii];
    assert(diag[ii] != 0);
  }

  double normval;
  for (int ii = 0; ii < size; ++ii) {
    for (int jj = 0; jj <= ii; ++jj) {
      normval = matrix_[ii][jj] / sqrt(diag[ii] * diag[jj]);
      matrix_[ii][jj] = normval;
    }
  }
}

void KernelMatrix::NormalizeByMinCardinality(const vector<int>& cards) {
  int size = this->size();
  assert((int)cards.size() == size);
  for (int ii = 0; ii < size; ++ii) {
    for (int jj = 0; jj <= ii; ++jj) {
      double min_card = cards[ii] < cards[jj] ? cards[ii] : cards[jj];
      matrix_[ii][jj] /= min_card;
    }
  }
}

double& KernelMatrix::at(int row, int col) {
  assert(row < size());
  assert(col < size());
  // The matrix is a triangle, so the first index must always be at least
  // the second index.
  if (col <= row) {
    return matrix_[row][col];
  } else {
    return matrix_[col][row];
  }
}

double KernelMatrix::at(int row, int col) const {
  assert(row < size());
  assert(col < size());
  // The matrix is a triangle, so the first index must always be at least
  // the second index.
  if (col <= row) {
    return matrix_[row][col];
  } else {
    return matrix_[col][row];
  }
}

// File format:
// (int32_t) size
// (double) row 0
// (double*2) row 1
// ...
// (double*n) row n-1
void KernelMatrix::WriteToStream(ostream& output_stream) const {
  int32_t size = this->size();
  output_stream.write((char *)&size, sizeof(int32_t));
  for (int ii = 0; ii < size; ++ii) {
    assert((int)matrix_[ii].size() == ii+1);
    for (int jj = 0; jj < ii+1; ++jj) {
      output_stream.write((char *)&matrix_[ii][jj], sizeof(double));
    }
  }
}

void KernelMatrix::WriteToFile(const char* filename) const {
  ofstream output_stream(filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}

void KernelMatrix::ReadFromStream(istream& input_stream) {
  int32_t size = 0;
  input_stream.read((char *)&size, sizeof(int32_t));

  Resize(size);
  for (int ii = 0; ii < size; ++ii) {
    for (int jj = 0; jj <= ii; ++jj) {
      input_stream.read((char *)&matrix_[ii][jj], sizeof(double));
    }
  }
}

void KernelMatrix::ReadFromFile(const char* filename) {
  ifstream input_stream(filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}
}  // namespace libpmk_util
