// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// TODO(jjl): Allow users to pass in arbitrary Clusterer objects, not just
// hard-coded KMeansClusterer.
//

#ifndef CLUSTERING_HIERARCHICAL_CLUSTERER_H
#define CLUSTERING_HIERARCHICAL_CLUSTERER_H

#include <vector>
#include <list>
#include <iostream>
#include "clustering/clusterer.h"
#include "point_set/point-ref.h"
#include "tree/point-tree-node.h"
#include "tree/tree.cc"
#include "util/distance-computer.h"

using namespace std;

namespace libpmk {

template class Tree<PointTreeNode>;

/// Hierarchical K-Means clusterer.
/**
 * This clusterer does not use the Clusterer interface because its
 * output involves multiple levels. However, similar to Clusterer,
 * HierarchicalClusterer has the ability to get its data either by
 * actually doing hierarchical K-means, or by reading
 * already-processed data. The difference is that the returned cluster
 * centers are a PointSetList, where each element in the list gives
 * the centers for a particular level.
 */
class HierarchicalClusterer {
public:
  HierarchicalClusterer();


  /// Get the cluster centers.
  /**
   * Must be called after Cluster() or ReadFromStream(). Returns a
   * const ref to the tree of cluster centers. A PointTreeNode simply
   * has a Point in it, which represents the cluster center.
   * \sa Tree
   */
  const Tree<PointTreeNode>& centers() const;

  /// Report which leaf the <index>th point belongs to.
  /**
   * Returns the node ID of the leaf that this point belongs to. This ID
   * can be used for lookups in the centers() tree.
   * \sa IdentifyMemberIDPath()
   * \sa IdentifyMemberTreePath()
   */
  int membership(int index) const;

  /// Return the number of points that were clustered.
  int membership_size() const { return (int)membership_.size(); }

  /// Get the set of node IDs in the hierarchy that a point belongs to.
  /**
   * Outputs a list with size equal to the depth of the tree (or if
   * the tree is not balanced, a size equal to the depth of the leaf
   * node associated with a point). The elements of the returned
   * list are the IDs of the nodes (starting with the root, ending at
   * the leaf) this point belongs to at that level. The last element
   * will be the ID of the root, which currently will always be 0.
   *
   * \sa IdentifyMemberTreePath()
   */
  void IdentifyMemberIDPath(int index, list<int>* out) const;

  /// Get a path down the hierarchy that a point belongs to.
  /**
   * Outputs a list representing a path down the tree. If the leaf
   * is at depth n, then the output of this list has size (n-1).
   * The first element of the list specifies which child (its index)
   * of the root to go down. The second element specifies which child
   * to go to after that, and so on. Obviously, this value is at most
   * B (the branch factor), since each node will only have at most B
   * children. The value tells you the index of the child link to
   * traverse in the tree.
   *
   * This method is useful for converting to sparse representations,
   * such as Pyramids.
   *
   * \sa IdentifyMemberIDPath()
   */
  void IdentifyMemberTreePath(int index, list<int>* out) const;


  /// Performs the actual clustering and stores the result internally.
  void Cluster(int num_levels, int branch_factor,
               const vector<PointRef>& points,
               const DistanceComputer& distance_computer);

  /// Write the clustered data to a stream.
  /**
   * Must be called after Cluster() or ReadFromStream(). File format:
   * <ul>
   * <li> (int32) N, the number of levels
   * <li> (int32) P, the total number of clustered points
   * <li> (int32) D, the feature dim
   * <li> (Tree<PointTreeNode>) A tree representing the cluster centers
   * <li> For each point: <ul>
   *      <li> (int32) the node ID of the leaf node that the point belongs to.
   * </ul>
   * </ul>
   *
   * This function will abort if the stream is bad.
   */
  void WriteToStream(ostream& output_stream) const;

  /// Write the clustered data to a file.
  void WriteToFile(const char* output_filename) const;

  /// Read clustered data from a stream.
  /**
   * Can be called in lieu of Cluster() to load preprocessed data. See
   * WriteToStream() for the format.  This function will abort if the
   * stream is bad.
   * \sa WriteToStream()
   */
  void ReadFromStream(istream& input_stream);

  /// Read the clustered data from a file.
  void ReadFromFile(const char* input_filename);

  /// Places the given point into the tree, using the given distance computer.
  /**
   * Returns the node ID of the leaf that <f> gets assigned to.
   *
   * At a particular node, this method will go to the child whose center
   * is closest to it (by <distance_computer>). This will continue until
   * it reaches a leaf node.
   */
  int EmbedIntoTree(const Point& f,
                    const DistanceComputer& distance_computer) const;

  /// Default 100.
  static const int MAX_ITERATIONS;

private:
  int num_levels_;
  int point_dim_;

  // RecursiveCluster() takes as input which level it's going to get
  // clusters for, as well as a list of ints. This list of ints gives
  // *indices into all_points_* to cluster (note that all_points_ itself
  // is a list of indices into the actual data).
  void RecursiveCluster(int level, const vector<int>& indices,
                        int num_levels, int branch_factor,
                        const vector<PointRef>& points,
                        const DistanceComputer& distance_computer,
                        int parent_index);

  // The cluster centers.
  Tree<PointTreeNode> centers_;

  // A vector with size equal to the number of points being clustered.
  // membership_[ii] gives the node ID of the leaf node that the <ii>th
  // point belongs to.
  vector<int> membership_;

  bool done_;
};
}  // namespace libpmk

#endif  // CLUSTERING_HIERARCHICAL_CLUSTERER_H
