// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of VocabularyTreeRetriever. See
// vocabulary-tree-retriever.h for a description of how scores are
// computed.
//

#include <map>
#include <vector>

#include "trees/vocabulary-tree-retriever.h"

#include "point_set/point-ref.h"
#include "trees/incremental-vocabulary-tree.h"
#include "trees/inverted-file.h"
#include "trees/visual-word.h"

namespace incremental_vtree {

void VocabularyTreeRetriever::Retrieve(const IncrementalVocabularyTree& tree,
                                       const vector<PointRef>& image,
                                       int num_results,
                                       vector<int>* out_ids,
                                       vector<double>* out_scores) {
  assert(out_ids != NULL);
  assert(out_scores != NULL);
  out_ids->clear();
  out_scores->clear();

  // Maps image ID -> score.
  map<int, double> scores;

  for (int ii = 0; ii < (int)image.size(); ++ii) {
    const PointRef& point = image[ii];
    const VisualWord* voting_node = tree.Find(point.point());
    const InvertedFile& file = voting_node->inverted_file();

    vector<PointRecord> point_records;
    file.GetPointRecords(&point_records);
    for (int jj = 0; jj < (int)point_records.size(); ++jj) {
      int image_id = point_records[jj].image_id();
      scores[image_id] += 1;
    }
  }

  for (map<int, double>::iterator iter = scores.begin();
       iter != scores.end(); ++iter) {
    int card = tree.GetCardinality(iter->first);
    iter->second = (double)iter->second / (card + (int)image.size());
  }

  if ((int)scores.size() < num_results) {
    num_results = scores.size();
  }

  while ((int)out_ids->size() != num_results) {
    // Get the maximum score from the map, and insert it into
    // the vectors. Remove it from the map when done.
    map<int, double>::iterator best_iter = scores.begin();
    for (map<int, double>::iterator iter = scores.begin();
         iter != scores.end(); ++iter) {
      if (iter->second > best_iter->second) {
        best_iter = iter;
      }
    }

    out_ids->push_back(best_iter->first);
    out_scores->push_back(best_iter->second);
    scores.erase(best_iter);
  }
}



void VocabularyTreeRetriever::Retrieve(const IncrementalVocabularyTree& tree,
                                       const PointSet& image,
                                       int num_results,
                                       vector<int>* out_ids,
                                       vector<double>* out_scores) {
  assert(out_ids != NULL);
  assert(out_scores != NULL);
  out_ids->clear();
  out_scores->clear();

  // Maps image ID -> score.
  map<int, double> scores;

  for (int ii = 0; ii < (int)image.size(); ++ii) {
    const Point& point = image[ii];
    const VisualWord* voting_node = tree.Find(point);
    const InvertedFile& file = voting_node->inverted_file();

    vector<PointRecord> point_records;
    file.GetPointRecords(&point_records);
    for (int jj = 0; jj < (int)point_records.size(); ++jj) {
      int image_id = point_records[jj].image_id();
      scores[image_id] += 1;
    }
  }

  for (map<int, double>::iterator iter = scores.begin();
       iter != scores.end(); ++iter) {
    int card = tree.GetCardinality(iter->first);
    iter->second = (double)iter->second / (card + (int)image.size());
  }

  if ((int)scores.size() < num_results) {
    num_results = scores.size();
  }

  while ((int)out_ids->size() != num_results) {
    // Get the maximum score from the map, and insert it into
    // the vectors. Remove it from the map when done.
    map<int, double>::iterator best_iter = scores.begin();
    for (map<int, double>::iterator iter = scores.begin();
         iter != scores.end(); ++iter) {
      if (iter->second > best_iter->second) {
        best_iter = iter;
      }
    }

    out_ids->push_back(best_iter->first);
    out_scores->push_back(best_iter->second);
    scores.erase(best_iter);
  }
}
}  // end namespace incremental_vtree
