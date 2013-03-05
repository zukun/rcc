// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <cfloat>
#include <string>
#include <sstream>
#include "experiment/nn-experiment.h"
#include "experiment/experiment.h"
#include "kernel/kernel-matrix.h"
#include "util/labeled-index.h"

namespace libpmk_util {

NNExperiment::NNExperiment(vector<LabeledIndex> training,
                           vector<LabeledIndex> testing,
                           const KernelMatrix& kernel) :
  Experiment(training, testing, kernel) { }

NNExperiment::NNExperiment(vector<LabeledIndex> training,
                           const KernelMatrix& training_matrix,
                           vector<LabeledIndex> testing,
                           const Matrix& testing_matrix) :
  Experiment(training, training_matrix, testing, testing_matrix) { }


int NNExperiment::Test() {
  int correct = 0;
  for (int ii = 0; ii < (int)testing_.size(); ++ii) {
    const LabeledIndex& test_example(testing_[ii]);

    double best_kernel_value = -DBL_MAX;
    int best_label = -1;
    int best_index = -1;
    bool found_row = false;
    for (int jj = 0; jj < (int)training_.size(); ++jj) {
      const LabeledIndex& train_example(training_[jj]);
      double kernel_value = GetKernelValue(train_example, test_example);
      if (kernel_value > best_kernel_value) {
        best_kernel_value = kernel_value;
        best_label = train_example.label;
        best_index = train_example.index;
        found_row = true;
      }
    }
    assert(found_row);
    SetPrediction(ii, best_label);
    cout << "Predicted index " << best_index << endl;
    if (best_label == test_example.label) {
      ++correct;
    }
  }
  return correct;
}

}  // end namespace libpmk_util
