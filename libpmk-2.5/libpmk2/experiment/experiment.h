// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef EXPERIMENT_EXPERIMENT_H
#define EXPERIMENT_EXPERIMENT_H

#include <vector>
#include "kernel/matrix.h"
#include "kernel/kernel-matrix.h"
#include "util/labeled-index.h"

using namespace std;

using namespace libpmk;
namespace libpmk_util {

/// Encapsulates training/testing of any method involving a kernel.
/**
 * All that you need to implement are Train() and Test(). When
 * implementing Test(), the way you report a new prediction is via
 * SetPrediction(). Make sure to call SetPrediction() on each
 * testing example.
 */
class Experiment {
public:
  /**
   * <kernel> includes pairwise kernel values for all data (both
   * training and testing). The LabeledIndices in <training> and
   * <testing> specify which row of the kernel to look at.
   */
  Experiment(vector<LabeledIndex> training,
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
  Experiment(vector<LabeledIndex> training,
             const KernelMatrix& training_matrix,
             vector<LabeledIndex> testing, const Matrix& testing_matrix);

  virtual ~Experiment() { }

  /// Train a model (if applicable)
  virtual void Train() = 0;

  /// Make predictions for each testing example.
  /**
   * Returns the number of test examples that were correct.
   * When you implement Test(), you must report results via
   * SetPrediction().
   */
  virtual int Test() = 0;


  /// Returns the predicted value of the <test_index>th test example.
  /**
   * Can only call this after Test() is called.
   */
  int GetPrediction(int test_index) const;

  /// \brief Get the total number of testing examples that were
  /// correctly classified.
  int GetNumCorrect() const;

  /// \brief Get the number of testing examples that had label
  /// <label> that were also correctly classified.
  int GetNumCorrect(int label) const;

  /// Get the total number of test examples.
  int GetNumTestExamples() const;

  /// Get the number of text examples with label <label>.
  int GetNumTestExamples(int label) const;

  /// Same as GetNumCorrect() / GetNumTestExamples().
  double GetAccuracy() const;

  /// Same as GetNumCorrect(label) / GetNumTestExamples(label).
  double GetAccuracy(int label) const;

protected:
  vector<LabeledIndex> training_;
  vector<LabeledIndex> testing_;

  /// Get a kernel value (wrapper for KernelMatrix)
  double GetKernelValue(int row, int col) const;

  /// Get the kernel value corresponding to the given LabeledIndices.
  double GetKernelValue(const LabeledIndex& row,
                        const LabeledIndex& col) const;

  /**
   * Call this to tell Experiment's internals that the test example
   * at <test_index> was classified as <prediction>.
   */
  void SetPrediction(int test_index, int prediction);

private:
  KernelMatrix kernel_;
  vector<int> predictions_;
};
}  // namespace libpmk_util
#endif  // EXPERIMENT_EXPERIMENT_H
