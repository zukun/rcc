// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef HISTOGRAMS_MULTI_RESOLUTION_HISTOGRAM_H
#define HISTOGRAMS_MULTI_RESOLUTION_HISTOGRAM_H

#include <iostream>
#include "histograms/bin.h"
#include "tree/sparse-tree.cc"

using namespace std;

namespace libpmk {
template class SparseTree<Bin>;

/// \brief A data structure for a pyramid of histograms, with a link
/// structure between levels.
/**
 * A wrapper class around SparseTree. This data structure encapsulates
 * a SparseTree of Bin objects, where a Bin just contains a weight
 * (size) and a count.
 */

class MultiResolutionHistogram {
public:
  MultiResolutionHistogram() { }

  // Returns the number of bins in this histogram.
  int size() const { return tree_.size(); }

  // Returns the total counts of all of the bins in this histogram.
  double total_counts() const;

  /// Get a pointer to the bin with the specified index.
  /**
   * Returns NULL if the bin is not found.
   */
  Bin* bin(const LargeIndex& index);

  /// Get a pointer to the bin with specified index
  /**
   * Same as GetBin(LargeIndex), but localizes the search to the
   * subtree given by finger. Returns NULL if there is no such Bin
   * in a subtree of <finger>.
   */
  Bin* bin(const LargeIndex& index, Bin* finger);

  /// Get a pointer to the root bin.
  Bin* root() { return tree_.root(); }
  Bin* const root() const { return tree_.root(); }

  /// Insert a copy of the given bin into the tree.
  /**
   * Returns a pointer to the newly-added bin in the tree. This
   * function completely ignores any parent/child/sibling pointers in
   * <new_bin>.
   *
   * This function requires the parent bin to exist. It will not
   * create new bins along the way (it will abort if there is no
   * parent bin, i.e., a bin whose index is a prefix of that of
   * <new_bin> and whose index size is exactly 1 less than the new
   * bin's index size. The insertion happens such that the sibling
   * relationships remain sorted by index.
   *
   * If there is already is a bin with the given index,
   * we add the counts and keep the larger of the two sizes. This
   * applies to the root bin as well.
   *
   */
  Bin* add_bin(const Bin& new_bin);

  // Same as add_bin(), except if the user already has a pointer to the
  // parent, use that. If the parent is invalid, NULL is returned and
  // no bin is added. In order for this to work properly, the parent pointer
  // MUST be a bin which can be accessed through child pointers of
  // root_bin_. In other words, this MultiResolutionHistogram must be the
  // owner of the parent that you pass into it. We do NOT check for this;
  // it is up to the client to make sure of it.
  /// Insert a copy of the given bin into the tree.
  /**
   * Same as add_bin(const Bin&), except it starts the search for the
   * bin at <finger>. A parent to <new_bin> must already exist, and
   * must be present in a sub-branch of <finger> (it may also be
   * <finger>).
   */
  Bin* add_bin(const Bin& new_bin, Bin* finger);

  /// Remove a bin, and all of its children, from the pyramid.
  void remove_bin(Bin* finger);


  /// Normalizes the histogram in-place so that total_count() == 1.
  void Normalize();

  /// \brief Normalizes the histogram in-place by dividing every count
  /// by the given number.
  void Normalize(double divisor);

  /// Reads all the MultiResolutionHistograms in a stream.
  /**
   * File format: <ul>
   * <li> (int32) N, the number of MultiResolutionHistograms.
   * <li> (N * MultiResolutionHistogram) The histograms. </ul>
   *
   * Aborts if input_stream cannot be read.
   */
  static vector<MultiResolutionHistogram*>
  ReadFromStream(istream& input_stream);

  /// Reads just the header from a MultiResolutionHistogram file.
  /**
   * The header just contains one integer, the number of histograms
   * in the rest of the stream. So this method will return the value
   * of that integer.
   */
  static int ReadHeaderFromStream(istream& input_stream);

  /// Reads all the MultiResolutionHistograms in a file.
  /**
   * \sa ReadFromStream
   */
  static vector<MultiResolutionHistogram*>
  ReadFromFile(const char* filename);

  /// Reads some MultiResolutionHistograms from a stream.
  /**
   * <start> specifies the index of the first histogram to read in the
   * stream of data. If <selection_size> is large enough so that this
   * would want to read past the end of the file,
   * ReadSelectionFromStream will just stop reading. Observe that you
   * will need to adjust indices (i.e., if you choose to read Y
   * histograms starting from index X, then result[0] will be
   * the 6th histogram, as opposed to this[5]).
   *
   * Aborts if the input_stream cannot be read.
   */
  static vector<MultiResolutionHistogram*>
  ReadSelectionFromStream(istream& input_stream,
                          int start, int selection_size);

  /// Reads some MultiResolutionHistograms from a file.
  /**
   * \sa ReadSelectionFromStream
   */
  static vector<MultiResolutionHistogram*>
  ReadSelectionFromFile(const char* filename, int start,
                        int selection_size);

  /// Writes all of the MultiResolutionHistograms to a stream.
  /**
   * Aborts if the stream is bad. See ReadFromStream for the file
   * format.  \sa ReadFromStream
   */
  static void WriteToStream(ostream& output_stream,
                            const vector<MultiResolutionHistogram*>& hists);

  /// Writes just a header for a MultiResolutionHistogram file to stream.
  /**
   * \sa ReadHeaderFromStream
   */
  static void WriteHeaderToStream(ostream& output_stream, int num_hists);

  /// Writes all of the MultiResolutionHistograms to a file.
  /**
   * \sa WriteToFile
   */
  static void WriteToFile(const char* filename,
                          const vector<MultiResolutionHistogram*>& hists);

  /// Reads just one histogram from a stream.
  /**
   * File format: <ul>
   * <li> (int32) The total number of bins, including the root
   * <li> For each bin: <ul>
   *     <li> (int32) L, the size of its index (0 for the root, etc.)
   *     <li> (L * int32) the index
   *     <li> (double) size
   *     <li> (double) count
   *
   * The ordering of the bins is a depth-first traversal of the tree.
   *
   * Aborts if the stream is bad.
   */
  static MultiResolutionHistogram*
  ReadSingleHistogramFromStream(istream& input_stream);

  /// Throws out one histogram's worth of data from the stream.
  static void IgnoreSingleHistogramFromStream(istream& input_stream);

  /// Write just one histogram to a stream.
  /**
   * Aborts if the stream is bad. See ReadSingleHistogramFromStream
   * for the format. \sa ReadSingleHistogramFromStream
   */
  static void WriteSingleHistogramToStream(ostream& output_stream,
                                           const MultiResolutionHistogram* h);

private:
  SparseTree<Bin> tree_;
};
}  // namespace libpmk
#endif  // HISTOGRAMS_MULTI_RESOLUTION_HISTOGRAM_H
