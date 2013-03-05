// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of ModelGroup.
//

#include <stdint.h>
#include <set>
#include <vector>

#include "shape_model/model-group.h"

#include "point_set/mutable-point-set-list.h"
#include "point_set/point-set-list.h"
#include "shape_model/implicit-shape-model.h"

using namespace std;
using namespace libpmk;

namespace libpmk_shape_model {

ModelGroup::ModelGroup() { }

ModelGroup::~ModelGroup() {
  clear();
}

void ModelGroup::clear() {
  for (int ii = 0; ii < (int)models_.size(); ++ii) {
    delete models_[ii];
  }
  models_.clear();
  labels_.clear();
}

void ModelGroup::Train(const PointSetList& features,
                       const PointSetList& loc_scales,
                       const vector<int>& labels,
                       double threshold) {
  set<int> unique_labels;
  for (int ii = 0; ii < (int)labels.size(); ++ii) {
    unique_labels.insert(labels[ii]);
  }

  for (set<int>::iterator iter = unique_labels.begin();
       iter != unique_labels.end(); ++iter) {
    // Collect a PointSetList of the features and locs of
    // this label, as well as its scales.
    int label = *iter;
    MutablePointSetList current_features;
    MutablePointSetList current_locations;
    vector<double> scales;

    for (int ii = 0; ii < (int)labels.size(); ++ii) {
      if (labels[ii] == label) {
        current_features.add_point_set(features.point_set(ii));
        current_locations.add_point_set(loc_scales.point_set(ii));
        for (int jj = 0; jj < loc_scales.point_set(ii).size(); ++jj) {
          scales.push_back(loc_scales.point_set(ii).point(jj).feature(2));
        }
      }
    }

    ImplicitShapeModel* model = new ImplicitShapeModel();
    model->Train(current_features, current_locations, scales, threshold);
    models_.push_back(model);
    labels_.push_back(label);
  }
}

void ModelGroup::Test(const PointSet& features, const PointSet& locations,
                      const vector<double>& scales, double threshold,
                      int* out_label, double* out_x, double* out_y,
                      double* out_score) const {
  *out_score = 0;

  for (int ii = 0; ii < (int)models_.size(); ++ii) {
    vector<ObjectPositionVote> position_votes;
    models_[ii]->GetProbabalisticVotes(features, locations, scales, threshold,
                                       &position_votes);
    vector<pair<int, int> > hypotheses;
    vector<double> scores;
    models_[ii]->GetObjectHypotheses(position_votes, 0, 0, 0,
                                     *out_score, &hypotheses, &scores);
    for (int jj = 0; jj < (int)scores.size(); ++jj) {
      if (scores[jj] > *out_score) {
        *out_score = scores[jj];
        *out_label = labels_[ii];
        *out_x = hypotheses[jj].first;
        *out_y = hypotheses[jj].second;
      }
    }
  }
}

void ModelGroup::WriteToStream(ostream& output_stream) const {
  int32_t num_classes = (int)models_.size();
  output_stream.write((char *)(&num_classes), sizeof(int32_t));
  for (int ii = 0; ii < num_classes; ++ii) {
    models_[ii]->WriteToStream(output_stream);
  }

  for (int ii = 0; ii < num_classes; ++ii) {
    int32_t label = labels_[ii];
    output_stream.write((char *)(&label), sizeof(int32_t));
  }
}

void ModelGroup::ReadFromStream(istream& input_stream) {
  clear();
  int32_t num_classes;
  input_stream.read((char *)(&num_classes), sizeof(int32_t));
  for (int ii = 0; ii < num_classes; ++ii) {
    ImplicitShapeModel* model = new ImplicitShapeModel();
    model->ReadFromStream(input_stream);
    models_.push_back(model);
  }

  for (int ii = 0; ii < num_classes; ++ii) {
    int32_t label;
    input_stream.read((char *)(&label), sizeof(int32_t));
    labels_.push_back(label);
  }
}

}  // end namespace libpmk_shape_model
