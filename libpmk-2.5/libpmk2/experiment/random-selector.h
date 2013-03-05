// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Chooses <num_train_per_class> examples randomly, per class, to use as
// training, and uses the remaining examples as testing.

#ifndef EXPERIMENT_RANDOM_SELECTOR_H
#define EXPERIMENT_RANDOM_SELECTOR_H

#include "experiment/example-selector.h"

using namespace libpmk;

namespace libpmk_util {

/// Chooses a random sample of each class to use as testing.
class RandomSelector : public ExampleSelector {
public:
  RandomSelector(const vector<int>& labels, int num_train_per_class);

protected:
  virtual void SelectExamples();

private:
  int num_train_per_class_;
};
}  // namespace libpmk_util
#endif  // EXPERIMENT_RANDOM_SELECTOR_H
