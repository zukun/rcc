// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <cfloat>
#include <iostream>
#include <vector>
#include <fstream>
#include "clustering/hierarchical-clusterer.h"
#include "clustering/k-means-clusterer.h"
#include "point_set/point-ref.h"
#include "util/sparse-vector.h"
#include "util/distance-computer.h"
#include "tree/point-tree-node.h"
#include "tree/tree.cc"

using namespace std;

namespace libpmk {

const int HierarchicalClusterer::MAX_ITERATIONS(100);

HierarchicalClusterer::HierarchicalClusterer() : done_(false) { }

const Tree<PointTreeNode>& HierarchicalClusterer::centers() const {
  assert(done_);
  return centers_;
}

int HierarchicalClusterer::membership(int index) const {
  assert(done_);
  return membership_[index];
}

void HierarchicalClusterer::IdentifyMemberIDPath(int index,
                                                 list<int>* out) const {
  assert(done_);
  assert(NULL != out);
  out->clear();

  int node_id = membership_[index];

  // Start from the leaf, and follow parent pointers up to the root.
  while (node_id != TreeNode::kInvalidNodeID) {
    out->push_front(node_id);
    const PointTreeNode* node = centers_.node(node_id);
    node_id = node->parent();
  }
}

void HierarchicalClusterer::IdentifyMemberTreePath(int index,
                                                   list<int>* out) const {
  assert(done_);
  assert(NULL != out);
  out->clear();

  int node_id = membership_[index];
  const PointTreeNode* node = centers_.node(node_id);

  // Start from the leaf, and follow parent pointers up to the root.
  while (node->has_parent()) {

    // Go to the parent, and find which child the current node is.
    int parent_id = node->parent();
    const PointTreeNode* parent = centers_.node(parent_id);

    bool found = false;
    for (int ii = 0; ii < parent->child_size(); ++ii) {
      if (parent->child(ii) == node_id) {
        found = true;
        out->push_front(ii);
        break;
      }
    }
    assert(found);
    node = parent;
    node_id = parent_id;
  }
}


void HierarchicalClusterer::
Cluster(int num_levels, int branch_factor, const vector<PointRef>& all_points,
        const DistanceComputer& distance_computer) {
  assert((int)all_points.size() > 0);
  num_levels_ = num_levels;
  point_dim_ = all_points[0].point().size();
  // Do the top level, which is just the centroid (k-means with 1 cluster).
  KMeansClusterer clusterer(1, MAX_ITERATIONS, distance_computer);
  clusterer.Cluster(all_points);
  assert(clusterer.centers_size() == 1);
  centers_.node(0)->set_point(clusterer.centers().point(0));

  membership_.resize(all_points.size());
  // Build a list of indices into all_points of things we want to continue
  // to cluster. In this case, we want to continue clustering all of the
  // points.
  vector<int> to_cluster;
  for (int ii = 0; ii < (int)all_points.size(); ++ii) {
    to_cluster.push_back(ii);
  }

  RecursiveCluster(1, to_cluster, num_levels, branch_factor, all_points,
                   distance_computer, 0);
  done_ = true;
}



// RecursiveCluster will cluster the data, then break the input indices
// into smaller chunks and recursively cluster those. These smaller chunks,
// of course, are determined by k-means clustering.
// parent_id is the ID of the node of the parent cluster. New children
// will be added underneath this parent.
void HierarchicalClusterer::
RecursiveCluster(int level, const vector<int>& indices, int num_levels,
                 int branch_factor, const vector<PointRef>& all_points,
                 const DistanceComputer& distance_computer,
                 int parent_id) {
  auto_ptr<KMeansClusterer> clusterer(
    new KMeansClusterer(branch_factor, MAX_ITERATIONS, distance_computer));


  // Convert the indices into a list of PointRefs which we can throw
  // into the clusterer.
  vector<PointRef> points;
  for (int ii = 0; ii < (int)indices.size(); ++ii) {
    points.push_back(all_points[indices[ii]]);
  }

  clusterer->Cluster(points);

  PointTreeNode* parent = centers_.node(parent_id);
  assert(parent->child_size() == 0);

  const PointSet& local_centers = clusterer->centers();

  // Add the new nodes under <parent> that we just created from the
  // clustering.
  for (int ii = 0; ii < clusterer->centers_size(); ++ii) {
    PointTreeNode* added = centers_.add_node();
    added->set_point(local_centers.point(ii));
    added->set_parent(parent_id);
    parent->add_child(added->id());
  }

  // Update membership_ to reflect the newly computed clusters.
  for (int ii = 0; ii < (int)indices.size(); ++ii) {
    // This is the index of the child that this point belongs to.
    int member_child_index = clusterer->membership(ii);

    // This is the node ID of that child.
    membership_[indices[ii]] = parent->child(member_child_index);
  }

  // Before we start recursing, free up as much memory as we can.
  // TODO(jjl): do a clusterer.reset(NULL), and copy over the
  // membership data so we don't segfault in the jj loop below.
  points.clear();

  // Only bother continuing to cluster lower levels if we need to.
  if (level < num_levels - 1) {
    // For each cluster returned:
    for (int ii = 0; ii < branch_factor; ++ii) {
      // Create a new list of indices, which contain only points
      // that belong to this cluster.
      // The variable names can get confusing, so here is a summary:
      // * ii indexes a "local" cluster; "local" means it is something
      //   that was returned by the k-means clusterer that just ran.
      // * jj indexes a point in the parent cluster.
      // * indices[jj] gives you an index into all_points corresponding
      //   to the point that is referred to by the point jj.
      vector<int> next_level_indices;
      bool found = false;
      for (int jj = 0; jj < (int)clusterer->membership_size(); ++jj) {
        if (clusterer->membership(jj) == ii) {
          next_level_indices.push_back(indices[jj]);
          found = true;
        }
      }

      // Only recurse if there are more points to cluster.
      if (found) {
        RecursiveCluster(level + 1, next_level_indices, num_levels,
                         branch_factor, all_points,
                         distance_computer, parent->child(ii));
      }
    }
  }
}



void HierarchicalClusterer::WriteToStream(ostream& output_stream) const {
  assert(done_);
  assert(output_stream.good());
  int32_t num_points = (int32_t)membership_.size();
  int32_t num_levels = num_levels_;
  int32_t point_dim = point_dim_;
  output_stream.write((char*)&num_levels, sizeof(int32_t));
  output_stream.write((char*)&num_points, sizeof(int32_t));
  output_stream.write((char*)&point_dim, sizeof(int32_t));

  centers_.WriteToStream(output_stream);

  for (int ii = 0; ii < num_points; ++ii) {
    int32_t membership = membership_[ii];
    output_stream.write((char*)&membership, sizeof(int32_t));
  }
}

void HierarchicalClusterer::WriteToFile(const char* filename) const {
  ofstream output_stream(filename, ios::binary | ios::trunc);
  WriteToStream(output_stream);
  output_stream.close();
}

void HierarchicalClusterer::ReadFromFile(const char* filename) {
  ifstream input_stream(filename, ios::binary);
  ReadFromStream(input_stream);
  input_stream.close();
}

void HierarchicalClusterer::ReadFromStream(istream& input_stream) {
  assert(input_stream.good());
  centers_.clear();

  int32_t num_levels, num_points, point_dim;
  input_stream.read((char*)&num_levels, sizeof(int32_t));
  input_stream.read((char*)&num_points, sizeof(int32_t));
  input_stream.read((char*)&point_dim, sizeof(int32_t));
  num_levels_ = num_levels;
  point_dim_ = point_dim;

  centers_.ReadFromStream(input_stream);
  membership_.resize(num_points);

  for (int ii = 0; ii < num_points; ++ii) {
    int32_t member;
    input_stream.read((char*)&member, sizeof(int32_t));
    membership_[ii] = member;
  }

  done_ = true;
}

int HierarchicalClusterer::EmbedIntoTree(
  const Point& f, const DistanceComputer& distance_computer) const {
  int node_id = 0;  // Start at the root
  const PointTreeNode* node = centers_.node(node_id);

  // For each level, find the bin center closest to f, and add it
  // to <answer>. We can skip the first computation since it's always 0.
  while (node->has_child()) {
    double min_distance = DBL_MAX;
    const PointTreeNode* best_child = NULL;

    for (int jj = 0; jj < node->child_size(); ++jj) {
      const PointTreeNode* child = centers_.node(node->child(jj));
      double distance =
        distance_computer.ComputeDistance(f, child->point(), min_distance);
      if (distance < min_distance) {
        min_distance = distance;
        best_child = child;
      }
    }
    assert(best_child != NULL);
    node = best_child;
  }

  assert(node != NULL);
  return node->id();
}


}  // namespace libpmk
