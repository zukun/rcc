// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <sstream>
#include "experiment/svm-experiment.h"
#include "experiment/experiment.h"
#include "kernel/kernel-matrix.h"
#include "util/labeled-index.h"
#include "svm/svm.h"

namespace libpmk_util {

SVMExperiment::SVMExperiment(vector<LabeledIndex> training,
                             vector<LabeledIndex> testing,
                             const KernelMatrix& kernel,
                             double c) :
  Experiment(training, testing, kernel) {
  // Default values -- these don't really matter to us
  // since we're using a custom kernel as input into libsvm.
  param_.degree = 3;
  param_.gamma = 0;
  param_.coef0 = 0;
  param_.nu = 0.5;
  param_.cache_size = 40;
  param_.eps = 1e-3;
  param_.p = 0.1;
  param_.shrinking = 1;
  param_.probability = 0;
  param_.nr_weight = 0;
  param_.weight_label = NULL;
  param_.weight = NULL;

  // These are the params that are important:
  param_.svm_type = C_SVC;
  param_.multiclass_type = ONE_ALL;
  param_.kernel_type = MATRIX;
  param_.C = c;

  model_ = NULL;
}

// TODO(jjl): remove this ugly code duplication
SVMExperiment::SVMExperiment(vector<LabeledIndex> training,
                             const KernelMatrix& training_matrix,
                             vector<LabeledIndex> testing,
                             const Matrix& testing_matrix,
                             double c) :
  Experiment(training, training_matrix, testing, testing_matrix) {
  // Default values -- these don't really matter to us
  // since we're using a custom kernel as input into libsvm.
  param_.degree = 3;
  param_.gamma = 0;
  param_.coef0 = 0;
  param_.nu = 0.5;
  param_.cache_size = 40;
  param_.eps = 1e-3;
  param_.p = 0.1;
  param_.shrinking = 1;
  param_.probability = 0;
  param_.nr_weight = 0;
  param_.weight_label = NULL;
  param_.weight = NULL;

  // These are the params that are important:
  param_.svm_type = C_SVC;
  param_.multiclass_type = ONE_ALL;
  param_.kernel_type = MATRIX;
  param_.C = c;

  model_ = NULL;
}

SVMExperiment::~SVMExperiment() {
  if (model_) {
    svm_destroy_model(model_);
  }
  svm_destroy_param(&param_);
}

void SVMExperiment::Train() {
  struct svm_problem prob;
  struct svm_node *x_space;
  prob.l = training_.size();
  int elements = (training_.size() * training_.size()) + prob.l;

  prob.y = (double *)malloc(prob.l * sizeof(double));
  prob.x = (struct svm_node **)malloc(prob.l  * sizeof(struct svm_node *));
  x_space = (struct svm_node *)malloc((elements + prob.l) *
                                      sizeof(struct svm_node));

  int max_index = 0;
  int jj = 0;

  for (int ii = 0; ii < prob.l; ++ii) {
    prob.x[ii] = &x_space[jj];
    prob.y[ii] = training_[ii].label;

    x_space[jj].index = 1;
    x_space[jj].value = ii + 1;
    ++jj;

    for (int kk = 0; kk < (int)training_.size(); ++kk) {
      x_space[jj].index = kk + 1;
      x_space[jj].value = GetKernelValue(training_[ii], training_[kk]);
      ++jj;
    }

    if (jj >= 1 && x_space[jj-1].index > max_index) {
      max_index = x_space[jj-1].index;
    }
    x_space[jj++].index = -1;
  }

  if (param_.gamma == 0) {
    param_.gamma = 1.0 / max_index;
  }

  const char* error_msg;
  error_msg = svm_check_parameter(&prob, &param_);
  assert(!error_msg);

  if (param_.svm_type == C_SVC || param_.svm_type == NU_SVC) {
    prob.nr_binary = error_correcting_code(param_.multiclass_type,
                                           svm_find_nr_class(&prob),
                                           prob.I);
  }
  model_ = svm_train(&prob, &param_);

  // For some reason, libsvm doesn't like it when you use
  // the same model struct to train and to test. to get correct
  // results, you have to save the model to a file, then re-load it.
  stringstream stream;
  stream << "/tmp/svm_model_" << getpid();
  string model_file_name = stream.str();

  svm_save_model(model_file_name.c_str(), model_);
  svm_destroy_model(model_);
  model_ = svm_load_model(model_file_name.c_str());
  unlink(model_file_name.c_str());
  free(prob.y);
  free(prob.x);
  free(x_space);
}

int SVMExperiment::Test() {
  assert(model_);
  int correct = 0;
  int total = 0;
  double error = 0;
  double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;

  int xsize = 1 + training_.size();
  struct svm_node *x = (struct svm_node *)malloc(xsize *
                                                 sizeof(struct svm_node));
  for (int ii = 0; ii < (int)testing_.size(); ++ii) {
    double target, v;
    target = static_cast<double>(testing_[ii].label);
    for (int jj = 0; jj < (int)training_.size(); ++jj) {
      x[jj].index = jj + 1;
      x[jj].value = GetKernelValue(testing_[ii], training_[jj]);
    }

    x[xsize - 1].index = -1;
    x[xsize - 1].value = 0;

    v = svm_predict(model_, x);
    SetPrediction(ii, static_cast<int>(v));
    if (v == target) {
      ++correct;
    }

    error += (v - target) * (v - target);
    sumv  += v;
    sumy  += target;
    sumvv += v * v;
    sumyy += target * target;
    sumvy += v * target;
    ++total;
  }
  free(x);
  return correct;
}


void SVMExperiment::ReadFromFile(const char* filename) {
  svm_destroy_model(model_);
  model_ = svm_load_model(filename);
}

void SVMExperiment::WriteToFile(const char* filename) const {
  svm_save_model(filename, model_);
}

}  // end namespace libpmk_util
