// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include "experiment/eth-selector.h"

using namespace libpmk_util;

ETHSelector::ETHSelector(const vector<int>& labels, int which_test) :
  ExampleSelector(labels), which_test_(which_test) { }

void ETHSelector::SelectExamples() {
  const vector<LabeledIndex>& examples(GetExamples());

  // Include all 5 angles of a single object, given by which_test.
  for (int ii = 0; ii < 400; ++ii) {
    if ((which_test_ * 5 <= ii) &&
        (ii <= (which_test_ * 5) + 4)) {
      AddTestingExample(examples[ii]);
    } else {
      AddTrainingExample(examples[ii]);
    }
  }
}
