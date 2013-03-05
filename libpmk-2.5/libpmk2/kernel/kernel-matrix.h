// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
#ifndef KERNEL_KERNEL_MATRIX_H
#define KERNEL_KERNEL_MATRIX_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace libpmk_util {
/// Data structure for a square symmetric matrix.
class KernelMatrix {
public:
  /// Creates an empty (0x0) matrix.
  KernelMatrix() { }

  /// Creates a (size x size) identity matrix.
  KernelMatrix(int size);

  /// Gets the current size of the matrix.
  int size() const;

  /// Normalizes by dividing k[r][c] by sqrt(k[r][r] * k[c][c]).
  void Normalize();

  /// Normalize by min cardinality.
  /**
   * Requires cards.size() == size(). cards[i] is the cardinality
   * of the ith set. This function will divide k[r][c] by the minimum
   * of cards[r] and cards[c].
   * \sa PointSetList::GetSetCardinalities()
   */
  void NormalizeByMinCardinality(const vector<int>& cards);

  /// Resizes the matrix.
  /**
   * Warning: this can cause data loss if you are shrinking the
   * matrix. If rows are added, any new elements on the diagonal are
   * initialized to 1, all other new elements are 0.
   */
  void Resize(int new_size);

  /// Get a ref to the kernel value at row, col.
  /**
   * This allows you to do things like kernel.at(1, 3) = 37;
   * KernelMatrix will automatically maintain symmetry, so that after
   * you do this, if you call kernel.at(3, 1), it will be 37.
   */
  double& at(int row, int col);

  /// Get the kernel value at row, col.
  double at(int row, int col) const;

  /// Write the matrix to a stream.
  /**
   * File format: <ul>
   * <li> (int32) N, the number of rows (and cols)
   * <li> (1 * double) row 0 (just k[0][0])
   * <li> (2 * double) row 1
   * <li> ...
   * <li> (N * double) row N-1
   * </ul>
   * Aborts if the stream is bad.
   */
  void WriteToStream(ostream& output_stream) const;

  /// Write the matrix to a stream.
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
  // Internally, we represent a kernel matrix as a triangle. Row 0
  // has 1 entry, row 1 has 2 entries, etc.
  vector<vector<double> > matrix_;

};
}  // namespace libpmk_util

#endif  // KERNEL_KERNEL_MATRIX_H
