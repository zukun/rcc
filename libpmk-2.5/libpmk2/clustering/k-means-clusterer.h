// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef CLUSTERING_K_MEANS_CLUSTERER_H
#define CLUSTERING_K_MEANS_CLUSTERER_H

#include <vector>
#include "clustering/clusterer.h"
#include "point_set/point-ref.h"
#include "util/distance-computer.h"

using namespace std;

namespace libpmk {
/// Implements K-Means clustering.
/**
 * This implementation may not always return K clusters. There are
 * two cases where we will return fewer than K clusters:
 *
 * <ol>
 * <li>If the number of points provided (N) is less than K, then
 * KMeansClusterer will return N clusters, where each cluster center
 * is one of the points.
 * <li> If the data contains duplicate points, and there are fewer than
 * K unique points, then KMeansClusterer will return M points, where
 * M is the number of unique points in the data.
 * </ol>
 *
 * Both of these situations are generally unlikely, but you should
 * be careful about the assumptions your code makes about the number
 * of returned clusters.
 */
class KMeansClusterer : public Clusterer {
public:
  KMeansClusterer(int num_clusters, int max_iters,
                  const DistanceComputer& distance_computer);

protected:
  /// Perform K-means.
  /**
   * Uses the DistanceComputer it was constructed with to fill up
   * cluster_centers_ with K Point representing the K-means cluster
   * centers. K is assigned by the constructor of KMeansClusterer. If
   * there are fewer data points than K, then the total number of
   * clusters returned is simply the total number of data points (not
   * K).
   */
  virtual void DoClustering(const vector<const Point*>& data);

  /// Perform K-means.
  /**
   * Uses the DistanceComputer it was constructed with to fill up
   * cluster_centers_ with K Point representing the K-means cluster
   * centers. K is assigned by the constructor of KMeansClusterer. If
   * there are fewer data points than K, then the total number of
   * clusters returned is simply the total number of data points (not
   * K).
   */
  virtual void DoClustering(const vector<PointRef>& data);

private:
  int num_clusters_;
  int max_iterations_;
  const DistanceComputer& distance_computer_;

  void ComputeMembership(const vector<const Point*>& data);
  void ComputeMembership(const vector<PointRef>& data);

  // Returns true if this call to ComputeMeans() has changed
  // cluster_centers_.
  bool ComputeMeans(const vector<const Point*>& data);
  bool ComputeMeans(const vector<PointRef>& data);
};
}  // namespace libpmk

#endif  // CLUSTERING_K_MEANS_CLUSTERER_H
