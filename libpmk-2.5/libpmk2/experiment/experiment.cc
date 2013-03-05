// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <vector>
#include "experiment/experiment.h"
#include "util/labeled-index.h"
#include "kernel/kernel-matrix.h"

using namespace libpmk;
using namespace libpmk_util;

Experiment::Experiment(vector<LabeledIndex> training,
                       vector<LabeledIndex> testing,
                       const KernelMatrix& kernel) :
  training_(training), testing_(testing), kernel_(kernel) {
  predictions_.resize(testing_.size());
}

Experiment::Experiment(vector<LabeledIndex> training,
                       const KernelMatrix& training_matrix,
                       vector<LabeledIndex> testing,
                       const Matrix& testing_matrix) :
  training_(training), testing_(testing), kernel_(training_matrix) {
  int num_train = (int)training_.size();
  int num_test = (int)testing_.size();

  assert(training_matrix.size() == num_train);
  assert(testing_matrix.num_rows() == num_train);
  assert(testing_matrix.num_cols() == num_test);

  // Lump the training matrix and testing matrix into one big kernel
  // matrix by just adding the test examples to the end. If we do this,
  // we need to modify the testing LabeledIndices since the indices
  // are now offset by training_.size().
  for (int ii = 0; ii < num_test; ++ii) {
    testing_[ii].index += num_train;
  }

  // By doing this, the part of the resulting kernel matrix that
  // describes similarity between test examples is the identity
  // matrix (not that it should matter).
  kernel_.Resize(num_train + num_test);
  for (int ii = 0; ii < num_train; ++ii) {
    for (int jj = 0; jj < num_test; ++jj) {
      kernel_.at(ii, jj + num_train) = testing_matrix.at(ii, jj);
    }
  }

  predictions_.resize(testing_.size());
}

int Experiment::GetPrediction(int test_index) const {
  assert(test_index < (int)testing_.size());
  return predictions_[test_index];
}

void Experiment::SetPrediction(int test_index, int prediction) {
  assert(test_index < (int)testing_.size());
  predictions_[test_index] = prediction;
}

int Experiment::GetNumCorrect() const {
  int count = 0;
  for (int ii = 0; ii < (int)testing_.size(); ++ii) {
    if (testing_[ii].label == predictions_[ii]) {
      ++count;
    }
  }
  return count;
}

int Experiment::GetNumCorrect(int label) const {
  int count = 0;
  for (int ii = 0; ii < (int)testing_.size(); ++ii) {
    if (testing_[ii].label == label &&
        testing_[ii].label == predictions_[ii]) {
      ++count;
    }
  }
  return count;
}

int Experiment::GetNumTestExamples() const {
  return testing_.size();
}

int Experiment::GetNumTestExamples(int label) const {
  int count = 0;
  for (int ii = 0; ii < (int)testing_.size(); ++ii) {
    if (testing_[ii].label == label) {
      ++count;
    }
  }
  return count;
}

double Experiment::GetAccuracy() const {
  return (static_cast<double>(GetNumCorrect()) /
          static_cast<double>(GetNumTestExamples()));
}

double Experiment::GetAccuracy(int label) const {
  return (static_cast<double>(GetNumCorrect(label)) /
          static_cast<double>(GetNumTestExamples(label)));
}

double Experiment::GetKernelValue(int row, int col) const {
  return kernel_.at(row, col);
}

double Experiment::GetKernelValue(const LabeledIndex& row,
                                  const LabeledIndex& col) const {
  return kernel_.at(row.index, col.index);
}
