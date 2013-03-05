// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: Tom Yeh  (tomyeh@mit.edu)
//         John Lee (jjl@mit.edu)
//
// A visual word is a node in a vocabulary tree. It stores a "center"
// (not computed or maintained; must be set manually) as well as an
// InvertedFile, which is a record of all of the points and where they
// came from.
//

#ifndef TREES_VISUAL_WORD_H
#define TREES_VISUAL_WORD_H

#include <assert.h>
#include <memory>
#include <iostream>
#include "tree/tree-node.h"
#include "point_set/point.h"
#include "point_set/point-set-list.h"
#include "inverted-file.h"

using namespace libpmk;

namespace incremental_vtree {

class VisualWord : public TreeNode {
public:
  /// Initializes empty word with invalid node ID.
  VisualWord();

  /// Copies entire word, including node ID.
  VisualWord(const VisualWord& word);

  /// Copies entire word, but uses the given node ID.
  VisualWord(const VisualWord& word, int node_id);

  /// Empty word with the given ID.
  VisualWord(int node_id);

  // The center point is an optional data member. Since it is not
  // guaranteed to exist, clients should call has_center() first.
  const Point& center() const { return *(center_.get()); }
  Point* mutable_center() { return center_.get(); }
  void set_center(const Point& point) { center_.reset(new Point(point)); }
  bool has_center() const { return center_.get() != NULL;  }

  void set_data(const PointSetList* data) { data_ = data; }
  const PointSetList* data() const { return data_; }
  bool has_data() const { return data_ != NULL; }

  // The inverted file is not an optional data member. It will always
  // exist.
  const InvertedFile& inverted_file() const { return *(inverted_file_.get()); }
  InvertedFile* mutable_inverted_file() { return inverted_file_.get(); }
  void set_inverted_file(const InvertedFile& f) {
    inverted_file_.reset(new InvertedFile(f));
  }

  double size() const { return size_; }
  void set_size(double size) { size_ = size; }

  /// Tests for same center and same size.
  /**
   * Does not require the node IDs or InvertedFiles to be equal.
   * If this VisualWord has has_center()==false, then <other> must
   * also satisfy that.
   */
  bool operator==(const VisualWord& other) const;

  virtual void ReadData(istream& input_stream);

  /**
   * Output format:<ul>
   * <li> (double) size
   * <li> (int32_t) d, the dim of the center point (0 if no center)
   * <li> (Point = d*int32_t) the center point
   * <li> (InvertedFile) the inverted file
   * </ul>
   *
   * This does not write the inverted files to disk.
   */
  virtual void WriteData(ostream& output_stream) const;

  // Combining currently does nothing.
  virtual void Combine(const TreeNode& other) { assert(false); }

private:
  double size_;

  // This visual word does not own this pointer. It is merely used
  // as a reference for the InvertedFiles.
  const PointSetList* data_;

  auto_ptr<Point> center_;
  auto_ptr<InvertedFile> inverted_file_;
};
}  // end namespace incremental_vtree

#endif  // TREES_VISUAL_WORD_H
