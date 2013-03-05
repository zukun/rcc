// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
#ifndef KERNEL_MATRIX_H
#define KERNEL_MATRIX_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace libpmk_util {
/// Data structure for a matrix of doubles.
class Matrix {
public:
  /// Creates an empty (0x0) matrix.
  Matrix() { }

  /// Creates a (rows x cols) matrix full of 0s.
  Matrix(int rows, int cols);

  /// Gets the number of rows.
  int num_rows() const;

  /// Gets the number of columns.
  int num_cols() const;

  /// Resizes the matrix.
  /**
   * Warning: this can cause data loss if you are shrinking the
   * matrix. If rows or columns are added, any new elements are 0.
   */
  void Resize(int new_rows, int new_cols);

  /// Get a ref to the kernel value at row, col.
  double& at(int row, int col);

  /// Get the kernel value at row, col.
  double at(int row, int col) const;

  /// Write the matrix to a stream.
  /**
   * File format: <ul>
   * <li> (int32) R, the number of rows
   * <li> (int32) C, the number of cols
   * <li> (double * C) k[0][0], k[0][1], ...
   * <li> ...
   * <li> (double * C) k[R-1][0], k[R-1][1], ...
   * </ul>
   * Aborts if the stream is bad.
   */
  void WriteToStream(ostream& output_stream) const;

  /// Write the matrix to a file.
  /**
   * \sa WriteToStream
   */
  void WriteToFile(const char* filename) const;

  /// Replaces all data in this matrix with new data from the stream.
  /**
   * Aborts if the stream is bad. See WriteToStream() for the file
   * format. \sa WriteToStream()
   */
  void ReadFromStream(istream& input_stream);

  /// Replaces all data in this matrix with new data from the file.
  /**
   * \sa ReadFromStream
   */
  void ReadFromFile(const char* filename);

private:
  vector<vector<double> > matrix_;
  int num_rows_;
  int num_cols_;

};
}  // namespace libpmk_util

#endif  // KERNEL_MATRIX_H
