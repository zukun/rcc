// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// TODO(jjl): what about turning new PointSets with min/max feature
// value beyond min_value_ or max_value_? check for it and add a note in
// the spec.
#ifndef PYRAMIDS_UNIFORM_PYRAMID_MAKER_H
#define PYRAMIDS_UNIFORM_PYRAMID_MAKER_H

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
 * The <tt>discretize_factor</tt> parameter, passed into Preprocess(),
 * is particularly important because it can greatly affect the shape
 * of the resulting pyramid. This parameter multiplies all of the
 * feature values by 10^{discretize_factor}. We typically set it in
 * such a way that the resulting features are on the order of 10^2.
 *
 * It is also important to note that uniform-bin pyramids are useful
 * only when dealing with low-dimensional data (up to about
 * 10-dimensional). When dealing with higher-dimensional features,
 * consider using VGPyramidMaker instead.
 */
class UniformPyramidMaker : public PyramidMaker {
public:
  UniformPyramidMaker();

  /// Set parameters and preprocess the pyramid settings.
  /**
   * \param finest_side_length The length of a side of the smallest
   * bin at the lowest level of the pyramid.
   * \param side_factor The length of the side of the bin is this
   * factor larger for bins one level above.
   * \param discretize_factor Scales all of the features by a factor
   * 10^{discretize_factor}.
   * \param do_translations Determines whether to apply random shifts
   * to the bins.
   * \param global_translations Determines whether the shift should be
   * the same across all levels or not.
   *
   * Rather than preprocessing, ReadFromStream() may be called in
   * lieu of this.
   */
  void Preprocess(const PointSetList& point_sets,
                  double finest_side_length,
                  double side_factor,
                  int discretize_factor,
                  bool do_translations,
                  bool global_translations);

  /// Set parameters and preprocess the pyramid settings.
  void Preprocess(const vector<PointRef>& points,
                  double finest_side_length,
                  double side_factor,
                  int discretize_factor,
                  bool do_translations,
                  bool global_translations);

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
  int num_levels() const;

  /// Creates a uniform pyramid from a PointSet.
  /**
   * Requires Preprocess() or ReadFromStream() to be called first.
   * This function allocates memory on its own; it is up to the
   * caller to free the memory. Note that the input point set should
   * not be on the same scale as the point sets that the
   * UniformPyramidMaker was constructed with. If you specified a
   * nonzero <tt>discretize_factor</tt> in the constructor, the
   * PointSet <ps> will automatically be scaled according to that.
   * Additionally, any translations (if you told the constructor to
   * do so) will also be applied automatically-- you should not have
   * to shift or scale anything in <ps> before calling MakePyramid()
   * on it.
   */
  virtual MultiResolutionHistogram* MakePyramid(const PointSet& ps);
  virtual MultiResolutionHistogram* MakePyramid(
    const vector<PointRef>& points);


private:
  double finest_side_length_;
  double side_factor_;
  int discretize_factor_;
  int feature_dim_;
  bool do_translations_;
  bool global_translations_;
  bool preprocessed_;

  double min_value_;
  double max_value_;

  double diameter_;
  int num_levels_;

  // One vector per level. This is a num_levels * dim matrix.
  vector<vector<double> > translations_;

  // Make sure that the values (ints) are unique. We need to do this for
  // each level, so it's a vector of size num_levels_ of these mappings from
  // index to int.
  vector<map<LargeIndex, int> > vector_map_;

  // A vector of size num_levels. The ith entry tells you the last-used
  // value in vector_map_.
  vector<int> last_vector_map_values_;

  int GetSparseValue(int level, const LargeIndex& index);
  void ComputeHistogramSettings(int dim,
                                double finest_side_length,
                                double side_factor,
                                int discretize_factor,
                                bool do_translations,
                                bool global_translations);
  void ProcessPoint(const Point& feature, MultiResolutionHistogram* histogram);
};
}  // namespace libpmk
#endif  // PYRAMIDS_UNIFORM_PYRAMID_MAKER_H
