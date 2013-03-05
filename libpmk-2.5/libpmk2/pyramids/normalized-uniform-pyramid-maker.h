// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
#ifndef PYRAMIDS_NORMALIZED_UNIFORM_PYRAMID_MAKER_H
#define PYRAMIDS_NORMALIZED_UNIFORM_PYRAMID_MAKER_H

#include <vector>
#include <map>
#include <iostream>
#include "pyramids/pyramid-maker.h"
#include "point_set/point-set-list.h"
#include "point_set/point-set.h"
#include "histograms/multi-resolution-histogram.h"

using namespace std;

namespace libpmk {

/// \brief Creates MultiResolutionHistograms with bins of uniform size at
/// each level.
/**
 * The difference between this class and UniformPyramidMaker is that
 * the image dimensions are normalized-- that is, regardless of the
 * minimum and maximum values of a particular feature value, the feature
 * space is always partitioned into a particular number of pieces.
 * Each dimension is always halved at each level in the pyramid.
 *
 * The bin sizes are normalized too-- that is, the top-level bin has
 * size 10000, and bins under that have bin sizes scaled appropriately
 * (i.e., if it is n levels down, the size is (1 / 2^d) * (1 / 2^n)
 * where d is the dimension of the points).
 */
class NormalizedUniformPyramidMaker : public PyramidMaker {
public:
  const static double TOP_LEVEL_BIN_SIZE = 10000;

  NormalizedUniformPyramidMaker(int num_levels);

  /**
   * Must be called after Preprocess() or ReadFromStream().
   */
  int num_levels() const;

  /// Creates a normalized uniform pyramid from a PointSet.
  /**
   * This function allocates memory on its own; it is up to the
   * caller to free the memory.
   */
  virtual MultiResolutionHistogram* MakePyramid(const PointSet& ps);
  virtual MultiResolutionHistogram* MakePyramid(
    const vector<PointRef>& points);


  /// Read parameters and preprocessed data from a stream.
  /**
   * Aborts if the stream is bad.
   */
  void ReadFromStream(istream& input_stream);

  /// Read parameters and preprocessed data from a file.
  /**
   * \sa ReadFromStream
   */
  void ReadFromFile(const char* filename);

  /// Write parameters and preprocessed data to a stream.
  /**
   * Aborts if the stream is bad.
   */
  void WriteToStream(ostream& output_stream) const;

  /// Write parameters and preprocessed data to a file.
  /**
   * \sa WriteToStream
   */
  void WriteToFile(const char* filename) const;

  /// Get the number of levels in the pyramids that will be created.
  /**
   * Must be called after Preprocess() or ReadFromStream().
   */

private:
  int num_levels_;

  // Make sure that the values (ints) are unique. We need to do this for
  // each level, so it's a vector of size num_levels_ of these mappings from
  // index to int.
  vector<map<LargeIndex, int> > vector_map_;

  // A vector of size num_levels. The ith entry tells you the last-used
  // value in vector_map_.
  vector<int> last_vector_map_values_;

  int GetSparseValue(int level, const LargeIndex& index);
  void ProcessPoint(const Point& feature, MultiResolutionHistogram* histogram,
                    const vector<double>& min_values,
                    const vector<double>& max_values);
};
}  // namespace libpmk
#endif  // PYRAMIDS_NORMALIZED_UNIFORM_PYRAMID_MAKER_H
