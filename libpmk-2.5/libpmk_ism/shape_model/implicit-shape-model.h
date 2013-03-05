// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This records an implicit shape model based on training examples for
// one particular class.
//

#ifndef MULTIOBJ_SHAPE_MODEL_IMPLICIT_SHAPE_MODEL_H
#define MULTIOBJ_SHAPE_MODEL_IMPLICIT_SHAPE_MODEL_H

#include <iostream>
#include <memory>
#include <vector>
#include "point_set/point-set-list.h"
#include "shape_model/codebook-entry.h"
#include "shape_model/object-position-vote.h"

using namespace std;
using namespace libpmk;

namespace libpmk_shape_model {

class ImplicitShapeModel {
public:
  ImplicitShapeModel();
  ~ImplicitShapeModel();
  
  // This trains the implicit shape model with the given features and
  // feature locations. Later, we can extend this to include scales
  // as well. The locations are relative to the center of the image,
  // where 0,0 is the center. They are 2-dimensional. X increases
  // going to the right, and Y increases going DOWN (not up!). If
  // <locations> has more than 2 dimensions, only the first two
  // are used as X/Y and the rest are ignored.
  //
  // The <scales> vector is parallel to features and locations. It
  // denotes the radius, in pixels, of the patch.
  //
  // The <dimensions> vector gives the width and height of each
  // training example.
  //
  // The threshold argument is used for the RNN clustering step.  It
  // specifies the minimum similarity required to merge two clusters.
  // Take note that currently, the RNN algorithm is set up in a way
  // that all similarities are negative (the closer to 0, the more
  // similar), so set the threshold appropriately. In general, a
  // threshold too low might cause too much to get lumped into one
  // cluster; a threshold too high will cause the algorithm to not
  // find any clusters.
  //
  // If you don't care and just want to run quick tests, know that it
  // is better to set a threshold too low than one too high.
  void Train(const PointSetList& features,
             const PointSetList& locations,
             const vector<double>& scales,
             double threshold);

  // Votes on the locations of the object given a new image with
  // features and locations. The result is stored in out_votes, which
  // may not be NULL. out_votes will be cleared first.
  void GetProbabalisticVotes(const PointSet& features,
                             const PointSet& locations,
                             const vector<double>& scales,
                             double threshold,
                             vector<ObjectPositionVote>* out_votes) const;

  void GetObjectHypotheses(
    const vector<ObjectPositionVote>& votes,
    int image_width, int image_height, int grid_size,
    double threshold,
    vector<pair<int, int> >* out_hypotheses,
    vector<double>* out_scores) const;



  // Like GetObjectHypotheses(), except it only returns the top match.
  // But with the top match, it also records the contribution of each
  // feature point (identified by <votes>) to that hypothesis, where
  // it's 0 if it didn't contribute.
  void GetBestHypothesisWithVotes(
    const vector<ObjectPositionVote>& votes,
    int image_width, int image_height, int grid_size,
    double* best_score,
    pair<int, int>* hypothesis,
    vector<double>* weights) const;


  void clear();

  /// Writes the model to a stream.
  /**
   * The format is as follows:
   * <ul>
   * <li>(int32_t) num codebook entries, N
   * <li>(int32_t) the feature dim, d
   * <li>(PointSet[d]) Cluster centers (codebook entry centers)
   * <li>(double * N) variance of each codebook entry
   * <li>For each codebook entry:
   *    <ul><li>(CodebookEntry) the codebook entry</ul>
   * </ul>
   *
   * The model must already be trained. If the model was never trained,
   * this method will fail.
   */
  void WriteToStream(ostream& output_stream) const;

  /// Reads the model from a stream.
  /**
   * This function will clear any data it currently has.
   */
  void ReadFromStream(istream& input_stream);


private:
  bool trained_;

  // These two structures are parallel; we need to store the variances
  // so we can efficiently compute cluster similarity.
  auto_ptr<PointSet> cluster_centers_;
  vector<double> cluster_variances_;

  vector<CodebookEntry*> codebook_;

};

}  // end namespace libpmk_shape_model

#endif  // MULTIOBJ_SHAPE_MODEL_IMPLICIT_SHAPE_MODEL_H
