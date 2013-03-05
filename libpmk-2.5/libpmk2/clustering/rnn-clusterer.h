// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
//

#ifndef CLUSTERING_RNN_CLUSTERER_H
#define CLUSTERING_RNN_CLUSTERER_H

#include <ext/hash_set>
#include <vector>

#include "clustering/clusterer.h"
#include "point_set/point-ref.h"
#include "point_set/point.h"
#include "util/distance-computer.h"

using namespace std;
using namespace __gnu_cxx;

namespace libpmk {
/// Implements RNN clustering.
/** This implements the RNN (reciprocal nearest neighbor) algorithm for
 * average-link clustering with nearest-neighbor chains. It is
 * described in Leibe et. al IJCV, "Robust Object Detection with
 * Interleaved Categorization and Segmentation".
 *
 * Internal details of how it works (mostly safe to ignore):
 * 
 */
class RNNClusterer : public Clusterer {
public:
  RNNClusterer(double threshold);
  virtual ~RNNClusterer();
  /// \brief Returns the variance of the iith cluster after clustering
  /// is done.
  /** This requires Cluster() to already be called. It will fail if ii
   * is greater than or equal to the number of clusters, or if Cluster()
   * has not been successfully called.
   * Note that the returned value is the sum of variances across all
   * dimensions. It should be used to help compute similarities.
   */
  double cluster_variance(int ii) const;
  
protected:
  virtual void DoClustering(const vector<const Point*>& data);
  virtual void DoClustering(const vector<PointRef>& data);

private:
  double threshold_;
  L2DistanceComputer euclidean_distance_computer_;

  struct NNGroup {
    hash_set<int> point_indices_;
    auto_ptr<Point> center_;
    auto_ptr<Point> variance_;

    // This maintains the sum of all entries in variance_.
    double variance_sum_;
  };

  vector<NNGroup*> current_chain_;
  vector<double> current_chain_similarities_;
  vector<NNGroup*> unused_groups_;

  // During the agglomeration algorithm, we sometimes discard points.
  // Technically, those points are ignored as outliers; and this code
  // treats them as such-- it does not use those points to determine
  // any cluster centers. However, since the Clusterer interface
  // requires us to provide a cluster ID for each input point, we'll
  // keep these around, and simply assign these points to the nearest
  // cluster center after the agglomeration is finished.
  hash_set<int> discarded_indices_;

  void clear();

  // This performs the meat of the clustering.
  void PerformAgglomeration();

  // After the RNN clustering algorithm completes, this method will
  // push all of the data into membership_ and cluster_centers_,
  // as required by the Clusterer interface.
  void PerformAssignment(const vector<PointRef>& data);
  void PerformAssignment(const vector<const Point*>& data);
  void ReportClusters(); // Factored out from PerformAssignment().

  // This will merge the groups and store the result in first.
  // The value in <second> is unchanged.
  void MergeGroups(NNGroup* first, NNGroup* second);

  // Finds the nearest NNGroup to <group> within <candidates>,
  // returning its index and similarity value. The out_ pointers must
  // not be NULL. This also assumes that <candidates> is not empty, so
  // a valid candidate_index is guaranteed to be returned.
  void GetNearestNeighbor(const NNGroup& group,
                          const vector<NNGroup*>& candidates,
                          int* out_candidate_index,
                          double* out_similarity) const;

  // Returns the similarity, which may be negative, between the two
  // given groups. It relies on proper maintainence of
  // NNGroup::variance_sum_; the similarity is computed in O(d) by
  // using the means and variances, where d is the feature dimension.
  double ComputeSimilarity(const NNGroup& first, const NNGroup& second) const;
};

}  // end namespace lipmk

#endif  // CLUSTERING_RNN_CLUSTERER_H
