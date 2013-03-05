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
#include "pyramids/uniform-pyramid-maker.h"
#include "util/sparse-vector.h"

namespace libpmk {

UniformPyramidMaker::UniformPyramidMaker() : preprocessed_(false) { }

void UniformPyramidMaker::Preprocess(const PointSetList& point_sets,
                                     double finest_side_length,
                                     double side_factor,
                                     int discretize_factor,
                                     bool do_translations,
                                     bool global_translations) {
  assert(point_sets.point_set_size() > 0);
  finest_side_length_ = finest_side_length;
  side_factor_ = side_factor;
  do_translations_ = do_translations;
  global_translations_ = global_translations;
  min_value_ = DBL_MAX;
  max_value_ = DBL_MIN;
  diameter_ = 0;
  discretize_factor_ = discretize_factor;

  // Initialize the min and max values.
  for (int ii = 0; ii < point_sets.point_set_size(); ++ii) {
    for (int jj = 0; jj < point_sets.point_set(ii).size(); ++jj) {
      for (int kk = 0; kk < point_sets.point_set(ii).point_dim(); ++kk) {
        const Point& p = point_sets.point_set(ii).point(jj);
        if(p[kk] > max_value_) {
          max_value_ = p[kk];
        }
        if(p[kk] < min_value_) {
          min_value_ = p[kk];
        }
      }
    }
  }

  int dim = point_sets.point_set(0).point_dim();
  feature_dim_ = dim;

  ComputeHistogramSettings(dim, finest_side_length, side_factor,
                           discretize_factor, do_translations,
                           global_translations);
  preprocessed_ = true;
}

void UniformPyramidMaker::ComputeHistogramSettings(int dim,
                                                   double finest_side_length,
                                                   double side_factor,
                                                   int discretize_factor,
                                                   bool do_translations,
                                                   bool global_translations) {
  // Compute the diameter:
  double factor = pow((double)10, discretize_factor_);
  double max_discrete_value = round((max_value_ - min_value_) * factor) + 1;
  diameter_ = max_discrete_value;
  num_levels_ = (int)round(log(diameter_ / finest_side_length) /
                           log(side_factor)) + 2;
  assert(num_levels_ > 0);

  translations_.clear();
  translations_.resize(num_levels_);
  for (int ii = 0; ii < num_levels_; ++ii) {
    translations_[ii].resize(dim);
  }

  if (do_translations && global_translations) {
    for (int jj = 0; jj < dim; ++jj) {
      double shift = ((double)rand() / RAND_MAX) * diameter_;
      for (int ii = 0; ii < num_levels_; ++ii) {
        translations_[ii][jj] = shift;
      }
    }
  } else if(do_translations) {
    for (int ii = 0; ii < num_levels_; ++ii) {
      factor = pow(side_factor, ii);
      for (int jj = 0; jj < dim; ++jj) {
        translations_[ii][jj] = ((double)rand() / RAND_MAX) *
          (finest_side_length * factor);
      }
    }
  }

  last_vector_map_values_.resize(num_levels_);
  for (int ii = 0; ii < (int)last_vector_map_values_.size(); ++ii) {
    last_vector_map_values_[ii] = 0;
  }

  vector_map_.resize(num_levels_);
}



void UniformPyramidMaker::Preprocess(const vector<PointRef>& points,
                                     double finest_side_length,
                                     double side_factor,
                                     int discretize_factor,
                                     bool do_translations,
                                     bool global_translations) {
  assert((int)points.size() > 0);
  finest_side_length_ = finest_side_length;
  side_factor_ = side_factor;
  do_translations_ = do_translations;
  global_translations_ = global_translations;
  min_value_ = DBL_MAX;
  max_value_ = DBL_MIN;
  diameter_ = 0;
  discretize_factor_ = discretize_factor;

  // Initialize the min and max values.
  for (int ii = 0; ii < (int)points.size(); ++ii) {
    const Point& f(points[ii].point());
    for (int kk = 0; kk < f.size(); ++kk) {
      if(f[kk] > max_value_) {
        max_value_ = f[kk];
      }
      if(f[kk] < min_value_) {
        min_value_ = f[kk];
      }
    }
  }

  int dim = points[0].point().size();
  feature_dim_ = dim;

  ComputeHistogramSettings(dim, finest_side_length, side_factor,
                           discretize_factor, do_translations,
                           global_translations);
  preprocessed_ = true;
}

void UniformPyramidMaker::WriteToStream(ostream& output_stream) const {
  // finest side length
  // side factor
  // discretize factor
  // feature dim
  // do translations
  // global translations
  // min max value
  // diameter
  // num levels
  // translations
  // vector map
  // last vector map values
  assert(output_stream.good());
  assert(preprocessed_);

  output_stream.write((char *)&finest_side_length_, sizeof(double));
  output_stream.write((char *)&side_factor_, sizeof(double));
  output_stream.write((char *)&discretize_factor_, sizeof(int32_t));
  output_stream.write((char *)&feature_dim_, sizeof(int32_t));
  output_stream.write((char *)&do_translations_, sizeof(bool));
  output_stream.write((char *)&global_translations_, sizeof(bool));
  output_stream.write((char *)&min_value_, sizeof(double));
  output_stream.write((char *)&max_value_, sizeof(double));
  output_stream.write((char *)&diameter_, sizeof(double));
  output_stream.write((char *)&num_levels_, sizeof(int));

  assert((int)translations_.size() == num_levels_);
  for (int ii = 0; ii < num_levels_; ++ii) {
    assert((int)translations_[ii].size() == feature_dim_);
    for (int jj = 0; jj < feature_dim_; ++jj) {
      double trans = translations_[ii][jj];
      output_stream.write((char *)&trans, sizeof(double));
    }
  }

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

void UniformPyramidMaker::ReadFromStream(istream& input_stream) {
  // finest side length
  // side factor
  // discretize factor
  // feature dim
  // do translations
  // global translations
  // min max value
  // diameter
  // num levels
  // translations
  // vector map
  // last vector map values
  assert(input_stream.good());

  for (int ii = 0; ii < (int)translations_.size(); ++ii) {
    translations_[ii].clear();
    vector_map_[ii].clear();
  }
  translations_.clear();
  vector_map_.clear();
  last_vector_map_values_.clear();


  input_stream.read((char *)&finest_side_length_, sizeof(double));
  input_stream.read((char *)&side_factor_, sizeof(double));
  input_stream.read((char *)&discretize_factor_, sizeof(int32_t));
  input_stream.read((char *)&feature_dim_, sizeof(int32_t));
  input_stream.read((char *)&do_translations_, sizeof(bool));
  input_stream.read((char *)&global_translations_, sizeof(bool));
  input_stream.read((char *)&min_value_, sizeof(double));
  input_stream.read((char *)&max_value_, sizeof(double));
  input_stream.read((char *)&diameter_, sizeof(double));
  input_stream.read((char *)&num_levels_, sizeof(int));

  translations_.resize(num_levels_);
  for (int ii = 0; ii < num_levels_; ++ii) {
    translations_[ii].resize(feature_dim_);
  }
  last_vector_map_values_.resize(num_levels_);
  vector_map_.resize(num_levels_);


  for (int ii = 0; ii < num_levels_; ++ii) {
    for (int jj = 0; jj < feature_dim_; ++jj) {
      double trans;
      input_stream.read((char *)&trans, sizeof(double));
      translations_[ii][jj] = trans;
    }
  }


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

  preprocessed_ = true;
}

void UniformPyramidMaker::ReadFromFile(const char* filename) {
  ifstream input_stream(filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

void UniformPyramidMaker::WriteToFile(const char* filename) const {
  ofstream output_stream(filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}

void UniformPyramidMaker::ProcessPoint(const Point& feature,
                                       MultiResolutionHistogram* histogram) {
  double factor = pow((double)10, discretize_factor_);
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
  for (int kk = num_levels_ - 1; kk >= 0; --kk) {
    // By convention, kk=0 means the bottom most level (tiny bins).

    // Note that <index> here does NOT refer to a path down the tree.
    // We just use it to represent which bin at the current level
    // this point belongs to. <bin_index> is what accumulates the
    // path down the tree. Later on, we'll convert <index> to an int
    // using GetSparseValue().
    LargeIndex index;
    index.resize(feature_dim_);
    double grid_size = finest_side_length_ * pow(side_factor_, kk);

    for (int dd = 0; dd < feature_dim_; ++dd) {
      // Ordinarily this first line should just be feature[dd].
      // But since translations_ is discretized, we need to
      // do the same to the features on the fly.
      int value = (int) ((round((feature[dd] - min_value_) * factor)
                          - translations_[kk][dd]) /
                         grid_size);
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
    bin.set_size(grid_size);
    bin.set_count(feature.weight());

    if (finger == NULL) {
      // Should only happen when we're adding a point to the
      // root bin.
      finger = histogram->add_bin(bin);
    } else {
      finger = histogram->add_bin(bin, finger);
    }
  }
}

MultiResolutionHistogram* UniformPyramidMaker::MakePyramid(
  const vector<PointRef>& points) {
  assert(preprocessed_);
  MultiResolutionHistogram* histogram = new MultiResolutionHistogram();
  // For each feature, find which bin it's on in the 1st level (root).
  // Then on the 2nd level. Then on the 3rd, etc.
  // Bin descriptor is a LargeIndex, we get one for each level.
  // Convert the LargeIndex into an int, which describes it path
  // down the tree.

  // If there were no features to add, just create one bin at the top
  // with the correct size.
  if ((int)points.size() == 0) {
    double top_size = finest_side_length_ * pow(side_factor_,
                                                num_levels_ - 1);

    // Root has empty index
    LargeIndex empty;
    Bin bin_to_add(empty);
    bin_to_add.set_count(0);
    bin_to_add.set_size(top_size);
    histogram->add_bin(bin_to_add);
  }

  for (int jj = 0; jj < (int)points.size(); ++jj) {
    ProcessPoint(points[jj].point(), histogram);
  }

  return histogram;
}


MultiResolutionHistogram* UniformPyramidMaker::MakePyramid(
  const PointSet& point_set) {
  assert(preprocessed_);
  assert(feature_dim_ == point_set.point_dim());

  MultiResolutionHistogram* histogram = new MultiResolutionHistogram();

  // For each feature, find which bin it's on in the 1st level (root).
  // Then on the 2nd level. Then on the 3rd, etc.
  // Bin descriptor is a LargeIndex, we get one for each level.
  // Convert the LargeIndex into an int, which describes it path
  // down the tree.

  // If there were no features to add, just create one bin at the top
  // with the correct size.
  if (point_set.size() == 0) {
    double top_size = finest_side_length_ * pow(side_factor_,
                                                num_levels_ - 1);

    // Root has empty index
    LargeIndex empty;
    Bin bin_to_add(empty);
    bin_to_add.set_count(0);
    bin_to_add.set_size(top_size);
    histogram->add_bin(bin_to_add);
  }

  for (int jj = 0; jj < point_set.size(); ++jj) {
    ProcessPoint(point_set.point(jj), histogram);
  }

  return histogram;
}


int UniformPyramidMaker::GetSparseValue(int level, const LargeIndex& index) {
  if (vector_map_[level].find(index) != vector_map_[level].end()) {
    return vector_map_[level][index];
  } else {
    int value = ++last_vector_map_values_[level];
    vector_map_[level][index] = value;
    return value;
  }
}

int UniformPyramidMaker::num_levels() const {
  assert(preprocessed_);
  return num_levels_;
}

}  // namespace libpmk
