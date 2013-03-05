// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <cassert>
#include <algorithm>
#include <iostream>
#include "histograms/bin.h"
#include "histograms/histogram.h"

namespace libpmk {

Histogram::Histogram() : bins_(0), is_sorted_(true) { }

Histogram::~Histogram() {
  for(int ii = 0; ii < (int)bins_.size(); ++ii) {
    delete bins_[ii];
  }
  bins_.clear();
}

Bin* Histogram::add_bin(const Bin& new_bin) {
  int last_index = 0;
  if (!bins_.empty()) {
    last_index = bins_.size() - 1;
  }
  Bin* added = new Bin(new_bin.index());
  added->set_size(new_bin.size());
  added->set_count(new_bin.count());
  bins_.push_back(added);

  if (!bins_.empty()) {
    if (is_sorted_ && (added->index() < bins_[last_index]->index())) {
      is_sorted_ = false;
    }
  }

  return added;
}

void Histogram::SortBins() const {
  if (is_sorted_) {
    return;
  }
  
  sort(bins_.begin(), bins_.end(), Bin::CompareNodes);

  // Consolidate duplicate bins:
  for (vector<Bin*>::iterator iter = bins_.begin();
       iter + 1 != bins_.end(); ) {
    vector<Bin*>::iterator next = iter + 1;
    Bin* current = *iter;
    Bin* nextbin = *next;
    if ((next != bins_.end()) &&
        (nextbin->index() == current->index())) {
      current->set_count(current->count() + nextbin->count());
      double max_size = current->size() > nextbin->size() ?
        current->size() : nextbin->size();
      current->set_size(max_size);
      delete nextbin;
      bins_.erase(next);
    } else {
      ++iter;
    }

  }

  is_sorted_ = true;
}

double Histogram::ComputeIntersection(Histogram* first, Histogram* second) {
  double intersection = 0;
  first->SortBins();
  second->SortBins();

  vector<Bin*>::iterator iter1 = first->bins_.begin();
  vector<Bin*>::iterator iter2 = second->bins_.begin();
  while (iter1 != first->bins_.end() &&
         iter2 != second->bins_.end()) {
    // If the indices are not equal, increment the iterator with the
    // smaller index.
    const LargeIndex& index1 = (*iter1)->index();
    const LargeIndex& index2 = (*iter2)->index();
    if (index1 < index2) {
      ++iter1;
    } else if (index2 < index1) {
      ++iter2;
    } else {
      // If the indices are equal, add to the score and increment both
      // iterators.
      assert((*iter1)->size() == (*iter2)->size());
      double minimum = ((*iter1)->count()) < ((*iter2)->count()) ?
        ((*iter1)->count()) : ((*iter2)->count());
      intersection += ((*iter1)->size()) * minimum;
      ++iter1;
      ++iter2;
    }
  }
  return intersection;
}

double Histogram::ComputeChiSquaredDistance(Histogram* first,
                                            Histogram* second) {
  double score = 0;
  first->SortBins();
  second->SortBins();

  vector<Bin*>::iterator iter1 = first->bins_.begin();
  vector<Bin*>::iterator iter2 = second->bins_.begin();

  while (iter1 != first->bins_.end() &&
         iter2 != second->bins_.end()) {
    // If the indices are not equal, increment the iterator with the
    // smaller index.
    const LargeIndex& index1 = (*iter1)->index();
    const LargeIndex& index2 = (*iter2)->index();
    if (index1 < index2) {
      score += (*iter1)->count();
      ++iter1;
    } else if (index2 > index1) {
      score += (*iter2)->count();
      ++iter2;
    } else {
      // If the indices are equal, add to the score and increment both
      // iterators.
      assert((*iter1)->size() == (*iter2)->size());
      double sum = (*iter1)->count() + (*iter2)->count();
      double difference = (*iter1)->count() - (*iter2)->count();

      // If the denominator is 0, just do nothing; we'll count it as
      // if neither histogram had an entry here.
      if (sum != 0) {
        score += (difference * difference) / sum;
      }
      ++iter1;
      ++iter2;
    }
  }

  while (iter1 != first->bins_.end()) {
    score += (*iter1)->count();
    ++iter1;
  }

  while (iter2 != second->bins_.end()) {
    score += (*iter2)->count();
    ++iter2;
  }

  return score;
}

double Histogram::ComputeSumSquaredDistance(Histogram* first,
                                            Histogram* second) {
  double score = 0;
  first->SortBins();
  second->SortBins();

  vector<Bin*>::iterator iter1 = first->bins_.begin();
  vector<Bin*>::iterator iter2 = second->bins_.begin();
  while (iter1 != first->bins_.end() &&
         iter2 != second->bins_.end()) {
    // If the indices are not equal, increment the iterator with the
    // smaller index.
    const LargeIndex& index1 = (*iter1)->index();
    const LargeIndex& index2 = (*iter2)->index();
    if (index1 < index2) {
      score += ((*iter1)->count() * (*iter1)->count());
      ++iter1;
    } else if (index2 > index1) {
      score += ((*iter2)->count() * (*iter2)->count());
      ++iter2;
    } else {
      // If the indices are equal, add to the score and increment both
      // iterators.
      assert((*iter1)->size() == (*iter2)->size());
      double difference = (*iter1)->count() - (*iter2)->count();
      score += (difference * difference);
      ++iter1;
      ++iter2;
    }
  }

  while (iter1 != first->bins_.end()) {
    score += (*iter1)->count() * (*iter1)->count();
    ++iter1;
  }

  while (iter2 != second->bins_.end()) {
    score += (*iter2)->count() * (*iter2)->count();
    ++iter2;
  }
  return score;
}

// File format:
// (int32_t) num_bins
// Then, num_bins Bins, which are:
//   int32_t index_size
//   (index_size * int32_t) the index
//   double size
//   double count
// The bins are written in sorted order.
void Histogram::WriteSingleHistogramToStream(ostream& output_stream,
                                             Histogram* h) {
  assert(output_stream.good());

  h->SortBins();
  int32_t num_bins = h->size();
  output_stream.write((char *)&num_bins, sizeof(int32_t));

  for (int ii = 0; ii < h->size(); ++ii) {
    Bin* current = h->bins_[ii];
    int32_t index_size = current->index().size();
    double size = current->size();
    double count = current->count();
    output_stream.write((char *)&index_size, sizeof(int32_t));
    for (int ii = 0; ii < index_size; ++ii) {
      int32_t element = current->index()[ii];
      output_stream.write((char *)&element, sizeof(int32_t));
    }
    output_stream.write((char *)&size, sizeof(double));
    output_stream.write((char *)&count, sizeof(double));
  }
}

Histogram* Histogram::ReadSingleHistogramFromStream(istream& input_stream) {
  assert(input_stream.good());

  Histogram* result = new Histogram();
  int32_t num_bins = 0;
  input_stream.read((char *)&num_bins, sizeof(int32_t));

  for (int ii = 0; ii < num_bins; ++ii) {
    int32_t index_size;
    LargeIndex index;
    double size;
    double count;

    input_stream.read((char *)&index_size, sizeof(int32_t));
    for (int jj = 0; jj < index_size; ++jj) {
      int32_t index_element;
      input_stream.read((char *)&index_element, sizeof(int32_t));
      index.push_back(index_element);
    }

    input_stream.read((char *)&size, sizeof(double));
    input_stream.read((char *)&count, sizeof(double));
    Bin temp(index);
    temp.set_size(size);
    temp.set_count(count);
    result->add_bin(temp);
  }
  result->is_sorted_ = true;
  return result;
}

void Histogram::WriteToStream(ostream& output_stream,
                              const vector<Histogram*>& hists) {
  assert(output_stream.good());

  int32_t num_hists = hists.size();
  output_stream.write((char *)&num_hists, sizeof(int32_t));

  for (int ii = 0; ii < (int)hists.size(); ++ii) {
    WriteSingleHistogramToStream(output_stream, hists[ii]);
  }
}

vector<Histogram*> Histogram::ReadFromStream(istream& input_stream) {
  assert(input_stream.good());
  vector<Histogram*> histograms;

  int32_t num_hists;
  input_stream.read((char *)&num_hists, sizeof(int32_t));

  for (int ii = 0; ii < num_hists; ++ii) {
    Histogram* next = ReadSingleHistogramFromStream(input_stream);
    histograms.push_back(next);
  }
  return histograms;
}

int Histogram::size() const {
  SortBins();
  return bins_.size();
}

const Bin* Histogram::bin(int ii) {
  SortBins();
  return bins_.at(ii);
}

const Bin* Histogram::bin(const LargeIndex& index) {
  SortBins();
  return bin(index, 0);
}

const Bin* Histogram::bin(const LargeIndex& index, int finger) {
  assert(finger < size());
  SortBins();
  for (int ii = finger; ii < size(); ++ii) {
    const Bin* bin = bins_[ii];
    if (bin->index() < index) {
      return NULL;
    } else if (bin->index() == index) {
      return bin;
    }
  }
  return NULL;
}

void Histogram::Normalize() {
  double count = 0;
  SortBins();

  for (int jj = 0; jj < (int)bins_.size(); ++jj) {
    count += bins_[jj]->count();
  }

  for (int jj = 0; jj < (int)bins_.size(); ++jj) {
    bins_[jj]->set_count(bins_[jj]->count() / count);
  }
}

}  // namespace libpmk
