// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef EXPERIMENT_EXAMPLE_SELECTOR_H
#define EXPERIMENT_EXAMPLE_SELECTOR_H

#include <vector>
#include "util/labeled-index.h"

using namespace std;
using namespace libpmk;

namespace libpmk_util {

/// Splits a list of labels into a training and test set.
/**
 * ExampleSelector does not specify any particular method of picking a
 * training and test set, nor does it care about the data itself. The
 * only input is a vector of labels, where each element in the vector
 * corresponds to one image (or other data point).
 *
 * To use ExampleSelector, simply subclass it and implement
 * SelectExamples(). Within this function, designate training and
 * testing examples by using AddTrainingExample() and
 * AddTestingExample(). There are a number of protected member
 * functions at your disposal that could help make things easier.
 */
class ExampleSelector {
public:
  ExampleSelector(const vector<int>& labels);
  virtual ~ExampleSelector() { }

  /// \brief Returns a vector of LabeledIndices of training instances
  /// sorted by index.
  vector<LabeledIndex> GetTrainingExamples();

  /// \brief Returns a vector of LabeledIndices of testing instances
  /// sorted by index.
  vector<LabeledIndex> GetTestingExamples();

  /// \brief Returns a vector containing all the unique labels
  /// seen. Sorting is NOT guaranteed.
  vector<int> GetUniqueLabels();

  /// Returns the total number of examples there are.
  int GetNumExamples() const;

protected:
  /// \brief Get all of the examples (in the same order as given when
  /// constructed)
  const vector<LabeledIndex>& GetExamples() const;

  /// Returns all instances are NOT in <selection>.
  /**
   * <selection> must be sorted by index. The output will be sorted
   * by index.
   */
  vector<LabeledIndex>
  InvertSelection(const vector<LabeledIndex>& selection) const;

  /// Returns all instances in <superset> that are NOT in <selection>
  /**
   * <selection> and <superset> must be sorted by index. The output
   * will be sorted by index.
   */
  vector<LabeledIndex>
  InvertSelection(const vector<LabeledIndex>& selection,
                  const vector<LabeledIndex>& superset) const;


  /// Return a random sample of size <n> of <selection>.
  /**
   * <selection> must be sorted by index. If the size of <selection>
   * is less than n, this function will return a vector of size
   * selection.size(). Output is sorted by index.
   */
  vector<LabeledIndex>
  RandomSample(int n, const vector<LabeledIndex>& selection) const;

  /// Gets all elements in <selection> with the given <label>.
  /**
   * <selection> must be sorted. The output will be sorted.
   */
  vector<LabeledIndex>
  GetInstancesWithLabel(int label,
                        const vector<LabeledIndex>& selection) const;

  /// Add a LabeledIndex to the list of designated training examples.
  void AddTrainingExample(LabeledIndex index);

  /// Add a LabeledIndex to the list of designated testing examples.
  void AddTestingExample(LabeledIndex index);

  /// \brief Responsible for calling AddTrainingExample and
  /// AddTestingExample to generate the lists.
  virtual void SelectExamples() = 0;

private:
  // The internal function that manages the state of whether SelectExamples
  // has already been called and sorts the vectors.
  void LoadExamples();
  vector<LabeledIndex> labels_;
  vector<LabeledIndex> training_;
  vector<LabeledIndex> testing_;
  vector<int> unique_labels_;
  bool selected_;
};
}  // namespace libpmk_util
#endif  // EXPERIMENT_EXAMPLE_SELECTOR_H
