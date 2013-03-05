// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of IncrementalVocabularyTree.
//

#include <cfloat>
#include <vector>
#include <set>
#include <list>

#include "incremental-vocabulary-tree.h"

#include "visual-word.h"
#include "tree/tree.h"
#include "util/distance-computer.h"
#include "clustering/k-means-clusterer.h"

using namespace libpmk;

namespace incremental_vtree {

IncrementalVocabularyTree::IncrementalVocabularyTree() :
  branch_factor_(6), max_num_points_(10), reduction_factor_(5),
  recluster_set_size_(600), point_sets_(new MutablePointSetList()) {
  root()->set_data(point_sets_.get());
}

IncrementalVocabularyTree::
IncrementalVocabularyTree(int branch_factor, int max_num_points,
                          double reduction_factor, int recluster_set_size) :
  branch_factor_(branch_factor), max_num_points_(max_num_points),
  reduction_factor_(reduction_factor),
  recluster_set_size_(recluster_set_size),
  point_sets_(new MutablePointSetList()) {
  root()->set_data(point_sets_.get());
}

IncrementalVocabularyTree::~IncrementalVocabularyTree() { }

int IncrementalVocabularyTree::GetNumPointsAtLeaves() const {
  return GetNumPointsAtLeaves(tree_.root());
}

int IncrementalVocabularyTree::GetNumPointsAtLeaves(
  const VisualWord* subtree_root) const {
  return subtree_root->inverted_file().size();
}

VisualWord* IncrementalVocabularyTree::Find(const Point& point,
                                            vector<double>* distances) {
  // TODO(jjl): distance computer should be passed in to constructor
  // of IncrementalVocabularyTree.
  L2DistanceComputer dc;
  VisualWord* current_word = root();
  assert(current_word->has_center());

  if (distances != NULL) {
    distances->push_back(dc.ComputeDistance(point,
                                            current_word->center()));
  }

  while (current_word->has_child()) {
    assert(current_word->has_center());
    double min_distance = DBL_MAX;
    int best_child_id = 0;
    VisualWord* best_child = NULL;

    for (int ii = 0; ii < current_word->child_size(); ++ii) {
      int child_id = current_word->child(ii);
      VisualWord* child = tree_.node(child_id);
      double distance = dc.ComputeDistance(point,
                                           child->center());
      if (distance < min_distance) {
        min_distance = distance;
        best_child = child;
        best_child_id = child_id;
      }
    }

    assert(NULL != best_child);
    current_word = best_child;

    if (distances != NULL){
      distances->push_back(min_distance);
    }
  }

  return current_word;
}

const VisualWord* IncrementalVocabularyTree::Find(
  const Point& point, vector<double>* distances) const {
  // TODO(jjl): distance computer should be passed in to constructor
  // of IncrementalVocabularyTree.
  L2DistanceComputer dc;
  const VisualWord* current_word = root();
  assert(current_word->has_center());

  if (distances != NULL) {
    distances->push_back(dc.ComputeDistance(point,
                                            current_word->center()));
  }

  while (current_word->has_child()) {
    assert(current_word->has_center());
    double min_distance = DBL_MAX;
    int best_child_id = 0;
    const VisualWord* best_child = NULL;

    for (int ii = 0; ii < current_word->child_size(); ++ii) {
      int child_id = current_word->child(ii);
      const VisualWord* child = tree_.node(child_id);
      double distance = dc.ComputeDistance(point,
                                           child->center());
      if (distance < min_distance) {
        min_distance = distance;
        best_child = child;
        best_child_id = child_id;
      }
    }

    assert(NULL != best_child);
    current_word = best_child;

    if (distances != NULL){
      distances->push_back(min_distance);
    }
  }

  return current_word;
}


VisualWord* IncrementalVocabularyTree::Find(const Point& point) {
  return Find(point, NULL);
}

int IncrementalVocabularyTree::InsertImage(const PointSet& image) {
  int image_id = point_sets_->point_set_size();

  // Insert this image into our private database:
  point_sets_->add_point_set(image);

  // Retrieve the PointRefs, with which we will be performing
  // all of the operations.
  vector<PointRef> point_refs;
  point_sets_->GetPointRefs(image_id, &point_refs);
  

  // If this is the first image, that means the root is uninitialized.
  // We should initialize it to contain the centroid of this initial
  // set of points.
  if (point_sets_->point_set_size() == 1) {
    L2DistanceComputer dist_computer;
    VisualWord* root = tree_.root();
    KMeansClusterer centroid_finder(1, 100, dist_computer);
    centroid_finder.Cluster(point_refs);
    root->set_center(centroid_finder.centers()[0]);
  }

  // Keep a list of leaves where a point was added. Rather than
  // pointers to VisualWord*, which is more efficient, we use their
  // integer IDs to refer to them. This is because of the nature of
  // releasing nodes. Suppose there are two changed leaves; when we
  // run the Release process on one of them, it may actually cause the
  // second one to be released as well, in which case the second
  // element in changed_leaves would be invalid.  Thus, we store them
  // as LargeIndices; if something gets released, we can check to see
  // whether it is still valid before trying anything else.
  set<int> changed_leaves;

  cout << "Inserting points... " << flush;
  for (int ii = 0; ii < (int)image.size(); ++ii) {
    vector<double> distances;
    VisualWord* matched_word = Find(image[ii], &distances);

    // Insert it into the changed-leaf list only if this insertion
    // will cause the capacity constraint to be violated.
    if (GetNumPointsAtLeaves(matched_word) >= max_num_points_) {
      changed_leaves.insert(matched_word->id());
    }

    // Trace up the tree, keeping the bin sizes, centers, and inverted
    // files up to date.
    int trace_id = matched_word->id();
    int height = (int)distances.size();
    while (trace_id != TreeNode::kInvalidNodeID) {
      VisualWord* trace = tree_.node(trace_id);
      --height;
      assert(height >= 0);

      // Update the bin size:
      double distance = distances[height];
      if (distance > trace->size()) {
        trace->set_size(distance);
      }

      // Now insert into the inverted file:
      trace->mutable_inverted_file()->add_point(image_id, point_refs[ii]);

      trace_id = trace->parent();
    }
  }

  cout << "done. " << (int)changed_leaves.size() << " leaves changed." << endl;
  ReleaseAndRecluster(changed_leaves);
  return image_id;
}

void IncrementalVocabularyTree::
ReleaseAndRecluster(const set<int>& changed_leaves) {
  for(set<int>::const_iterator iter = changed_leaves.begin();
      iter != changed_leaves.end(); ++iter) {
    int leaf_id = *iter;
    VisualWord* changed_leaf = tree_.node(leaf_id);

    // If the node is still there (i.e., hasn't been clobbered
    // by a previous release process):
    if (changed_leaf != NULL &&
        GetNumPointsAtLeaves(changed_leaf) > max_num_points_) {
      vector<PointRecord> released_points;
      cout << "Releasing... " << flush;
      VisualWord* changed_root = ReleaseNodes(changed_leaf, &released_points);
      cout << "done. Released " << (int)released_points.size()
           << " points." << flush;

      cout << " Reclustering... " << flush;
      Recluster(released_points, changed_root);
      cout << "done." << endl;
    }
  }
}


VisualWord* IncrementalVocabularyTree::ReleaseNodes(
  VisualWord* changed_leaf, vector<PointRecord>* out_pool) {
  assert(out_pool != NULL);
  out_pool->clear();

  // Corner case: the <changed_leaf> is also the root of the tree.
  // Fill out_pool with every point and return NULL.
  if (!changed_leaf->has_parent()) {
    changed_leaf->inverted_file().GetPointRecords(out_pool);
    tree_.DeleteNode(0);
    root()->set_data(point_sets_.get());
    return NULL;
  }

  // Special case: if <changed_leaf> itself violates the
  // recluster_set_size_ constraint, don't release any nodes. Just
  // add all of the points at that leaf into the pool and return
  // the leaf. This indicates that we just do reclustering.
  if (GetNumPointsAtLeaves(changed_leaf) > recluster_set_size_) {
    changed_leaf->inverted_file().GetPointRecords(out_pool);
    return changed_leaf;
  }

  // Algorithm: Starting from <changed_leaf>, trace up the tree until
  // the node we are currently looking at violates the
  // recluster_set_size_ constraint.  Then, we'll look through each
  // child of the resulting root...
  VisualWord* current = changed_leaf;
  VisualWord* previous = current;
  while (current != NULL) {
    if (GetNumPointsAtLeaves(current) > recluster_set_size_) {
      break;
    }
    previous = current;
    current = tree_.node(current->parent());
  }


  // If we went all the way up the tree, then <current> is NULL. This
  // means that we should release the whole tree.
  if (current == NULL) {
    root()->inverted_file().GetPointRecords(out_pool);
    tree_.DeleteNode(0);
    root()->set_data(point_sets_.get());
  } else {
    // Otherwise, check each individual child. Include only the ones
    // that would not violate the recluster_set_size_ constraint,
    // starting from the closest to <previous>.

    // Form a list of all children.
    list<VisualWord*> children;
    list<VisualWord*> children_to_release;

    for (int ii = 0; ii < current->child_size(); ++ii) {
      children.push_back(tree_.node(current->child(ii)));
    }

    // Look at each child in the list, in order of closeness to
    // <previous>.
    L2DistanceComputer dc;
    int num_released_points = 0;
    while (!children.empty()) {
      double smallest_distance = DBL_MAX;
      VisualWord* nearest_child;
      list<VisualWord*>::iterator nearest_child_iter = children.begin();
      for (list<VisualWord*>::iterator iter = children.begin();
           iter != children.end(); ++iter) {
        VisualWord* current_child = *iter;
        double distance = dc.ComputeDistance(current_child->center(),
                                             previous->center());
        if (distance < smallest_distance) {
          smallest_distance = distance;
          nearest_child_iter = iter;
        }

      }

      nearest_child = *nearest_child_iter;
      children.erase(nearest_child_iter);

      if ((num_released_points + GetNumPointsAtLeaves(nearest_child))
          <= recluster_set_size_) {
        num_released_points += GetNumPointsAtLeaves(nearest_child);
        children_to_release.push_back(nearest_child);
      }
    }

    // Delete all of the nodes in <children_to_release> and put their
    // point refs into <out_pool>.
    for (list<VisualWord*>::iterator iter = children_to_release.begin();
         iter != children_to_release.end(); ++iter) {
      VisualWord* child = *iter;
      child->inverted_file().GetPointRecords(out_pool);
      tree_.DeleteNode(child->id());
      root()->set_data(point_sets_.get());
    }
  }
  return current;
}

void IncrementalVocabularyTree::Recluster(const vector<PointRecord>& pool,
                                          VisualWord* parent) {
  L2DistanceComputer dist_computer;
  vector<PointRef> point_refs;
  for (int ii = 0; ii < (int)pool.size(); ++ii) {
    point_refs.push_back(pool[ii].point_ref());
  }

  // Special case: reclustering everything. In this case, simply do 1
  // level of k-means at the top level, and then proceed to
  // recluster recursively.
  if (parent == NULL) {
    VisualWord* root = tree_.root();
    assert(!root->has_child());
    KMeansClusterer centroid_finder(1, 100, dist_computer);
    centroid_finder.Cluster(point_refs);
    root->set_center(centroid_finder.centers()[0]);
    for (int ii = 0; ii < (int)pool.size(); ++ii) {
      InvertedFile* inverted_file = root->mutable_inverted_file();
      inverted_file->add_point(pool[ii].image_id(), point_refs[ii]);
    }
    RecursiveRecluster(pool, root);
    return;
  }

  // The value of K for the first level of clustering is such that
  // K + (# of children <parent> has) = branch_factor_.
  int num_boundary_siblings = parent->child_size();

  int first_k = branch_factor_ - num_boundary_siblings;
  KMeansClusterer top_clusterer(first_k, 100, dist_computer);
  top_clusterer.Cluster(point_refs);
  const PointSet& top_centers(top_clusterer.centers());

  int max_occupancy = (int)(max_num_points_ / reduction_factor_) + 1;
  vector<VisualWord*> added_words;

  // Insert the newly clustered children.
  for (int ii = 0; ii < top_centers.size(); ++ii) {
    VisualWord* added_word = tree_.add_node();
    added_word->set_data(point_sets_.get());
    added_words.push_back(added_word);
    added_word->set_center(top_centers.point(ii));
    parent->add_child(added_word->id());
    added_word->set_parent(parent->id());
  }

  // The clustering may fail if there are too many points that have
  // exactly the same feature value, causing the number of returned
  // clusters to not equal the number want. In such a case, terminate
  // the clustering.
  bool clustering_failed = false;
  if (top_centers.size() != first_k) {
    clustering_failed = true;
  }

  vector<int> counts(top_centers.size());
  // See if any of the children exceed max occupancy. If so,
  // recursively cluster the points.
  for (int ii = 0; ii < top_clusterer.membership_size(); ++ii) {
    ++counts[top_clusterer.membership(ii)];
  }

  for (int ii = 0; ii < (int)counts.size(); ++ii) {
    double max_distance = DBL_MIN;

    // Set the sizes of the bins:
    for (int jj = 0; jj < top_clusterer.membership_size(); ++jj) {
      if (top_clusterer.membership(jj) == ii) {
        double local_distance = dist_computer.ComputeDistance(
          point_refs[jj].point(), added_words[ii]->center());
        if (local_distance > max_distance) {
          max_distance = local_distance;
        }
      }
    }
    added_words[ii]->set_size(max_distance);


    // Add the clustered points belonging to this node to the inverted file.
    vector<PointRecord> next_cluster_points;
    for (int jj = 0; jj < top_clusterer.membership_size(); ++jj) {
      if (top_clusterer.membership(jj) == ii) {
        InvertedFile* inverted_file = added_words[ii]->mutable_inverted_file();
        inverted_file->add_point(pool[jj].image_id(), point_refs[jj]);
        next_cluster_points.push_back(pool[jj]);
      }
    }

    // If after the clustering there is still an over-full node,
    // recurse on that. We'll also add the points to this node's
    // inverted file.
    if ((counts[ii] > max_occupancy) && !clustering_failed) {
      RecursiveRecluster(next_cluster_points, added_words[ii]);
    }
  }
}

void IncrementalVocabularyTree::
RecursiveRecluster(const vector<PointRecord>& points,
                   VisualWord* parent) {
  vector<PointRef> point_refs;
  for (int ii = 0; ii < (int)points.size(); ++ii) {
    point_refs.push_back(points[ii].point_ref());
  }
  L2DistanceComputer dist_computer;
  KMeansClusterer clusterer(branch_factor_, 100, dist_computer);
  clusterer.Cluster(point_refs);
  const PointSet& centers(clusterer.centers());

  int max_occupancy = (int)(max_num_points_ / reduction_factor_) + 1;
  vector<VisualWord*> added_words;

  // Insert the newly clustered children.
  for (int ii = 0; ii < centers.size(); ++ii) {
    VisualWord* added_word = tree_.add_node();
    added_word->set_data(point_sets_.get());
    added_words.push_back(added_word);
    added_word->set_center(centers.point(ii));
    parent->add_child(added_word->id());
    added_word->set_parent(parent->id());
  }

  vector<int> counts(centers.size());
  bool clustering_failed = false;
  if (centers.size() != (int)branch_factor_) {
    clustering_failed = true;
  }

  // See if any of the children exceed maximum occupancy. If so,
  // recursively cluster the points.
  for (int ii = 0; ii < clusterer.membership_size(); ++ii) {
    ++counts[clusterer.membership(ii)];
  }

  for (int ii = 0; ii < (int)counts.size(); ++ii) {
    double max_distance = DBL_MIN;

    // Set the sizes of the bins:
    for (int jj = 0; jj < clusterer.membership_size(); ++jj) {
      if (clusterer.membership(jj) == ii) {
        double local_distance = dist_computer.ComputeDistance(
          point_refs[jj].point(), added_words[ii]->center());
        if (local_distance > max_distance) {
          max_distance = local_distance;
        }
      }
    }
    added_words[ii]->set_size(max_distance);


    // Add the clustered points belonging to this node to the inverted file.
    vector<PointRecord> next_cluster_points;
    for (int jj = 0; jj < clusterer.membership_size(); ++jj) {
      if (clusterer.membership(jj) == ii) {
        InvertedFile* inverted_file = added_words[ii]->mutable_inverted_file();
        inverted_file->add_point(points[jj].image_id(), point_refs[jj]);
        next_cluster_points.push_back(points[jj]);
      }
    }

    // If after the clustering there is still an over-full node,
    // recurse on that. We'll also add the points to this node's
    // inverted file.
    if ((counts[ii] > max_occupancy) && !clustering_failed) {
      RecursiveRecluster(next_cluster_points, added_words[ii]);
    }
  }
}


void IncrementalVocabularyTree::WriteToStream(ostream& output_stream) const {
  assert(output_stream.good());
  int32_t branch = branch_factor_;
  int32_t max_num_points = max_num_points_;
  double reduction_factor = reduction_factor_;
  int32_t recluster = recluster_set_size_;
  output_stream.write((char *)&branch, sizeof(int32_t));
  output_stream.write((char *)&max_num_points, sizeof(int32_t));
  output_stream.write((char *)&reduction_factor, sizeof(double));
  output_stream.write((char *)&recluster, sizeof(int32_t));

  point_sets_->WriteToStream(output_stream);
  tree_.WriteToStream(output_stream);
}

void IncrementalVocabularyTree::ReadFromStream(istream& input_stream) {
  assert(input_stream.good());

  int32_t branch_factor;
  int32_t max_num_points;
  double reduction_factor;
  int32_t recluster_set_size;
  input_stream.read((char *)&branch_factor, sizeof(int32_t));
  input_stream.read((char *)&max_num_points, sizeof(int32_t));
  input_stream.read((char *)&reduction_factor, sizeof(double));
  input_stream.read((char *)&recluster_set_size, sizeof(int32_t));

  branch_factor_ = branch_factor;
  max_num_points_ = max_num_points;
  reduction_factor_ = reduction_factor;
  recluster_set_size_ = recluster_set_size;

  point_sets_.reset(new MutablePointSetList());
  point_sets_->ReadFromStream(input_stream);

  // The tree needs to be read manually here, not from
  // Tree::ReadFromStream. This is because VisualWord depends on
  // knowing the point_sets_ at construction time, and ReadFromStream
  // doesn't let us do that.
  tree_.clear();

  int32_t size = 0;
  input_stream.read((char *)&size, sizeof(int32_t));

  for (int ii = 0; ii < size; ++ii) {
    VisualWord temp;
    temp.set_data(point_sets_.get());
    temp.ReadFromStream(input_stream);

    // The special case is the root. An empty Tree, in this
    // representation, has a root with ID 0. When we read the root
    // from the stream (it will have ID 0), we shouldn't be combining
    // that root with the existing one, which is what add_node would
    // do; we should be replacing it.
    if (temp.id() == 0) {
      VisualWord* tree_root = root();
      tree_root->set_size(temp.size());
      tree_root->set_data(temp.data());
      tree_root->set_center(temp.center());
      tree_root->set_inverted_file(temp.inverted_file());
      tree_root->set_parent(temp.parent());

      if (temp.has_child()) {
        for (int jj = 0; jj < temp.child_size(); ++jj) {
          tree_root->add_child(temp.child(jj));
        }
      }
    } else {
      VisualWord* added_word = add_word(temp);
      if (temp.has_parent()) {
        added_word->set_parent(temp.parent());
      }

      if (temp.has_child()) {
        for (int jj = 0; jj < temp.child_size(); ++jj) {
          added_word->add_child(temp.child(jj));
        }
      }
    }
  }
}

int IncrementalVocabularyTree::GetNumLeaves() const {    
  int count = 0;
  for (Tree<VisualWord>::PreorderIterator iter = tree_.BeginPreorder();
       iter != tree_.EndPreorder(); ++iter) {
    if (!word(*iter)->has_child()) {
      ++count;
    }
  }
  return count;
}


int IncrementalVocabularyTree::GetNumLevels() const {    
  int best_depth = 0;
  for (Tree<VisualWord>::PreorderIterator iter = tree_.BeginPreorder();
       iter != tree_.EndPreorder(); ++iter) {
    if (!word(*iter)->has_child()) {
      int depth = 1;
      int trace_id = *iter;
      const VisualWord* trace = word(trace_id);
      while (trace->has_parent()) {
        ++depth; 
        trace_id = trace->parent();
        trace = word(trace_id);
      }
      if (depth > best_depth) {
        best_depth = depth;
      }
    }
  }
  return best_depth;
}

int IncrementalVocabularyTree::GetCardinality(int image_id) const {
  return point_sets_->point_set(image_id).size();
}

int IncrementalVocabularyTree::GetNumImages() const {
  return point_sets_->point_set_size();
}

}  // end namespace incremental_vtree
