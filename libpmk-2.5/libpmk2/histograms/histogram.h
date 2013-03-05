// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
#ifndef HISTOGRAMS_HISTOGRAM_H
#define HISTOGRAMS_HISTOGRAM_H

#include <string>
#include <iostream>
#include "histograms/bin.h"

using namespace std;

namespace libpmk {
/// A sparse representation for a flat (1-D) histogram.
/**
 * Note that this implementation sorts and consolidates bins lazily,
 * meaning that there can be bins with duplicate indices in the
 * internal structure. However, the public interface behaves as though
 * the internal data is always sorted and consolidated, so there is no
 * need to worry about duplicate bins or unsorted bins when accessing
 * elements using any of the public methods.
 */
class Histogram {
public:
  Histogram();
  ~Histogram();

  /// Adds a copy of <new_bin> to the histogram.
  /**
   * By "adding a bin", we mean looking for the Bin in this histogram
   * that has the same index (the LargeIndex identifier) as <new_bin>
   * and increasing the count by that much. It also will set the size
   * of the bin to be the <b>maximum</b> of the two bin sizes. The
   * parent, child, and sibling pointers of <new_bin> are completely
   * ignored.
   */
  Bin* add_bin(const Bin& new_bin);

  /// Sorts and consolidates bins.
  /**
   * It is not necessary to call this manually, as all of the sorting
   * and consolidation is done automatically (lazily). However, for
   * performance reasons, you may call this manually.
   */
  void SortBins() const;

  /// Gets the total number of bins with unique index.
  int size() const;

  /// Assuming the Bins are sorted, gets the <ii>th bin in the ordering.
  /**
   * The Bins are sorted in order of increasing index.
   */
  const Bin* bin(int ii);

  /// Gets the bin with the given index.
  /**
   * This will return NULL if no such Bin is found.
   */
  const Bin* bin(const LargeIndex& index);

  /// Gets the Bin with the given index after the <finger>th Bin.
  /**
   * This does a linear search for the Bin with the given index, but
   * begins the search from the <finger>th Bin in the sorted list
   * of all Bins.
   */
  const Bin* bin(const LargeIndex& index, int finger);

  // We pass pointers instead of const references, since this function has
  // the capability to modify the histograms themselves (by sorting their
  // contents).
  /// Compute the histogram intersection.
  static double ComputeIntersection(Histogram* first, Histogram* second);

  // We pass pointers instead of const references, since this function has
  // the capability to modify the histograms themselves (by sorting their
  // contents).
  /// Computes the chi-squared distance between two Histograms.
  static double ComputeChiSquaredDistance(Histogram* first, Histogram* second);

  static double ComputeSumSquaredDistance(Histogram* first, Histogram* second);
  /// Write a vector of Histograms to a stream.
  /**
   * The output format is as follows: <ul>
   * <li> (int32) N, the number of histograms
   * <li> (N * Histogram) the histograms.</ul>
   */
  static void WriteToStream(ostream& output_stream,
                            const vector<Histogram*>& histograms);

  /// Write a single Histogram to a stream.
  /**
   * The output format is as follows: <ul>
   * <li> (int32) B, the number of bins
   * <li> For each bin: <ul>
   *    <li> (int32) S, the size of the LargeIndex identifier
   *    <li> (S * int32) the index
   *    <li> (double) size
   *    <li> (double) count
   *
   * The bins are sorted before they are written. They are sorted in
   * order of increasing index.
   */
  static void WriteSingleHistogramToStream(ostream& output_stream,
                                           Histogram* h);


  /// Read a vector of Histograms from a stream.
  /**
   * This function allocates new memory for the histograms. The
   * caller is responsible for freeing it. For the file format, see
   * WriteToStream.
   * \sa WriteToStream
   */
  static vector<Histogram*> ReadFromStream(istream& input_stream);

  /// Read a single Histogram from a stream.
  /**
   * This function allocates new memory for the histogram. The caller
   * is responsible for freeing it. For the file format, see
   * ReadFromStream.
   * \sa ReadFromStream
   */
  static Histogram* ReadSingleHistogramFromStream(istream& input_stream);

  /// Normalizes the histogram by number of entries.
  void Normalize();

private:
  // Disallow evil constructors
  Histogram(const Histogram&);
  Histogram& operator=(const Histogram&);

  mutable vector<Bin*> bins_;
  mutable bool is_sorted_;
};
}  // namespace libpmk

#endif  // HISTOGRAMS_HISTOGRAM_H
