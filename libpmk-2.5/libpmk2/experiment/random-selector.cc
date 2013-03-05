// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include "experiment/random-selector.h"

using namespace libpmk_util;

RandomSelector::RandomSelector(const vector<int>& labels,
                               int num_train_per_class) :
  ExampleSelector(labels), num_train_per_class_(num_train_per_class) { }

void RandomSelector::SelectExamples() {
  const vector<LabeledIndex>& examples(GetExamples());
  vector<int> unique_labels = GetUniqueLabels();

  for(vector<int>::iterator iter = unique_labels.begin();
      iter != unique_labels.end(); ++iter) {
    vector<LabeledIndex> this_class(GetInstancesWithLabel(*iter, examples));
    assert((int)this_class.size() >= num_train_per_class_);

    vector<LabeledIndex> training(RandomSample(num_train_per_class_,
                                               this_class));
    vector<LabeledIndex> testing(InvertSelection(training, this_class));

    for (int ii = 0; ii < (int)training.size(); ++ii) {
      AddTrainingExample(training[ii]);
    }

    for (int ii = 0; ii < (int)testing.size(); ++ii) {
      AddTestingExample(testing[ii]);
    }
  }
}
