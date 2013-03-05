// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: Tom Yeh  (tomyeh@mit.edu)
//         John Lee (jjl@mit.edu)
//
// Implementation of VisualWord.

#include <iostream>
#include <stdint.h>

#include "visual-word.h"

#include "inverted-file.h"
#include "point_set/point.h"
#include "point_set/point-set-list.h"
#include "tree/tree-node.h"

using namespace libpmk;

namespace incremental_vtree {

VisualWord::VisualWord() :
  TreeNode(), size_(0), data_(NULL), center_(NULL),
  inverted_file_(new InvertedFile()) { }

VisualWord::VisualWord(int node_id) :
  TreeNode(node_id), size_(0), data_(NULL), center_(NULL),
  inverted_file_(new InvertedFile()) { }

VisualWord::VisualWord(const VisualWord& word) :
  TreeNode(word.id()), size_(word.size_), data_(word.data_),
  inverted_file_(new InvertedFile(word.inverted_file())) {
  if (word.has_center()) {
    center_.reset(new Point(word.center()));
  }
}

VisualWord::VisualWord(const VisualWord& word, int node_id) :
  TreeNode(node_id), size_(word.size_), data_(word.data_),
  inverted_file_(new InvertedFile(word.inverted_file())) {
  if (word.has_center()) {
    center_.reset(new Point(word.center()));
  }
}


bool VisualWord::operator==(const VisualWord& other) const {
  if ((has_center() && !other.has_center()) ||
      (!has_center() && other.has_center())) {
    return false;
  }

  if (has_center()) {
    if (center_->size() != other.center_->size()) {
      return false;
    }

    for (int ii = 0; ii < center_->size(); ++ii) {
      if (center_->feature(ii) != other.center_->feature(ii)) {
        return false;
      }
    }
  }

  if (size_ != other.size_) {
    return false;
  }

  return true;
}

void VisualWord::ReadData(istream& input_stream) {
  input_stream.read((char *)&size_, sizeof(double));

  int32_t dim;
  input_stream.read((char *)&dim, sizeof(int32_t));
  center_.reset(new Point(dim));
  center_->ReadFromStream(input_stream);

  InvertedFile* inverted_file = new InvertedFile();
  inverted_file->ReadFromStream(input_stream, data_);
  inverted_file_.reset(inverted_file);
}

void VisualWord::WriteData(ostream& output_stream) const {
  assert (inverted_file_.get() != NULL);

  output_stream.write((char *)&size_, sizeof(double));

  int32_t dim = 0;
  if (has_center()) {
    dim = center_->size();
  }

  output_stream.write((char *)&dim, sizeof(int32_t));

  if (has_center()) {
    center_->WriteToStream(output_stream);
  }

  inverted_file_->WriteToStream(output_stream);
}
}  // end namespace incremental_vtree
