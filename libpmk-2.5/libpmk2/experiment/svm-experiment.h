// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef EXPERIMENT_SVM_EXPERIMENT_H
#define EXPERIMENT_SVM_EXPERIMENT_H

#include "experiment/experiment.h"
#include "kernel/kernel-matrix.h"
#include "util/labeled-index.h"
#include "svm/svm.h"

using namespace libpmk;

namespace libpmk_util {

/// Runs an experiment using LIBSVM.
/**
 * This will use a one-vs-all classifier.
 *
 * For more information about LIBSVM, please see<br>
 * Chih-Chung Chang and Chih-Jen Lin, LIBSVM : a library for support
 * vector machines, 2001. Software available at
 * http://www.csie.ntu.edu.tw/~cjlin/libsvm
 */
class SVMExperiment : public Experiment {
public:
  /**
   * <kernel> includes pairwise kernel values for all data (both
   * training and testing). The LabeledIndices in <training> and
   * <testing> specify which row of the kernel to look at.
   */
  SVMExperiment(vector<LabeledIndex> training,
                vector<LabeledIndex> testing,
                const KernelMatrix& kernel,
                double c);

  /**
   * <training_matrix> is a kernel matrix for training examples
   * only. Let N be the number of training examples. Then
   * <testing_matrix> is a NxM Matrix where M is the number of test
   * examples, and the testing[i][j] is the kernel value between the
   * i'th training example and the j'th test example.  <training>
   * must be N-dimensional and <testing> must be M-dimensional.
   */
  SVMExperiment(vector<LabeledIndex> training,
                const KernelMatrix& training_matrix,
                vector<LabeledIndex> testing,
                const Matrix& testing_matrix,
                double c);

  virtual ~SVMExperiment();

  virtual void Train();
  virtual int Test();

  void ReadFromFile(const char* filename);
  void WriteToFile(const char* filename) const;

private:
  struct svm_parameter param_;
  struct svm_model *model_;
};
}  // namespace libpmk_util
#endif  // EXPERIMENT_SVM_EXPERIMENT_H
