// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <iostream>
#include "experiment/example-selector.h"

using namespace std;
using namespace libpmk;
using namespace libpmk_util;

class TestSelector : public ExampleSelector {
public:
  TestSelector(const vector<int>& labels) : ExampleSelector(labels) { }
  virtual void SelectExamples() {
    // Let odd indices go to training, evens to testing.
    const vector<LabeledIndex>& indices = GetExamples();
    for (int ii = 0; ii < GetNumExamples(); ++ii) {
      if (ii % 2 == 0) {
        AddTestingExample(indices[ii]);
      } else {
        AddTrainingExample(indices[ii]);
      }
    }
  }

  void TestInvertSelection() {
    // Test an empty inversion (result will be everything);
    vector<LabeledIndex> empty;
    vector<LabeledIndex> empty_invert = InvertSelection(empty);
    assert((int)empty_invert.size() == 10);
    assert(empty_invert == GetExamples());

    // The inversion of everything should be nothing:
    vector<LabeledIndex> full_invert = InvertSelection(GetExamples());
    assert((int)full_invert.size() == 0);

    // The inversion of the odds should be the evens and vice versa
    vector<LabeledIndex> evens = GetTestingExamples();
    vector<LabeledIndex> odds = GetTrainingExamples();
    assert(InvertSelection(evens) == odds);
    assert(InvertSelection(odds) == evens);
  }

  void TestRandomSample() {
    // Any random sample of something empty should be empty..
    vector<LabeledIndex> empty;
    assert((int)RandomSample(5, empty).size() == 0);

    vector<LabeledIndex> everything = RandomSample(1000, GetExamples());
    assert(everything.size() == GetExamples().size());
    assert(everything == GetExamples());

    // Make sure returned stuff is sorted
    vector<LabeledIndex> subset1 = RandomSample(5, GetExamples());
    for (int ii = 0; ii < (int)subset1.size() - 1; ++ii) {
      assert(subset1[ii] < subset1[ii+1]);
    }
    assert((int)subset1.size() == 5);

  }

  void TestGetInstancesWithLabel() {
    assert((int)GetInstancesWithLabel(1, GetExamples()).size() == 4);
    assert((int)GetInstancesWithLabel(2, GetExamples()).size() == 3);
    assert((int)GetInstancesWithLabel(3, GetExamples()).size() == 2);
    assert((int)GetInstancesWithLabel(5, GetExamples()).size() == 1);
    assert((int)GetInstancesWithLabel(4, GetExamples()).size() == 0);

    assert((int)GetInstancesWithLabel(1, GetTestingExamples()).size() == 2);
    assert((int)GetInstancesWithLabel(1, GetTrainingExamples()).size() == 2);
  }

};


void RunAllTests() {
  // The labels are {1 1 1 2 2 2 5 3 3 1}.
  vector<int> labels;
  labels.push_back(1); labels.push_back(1); labels.push_back(1);
  labels.push_back(2); labels.push_back(2); labels.push_back(2);
  labels.push_back(5);
  labels.push_back(3); labels.push_back(3);
  labels.push_back(1);

  TestSelector selector(labels);
  assert(selector.GetNumExamples() == 10);

  selector.TestInvertSelection();
  selector.TestRandomSample();
  selector.TestGetInstancesWithLabel();

  vector<LabeledIndex> testing = selector.GetTestingExamples();
  vector<LabeledIndex> training = selector.GetTrainingExamples();

  for (int ii = 0; ii < 5; ++ii) {
    assert(testing[ii].index == 2*ii);
  }
  for (int ii = 0; ii < 4; ++ii) {
    assert(training[ii].index == 2*ii + 1);
  }

  assert(testing[0].label == 1);
  assert(testing[1].label == 1);
  assert(testing[2].label == 2);
  assert(testing[3].label == 5);
  assert(testing[4].label == 3);

  assert(training[0].label == 1);
  assert(training[1].label == 2);
  assert(training[2].label == 2);
  assert(training[3].label == 3);
}

int main(int argc, char** argv) {
  RunAllTests();
  cout << "*****\nPASS\n*****" << endl;
  return 0;
}
