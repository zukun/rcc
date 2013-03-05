// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <cfloat>
#include <iostream>
#include <fstream>
#include "pyramids/normalized-uniform-pyramid-maker.h"
#include "util/sparse-vector.h"

namespace libpmk {

NormalizedUniformPyramidMaker::NormalizedUniformPyramidMaker(int num_levels) :
  num_levels_(num_levels) {
  last_vector_map_values_.resize(num_levels_);
  for (int ii = 0; ii < (int)last_vector_map_values_.size(); ++ii) {
    last_vector_map_values_[ii] = 0;
  }

  vector_map_.resize(num_levels_);
}


void NormalizedUniformPyramidMaker::ProcessPoint(
  const Point& feature,
  MultiResolutionHistogram* histogram,
  const vector<double>& min_values,
  const vector<double>& max_values) {
  vector<Bin> bins_to_add;

  // As we go down the levels, we push_back more stuff onto
  // bin_index. This becomes the index for the new bin to add.
  // Remember that the bin_index of a point at a shallow level
  // is a prefix of the bin_index of the same point at any
  // deeper level.
  LargeIndex bin_index;

  // We go backwards; we're starting at the top level (one giant
  // bin).
  Bin* finger = NULL;

  // Number of divisons in each dimension. It starts at 1 for the top
  // level.

  int num_divisions = 1;
  double bin_size = TOP_LEVEL_BIN_SIZE;
  for (int kk = num_levels_ - 1; kk >= 0; --kk) {
    // By convention, kk=0 means the bottom most level (tiny bins).

    // Note that <index> here does NOT refer to a path down the tree.
    // We just use it to represent which bin at the current level
    // this point belongs to. <bin_index> is what accumulates the
    // path down the tree. Later on, we'll convert <index> to an int
    // using GetSparseValue().
    LargeIndex index;
    index.resize(feature.size());


    for (int dd = 0; dd < feature.size(); ++dd) {
      double grid_size = (max_values[dd] - min_values[dd]) / num_divisions;
      // Ordinarily this first line should just be feature[dd].
      // But since translations_ is discretized, we need to
      // do the same to the features on the fly.
      int value = (int)((feature[dd] - min_values[dd]) / grid_size);
      index[dd] = value;
    }

    // Convert the index-value-pair into a Bin. We don't add
    // it yet, but rather collect all of the Bins
    // corresponding to each of the levels for one point. This
    // is because we need to add bins from the root level
    // first. So we keep a vector of them, then add them to
    // the multi-res-histogram in reverse order.

    int depth = (num_levels_ - 1) - kk;

    // Special case: the root bin. The bin_index is supposed
    // to be empty. index will be a bunch of 0s anyway, and
    // GetSparseValue(0, index) will be constant for
    // this case. So push back GetSparseValue() only for
    // the non root bin.
    if (depth > 0) {
      bin_index.push_back(GetSparseValue(kk, index));
    }

    Bin bin(bin_index);
    bin.set_size(bin_size);
    bin.set_count(feature.weight());

    if (finger == NULL) {
      // Should only happen when we're adding a point to the
      // root bin.
      finger = histogram->add_bin(bin);
    } else {
      finger = histogram->add_bin(bin, finger);
    }

    num_divisions *= 2;
    for (int ii = 0; ii < feature.size(); ++ii) {
      bin_size /= 2.0;
    }
  }
}

MultiResolutionHistogram* NormalizedUniformPyramidMaker::MakePyramid(
  const vector<PointRef>& points) {

  MultiResolutionHistogram* histogram = new MultiResolutionHistogram();
  // For each feature, find which bin it's on in the 1st level (root).
  // Then on the 2nd level. Then on the 3rd, etc.
  // Bin descriptor is a LargeIndex, we get one for each level.
  // Convert the LargeIndex into an int, which describes it path
  // down the tree.

  // If there were no features to add, just create one bin at the top
  // with the correct size.
  if ((int)points.size() == 0) {
    double top_size = TOP_LEVEL_BIN_SIZE;

    // Root has empty index
    LargeIndex empty;
    Bin bin_to_add(empty);
    bin_to_add.set_count(0);
    bin_to_add.set_size(top_size);
    histogram->add_bin(bin_to_add);
    return histogram;
  }
  

  // Get the extrema per dimension. These are vectors of size d.
  vector<double> max_values(points[0].point().size(), DBL_MIN);
  vector<double> min_values(points[0].point().size(), DBL_MAX);
  for (int jj = 0; jj < (int)points.size(); ++jj) {
    const Point& point = points[jj].point();
    for (int kk = 0; kk < point.size(); ++kk) {
      if (point[kk] > max_values[kk]) {
        max_values[kk] = point[kk];
      }
      if (point[kk] < min_values[kk]) {
        min_values[kk] = point[kk];
      }
    }
  }
  

  for (int jj = 0; jj < (int)points.size(); ++jj) {
    ProcessPoint(points[jj].point(), histogram, min_values, max_values);
  }

  return histogram;
}


MultiResolutionHistogram* NormalizedUniformPyramidMaker::MakePyramid(
  const PointSet& point_set) {
  MultiResolutionHistogram* histogram = new MultiResolutionHistogram();

  // For each feature, find which bin it's on in the 1st level (root).
  // Then on the 2nd level. Then on the 3rd, etc.
  // Bin descriptor is a LargeIndex, we get one for each level.
  // Convert the LargeIndex into an int, which describes it path
  // down the tree.

  // If there were no features to add, just create one bin at the top
  // with the correct size.
  if (point_set.size() == 0) {
    double top_size = TOP_LEVEL_BIN_SIZE;

    // Root has empty index
    LargeIndex empty;
    Bin bin_to_add(empty);
    bin_to_add.set_count(0);
    bin_to_add.set_size(top_size);
    histogram->add_bin(bin_to_add);
  }

  // Get the extrema per dimension. These are vectors of size d.
  vector<double> max_values(point_set.point_dim(), DBL_MIN);
  vector<double> min_values(point_set.point_dim(), DBL_MAX);
  for (int jj = 0; jj < point_set.size(); ++jj) {
    const Point& point = point_set.point(jj);
    for (int kk = 0; kk < point_set.point_dim(); ++kk) {
      if (point[kk] > max_values[kk]) {
        max_values[kk] = point[kk];
      }
      if (point[kk] < min_values[kk]) {
        min_values[kk] = point[kk];
      }
    }
  }

  for (int jj = 0; jj < point_set.size(); ++jj) {
    ProcessPoint(point_set.point(jj), histogram, min_values, max_values);
  }

  return histogram;
}


int NormalizedUniformPyramidMaker::GetSparseValue(int level,
                                                  const LargeIndex& index) {
  if (vector_map_[level].find(index) != vector_map_[level].end()) {
    return vector_map_[level][index];
  } else {
    int value = ++last_vector_map_values_[level];
    vector_map_[level][index] = value;
    return value;
  }
}

int NormalizedUniformPyramidMaker::num_levels() const {
  return num_levels_;
}

void NormalizedUniformPyramidMaker::WriteToStream(
  ostream& output_stream) const {
  // num levels
  // vector map
  // last vector map values
  assert(output_stream.good());

  output_stream.write((char *)&num_levels_, sizeof(int));


  assert((int)vector_map_.size() == num_levels_);
  for (int ii = 0; ii < num_levels_; ++ii) {
    int size = (int)vector_map_[ii].size();
    output_stream.write((char *)&size, sizeof(int));

    map<LargeIndex, int>::const_iterator iter = vector_map_[ii].begin();
    while (iter != vector_map_[ii].end()) {
      LargeIndex index = iter->first;
      int value = iter->second;

      int index_size = (int)index.size();
      output_stream.write((char *)&index_size, sizeof(int));

      for (int jj = 0; jj < index_size; ++jj) {
        int index_piece = index[jj];
        output_stream.write((char *)&index_piece, sizeof(int));
      }
      output_stream.write((char *)&value, sizeof(int));
      ++iter;
    }
  }

  assert((int)last_vector_map_values_.size() == num_levels_);
  for (int ii = 0; ii < num_levels_; ++ii) {
    int value = last_vector_map_values_[ii];
    output_stream.write((char *)&value, sizeof(int));
  }
}

void NormalizedUniformPyramidMaker::ReadFromStream(istream& input_stream) {
  // num levels
  // vector map
  // last vector map values
  assert(input_stream.good());

  for (int ii = 0; ii < (int)vector_map_.size(); ++ii) {
    vector_map_[ii].clear();
  }
  vector_map_.clear();
  last_vector_map_values_.clear();

  input_stream.read((char *)&num_levels_, sizeof(int));

  last_vector_map_values_.resize(num_levels_);
  vector_map_.resize(num_levels_);

  for (int ii = 0; ii < num_levels_; ++ii) {
    int size;
    input_stream.read((char *)&size, sizeof(int));

    for (int kk = 0; kk < size; ++kk) {
      // Read a LargeIndex
      int index_size;
      LargeIndex index;
      input_stream.read((char *)&index_size, sizeof(int));
      for (int jj = 0; jj < index_size; ++jj) {
        int index_piece;
        input_stream.read((char *)&index_piece, sizeof(int));
        index.push_back(index_piece);
      }

      int value;
      input_stream.read((char *)&value, sizeof(int));
      vector_map_[ii][index] = value;
    }
  }

  for (int ii = 0; ii < num_levels_; ++ii) {
    int value;
    input_stream.read((char *)&value, sizeof(int));
    last_vector_map_values_[ii] = value;
  }
}

void NormalizedUniformPyramidMaker::ReadFromFile(const char* filename) {
  ifstream input_stream(filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

void NormalizedUniformPyramidMaker::WriteToFile(const char* filename) const {
  ofstream output_stream(filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}


}  // namespace libpmk
