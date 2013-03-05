// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef EXPERIMENT_NN_EXPERIMENT_H
#define EXPERIMENT_NN_EXPERIMENT_H

#include <vector>
#include "experiment/experiment.h"
#include "kernel/matrix.h"
#include "kernel/kernel-matrix.h"
#include "util/labeled-index.h"


using namespace std;
using namespace libpmk;

namespace libpmk_util {

/// Run experiments using 1-NN.
class NNExperiment : public Experiment {
public:
  /**
   * <kernel> includes pairwise kernel values for all data (both
   * training and testing). The LabeledIndices in <training> and
   * <testing> specify which row of the kernel to look at.
   */
  NNExperiment(vector<LabeledIndex> training,
               vector<LabeledIndex> testing,
               const KernelMatrix& kernel);

  /**
   * <training_matrix> is a kernel matrix for training examples
   * only. Let N be the number of training examples. Then
   * <testing_matrix> is a NxM Matrix where M is the number of test
   * examples, and the testing[i][j] is the kernel value between the
   * i'th training example and the j'th test example.  <training>
   * must be N-dimensional and <testing> must be M-dimensional.
   */
  NNExperiment(vector<LabeledIndex> training,
             const KernelMatrix& training_matrix,
             vector<LabeledIndex> testing, const Matrix& testing_matrix);

  virtual ~NNExperiment() { }

  /// NN experiments don't need training, so this simply does nothing.
  virtual void Train() { }

  /// Make predictions for each testing example.
  /**
   * Returns the number of test examples that were correct.
   * When you implement Test(), you must report results via
   * SetPrediction().
   */
  virtual int Test();
};
}  // namespace libpmk_util
#endif  // EXPERIMENT_EXPERIMENT_H
