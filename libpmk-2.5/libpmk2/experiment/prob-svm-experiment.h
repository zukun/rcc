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
 * Similar to SVMExperiment, but reports the posterior probabilities
 * of each class for each test example.
 *
 * For more information about LIBSVM, please see<br>
 * Chih-Chung Chang and Chih-Jen Lin, LIBSVM : a library for support
 * vector machines, 2001. Software available at
 * http://www.csie.ntu.edu.tw/~cjlin/libsvm
 */
class ProbSVMExperiment : public Experiment {
public:
  ProbSVMExperiment(vector<LabeledIndex> training,
                    vector<LabeledIndex> testing,
                    const KernelMatrix& kernel,
                    double c);
  virtual ~ProbSVMExperiment();

  virtual void Train();
  virtual int Test();

  double GetProbability(int test_index, int which_class);

private:
  struct svm_parameter param_;
  struct svm_model *model_;

  void SetProbability(int test_index, int which_class, double prob);

  // Vector of length <num_test_examples>.
  // Each vector is a vec of length <num_classes>.
  vector<vector<double> > probabilities_;

};
}  // namespace libpmk_util
#endif  // EXPERIMENT_SVM_EXPERIMENT_H
