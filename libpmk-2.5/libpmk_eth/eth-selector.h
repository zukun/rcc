// Copyright 2006, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef ETH_SELECTOR_H
#define ETH_SELECTOR_H

#include "experiment/example-selector.h"

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
   ETHSelector(const vector<int>& labels, int which_test) :
      ExampleSelector(labels), which_test_(which_test) { }
   
 protected:
   void SelectExamples() {
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
 private:
   int which_test_;
};
}  // namespace libpmk_util

#endif  // ETH_SELECTOR_H
