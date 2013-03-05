// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <algorithm>
#include <vector>
#include "experiment/example-selector.h"

using namespace std;

using namespace libpmk_util;

ExampleSelector::ExampleSelector(const vector<int>& labels) :
  selected_(false) {
  for (int ii = 0; ii < (int)labels.size(); ++ii) {
    LabeledIndex labeled_index;
    labeled_index.index = ii;
    labeled_index.label = labels[ii];
    labels_.push_back(labeled_index);

    // If it's a new label we haven't seen, add it to the unique labels list.
    if (find(unique_labels_.begin(),
             unique_labels_.end(), labels[ii]) == unique_labels_.end()) {
      unique_labels_.push_back(labels[ii]);
    }
  }
}

void ExampleSelector::LoadExamples() {
  if (!selected_) {
    SelectExamples();
    selected_ = true;
    sort(training_.begin(), training_.end());
    sort(testing_.begin(), testing_.end());
  }
}

vector<int> ExampleSelector::GetUniqueLabels() {
  return unique_labels_;
}

int ExampleSelector::GetNumExamples() const {
  return labels_.size();
}

vector<LabeledIndex> ExampleSelector::GetTrainingExamples() {
  LoadExamples();
  return training_;
}

vector<LabeledIndex> ExampleSelector::GetTestingExamples() {
  LoadExamples();
  return testing_;
}

vector<LabeledIndex> ExampleSelector::
InvertSelection(const vector<LabeledIndex>& selection) const {
  vector<LabeledIndex> sample;
  int jj = 0;
  for (int ii = 0; ii < (int)labels_.size(); ++ii) {
    if (jj < (int)selection.size() && ii == selection[jj].index) {
      ++jj;
    } else {
      sample.push_back(labels_[ii]);
    }
  }
  return sample;
}

vector<LabeledIndex> ExampleSelector::
InvertSelection(const vector<LabeledIndex>& selection,
                const vector<LabeledIndex>& superset) const {
  vector<LabeledIndex> sample;
  int jj = 0;
  for (int ii = 0; ii < (int)superset.size(); ++ii) {
    if (jj < (int)selection.size() &&
        superset[ii].index == selection[jj].index) {
      ++jj;
    } else {
      sample.push_back(superset[ii]);
    }
  }
  return sample;
}

vector<LabeledIndex> ExampleSelector::
RandomSample(int n, const vector<LabeledIndex>& selection) const {
  vector<LabeledIndex> cache;

  // To create the random sample:
  // 1. Keep a cache, with max size n.
  // 2. Put the first n elements in <selection> into the cache.
  // 3. On the kth element (k > n) in selection, choose whether to
  //    put it in the cache with probability 1/k.
  // 4. Choose a random element already in the cache to kick out.

  if ((int)selection.size() <= n) {
    for (int ii = 0; ii < (int)selection.size(); ++ii) {
      cache.push_back(selection[ii]);
    }
  } else {
    for (int ii = 0; ii < n; ++ii) {
      cache.push_back(selection[ii]);
    }

    // We unroll this loop from the previous one for readability..
    for (int ii = n; ii < (int)selection.size(); ++ii) {
      if ((1.0 / (ii-1)) > drand48()) {
        int kick_out = rand() % n;
        cache[kick_out] = selection[ii];
      }
    }
  }

  // Sort the cache, since the randomized eviction process will
  // not leave things sorted.
  sort(cache.begin(), cache.end());
  return cache;
}

vector<LabeledIndex> ExampleSelector::
GetInstancesWithLabel(int label, const vector<LabeledIndex>& selection) const {
  vector<LabeledIndex> sample;
  for (int ii = 0; ii < (int)selection.size(); ++ii) {
    if (selection[ii].label == label) {
      sample.push_back(selection[ii]);
    }
  }
  return sample;
}

void ExampleSelector::AddTrainingExample(LabeledIndex index) {
  training_.push_back(index);
}

void ExampleSelector::AddTestingExample(LabeledIndex index) {
  testing_.push_back(index);
}

const vector<LabeledIndex>& ExampleSelector::GetExamples() const {
  return labels_;
}
