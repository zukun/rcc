// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This is an incremental vocabulary tree which reads images one at a
// time. This implementation does not do pruning and does not maintain
// pyramids, so it would be used for retrieval tasks. However, also
// included in this package is simple-pyramid-maker.cc, which can take
// an incremental vocabulary tree and return pyramids based on the
// current structure of the vocabulary tree.
//

#ifndef TREES_INCREMENTAL_VOCABULARY_TREE_H
#define TREES_INCREMENTAL_VOCABULARY_TREE_H

#include <set>
#include "point_set/point-ref.h"
#include "point_set/point.h"
#include "point_set/point-set.h"
#include "point_set/mutable-point-set-list.h"
#include "tree/tree.cc"
#include "trees/visual-word.h"

using namespace std;
using namespace libpmk;

namespace incremental_vtree {

class IncrementalVocabularyTree {
public:
  /// Uses the default parameters to make an empty tree.
  /**
   * Default branch_factor: 6
   * Default max_num_points: 10
   * Default reduction_factor: 5
   * Default recluster_set_size: 600
   *
   * Note that calling ReadFromStream() overrides these.
   */
  IncrementalVocabularyTree();
  IncrementalVocabularyTree(int branch_factor, int max_num_points,
                            double reduction_factor, int recluster_set_size);
  virtual ~IncrementalVocabularyTree();

  /// Returns the total number of nodes in the tree.
  int size() const { return tree_.size(); }

  /// Returns the number of points at the leaves of the tree.
  int GetNumPointsAtLeaves() const;

  /// \brief Returns the number of points at the leaves of the
  /// subtree whose root is the given node.
  int GetNumPointsAtLeaves(const VisualWord* subtree_root) const;

  int GetNumLeaves() const;
  int GetNumLevels() const;
  int GetNumImages() const;

  /// Returns a pointer to the visual word given by this index.
  /**
   * Returns NULL if the node is not found.
   */
  VisualWord* word(int id) { return tree_.node(id); }
  const VisualWord* word(int id) const { return tree_.node(id); }


  /// Return a pointer to the root of the tree.
  VisualWord* root() { return tree_.root(); }
  const VisualWord* root() const { return tree_.root(); }

  VisualWord* add_word(const VisualWord& new_word) {
    return tree_.add_node(new_word);
  }

  /// Insert the points from a new image into the tree.
  /**
   * Inserts a copy of the image into the private database,
   * and also returns an ID corresponding to the newly-added image.
   * These ints will start from 0 (for the very first image) and
   * will increment by 1.
   */
  virtual int InsertImage(const PointSet& image);

  void ReadFromStream(istream& input_stream);
  void WriteToStream(ostream& output_stream) const;

  VisualWord* Find(const Point& point, vector<double>* distances);
  VisualWord* Find(const Point& point);
  const VisualWord* Find(const Point& point, vector<double>* distances) const;
  const VisualWord* Find(const Point& point) const {
    return Find(point, NULL);
  }

  // Get the number of points that were contained in the given image.
  int GetCardinality(int image_id) const;

protected:

  virtual void ReleaseAndRecluster(const set<int>& changed_leaves);

  /// Given a leaf where new points were added and the capacity
  /// constraint was triggered, perform the release proecss.
  /**
   * Returns the root node of all changes (the parent of the
   * highest-level released node). If the root of the tree was
   * released, then this will return NULL. If <out_pool> is not NULL,
   * it will clear it and populate it with the newly released points.
   */
  virtual VisualWord* ReleaseNodes(VisualWord* changed_leaf,
                                   vector<PointRecord>* out_pool);

  virtual void Recluster(const vector<PointRecord>& points,
                         VisualWord* parent);
  virtual void RecursiveRecluster(const vector<PointRecord>& points,
                                  VisualWord* parent);

  int branch_factor_;
  int max_num_points_;
  double reduction_factor_;
  int recluster_set_size_;

  Tree<VisualWord> tree_;
  auto_ptr<MutablePointSetList> point_sets_;
};
}  // end namespace incremental_vtree

#endif  // TREES_INCREMENTAL_VOCABULARY_TREE_H
