// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This stores a set of implicit shape models of different classes.
//

#ifndef MULTIOBJ_SHAPE_MODEL_MODEL_GROUP_H
#define MULTIOBJ_SHAPE_MODEL_MODEL_GROUP_H

#include <iostream>
#include <set>
#include <vector>
#include "point_set/mutable-point-set-list.h"
#include "point_set/point-set-list.h"
#include "shape_model/implicit-shape-model.h"

using namespace std;
using namespace libpmk;

namespace libpmk_shape_model {

class ModelGroup {
public:
  ModelGroup();
  ~ModelGroup();

  void clear();

  // Given a large set of images, train the model group.  If there are
  // N images, there must be N point sets in both PSLs, and N entries
  // in <labels>. The features can be any type of feature.  The
  // <loc_scales> are 3-dimensional: [x, y, scale].  The labels are
  // integers; there are no requirements on them (they can be
  // anything; they don't have to start from 0, etc).
  // The threshold is for the RNN clusterer. A typical default value
  // is between -30000 and -100000.
  void Train(const PointSetList& features,
             const PointSetList& loc_scales,
             const vector<int>& labels,
             double threshold);

  void Test(const PointSet& features, const PointSet& locations,
            const vector<double>& scales, double threshold,
            int* out_label, double* out_x, double* out_y,
            double* out_score) const;

  /// Writes the model to a stream.
  /**
   * The format is as follows:
   * <ul>
   * <li>(int32_t) num classes, N
   * <li> For each class:
   * <ul><li>(ImplicitShapeModel) the shape model
   *     <li>(int32_t) the label
   * </ul>
   */
  void WriteToStream(ostream& output_stream) const;

  /// Reads the model from a stream.
  /**
   * This function will clear any data it currently has.
   */
  void ReadFromStream(istream& input_stream);

  ImplicitShapeModel* mutable_model(int index) { return models_.at(index); }

private:
  // These are parallel vectors containing the class labels and their
  // corresponding shape models.
  vector<ImplicitShapeModel*> models_;
  vector<int> labels_;
};
}  // end namespace libpmk_shape_model

#endif  // MULTIOBJ_SHAPE_MODEL_MODEL_GROUP_H
