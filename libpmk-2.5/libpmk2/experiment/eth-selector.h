// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef EXPERIMENT_ETH_SELECTOR_H
#define EXPERIMENT_ETH_SELECTOR_H

#include "experiment/example-selector.h"

using namespace libpmk;

namespace libpmk_util {

/// (Experiment-specific) Picks examples for a test on ETH-80.
class ETHSelector : public ExampleSelector {
public:
  /**
   * There are 80 different tests to run. The one you want is
   * specified by <which_test>. The test involves including all 5
   * images (viewpoints) of one object as testing examples, and
   * everything else as training. There are 80 objects, so there
   * are 80 different tests to run.
   */
  ETHSelector(const vector<int>& labels, int which_test);

protected:
  virtual void SelectExamples();

private:
  int which_test_;
};
}  // namespace libpmk_util

#endif  // EXPERIMENT_ETH_SELECTOR_H
