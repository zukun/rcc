// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// A static class that allows you to retrieve matches from an
// incremental vocabulary tree given a query image. Normalizes scores
// by dividing the number of votes by the sum of the cardinalities of
// the query image and the database image.
//

#ifndef TREES_VOCABULARY_TREE_RETRIEVER_H
#define TREES_VOCABULARY_TREE_RETRIEVER_H

#include <vector>
#include "point_set/point-ref.h"
#include "trees/incremental-vocabulary-tree.h"

using namespace std;
using namespace libpmk;

namespace incremental_vtree {

class VocabularyTreeRetriever {
public:
  /// Returns two vectors: the top <num_results> image IDs, and the
  /// corresponding <num_results> scores. Those vectors must not be
  /// NULL and will be cleared. Will return fewer than <num_results>
  /// if there are not enough images in the tree.
  /**
   * The out_ids and scores are returned in order of increasing score.
   */
  static void Retrieve(const IncrementalVocabularyTree& tree,
                       const vector<PointRef>& image,
                       int num_results,
                       vector<int>* out_ids,
                       vector<double>* out_scores);


  static void Retrieve(const IncrementalVocabularyTree& tree,
                       const PointSet& image,
                       int num_results,
                       vector<int>* out_ids,
                       vector<double>* out_scores);

private:
  /// Disallow construction:
  VocabularyTreeRetriever() { }
};

}  // end namespace incremental_vtree

#endif  // TREES_VOCABULARY_TREE_RETRIEVER_H
