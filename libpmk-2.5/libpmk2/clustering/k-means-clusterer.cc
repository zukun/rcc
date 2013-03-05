// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <cstdlib>
#include <vector>
#include "clustering/k-means-clusterer.h"
#include "point_set/point-ref.h"
#include "point_set/point-set.h"
#include "util/distance-computer.h"

namespace libpmk {

KMeansClusterer::KMeansClusterer(int num_clusters, int max_iters,
                                 const DistanceComputer& distance_computer) :
  Clusterer(), num_clusters_(num_clusters), max_iterations_(max_iters),
  distance_computer_(distance_computer) { }

void KMeansClusterer::DoClustering(const vector<const Point*>& data) {
  done_ = false;

  int num_points = data.size();
  for (int ii = 0; ii < num_points; ++ii) {
    membership_[ii] = 0;
  }

  // Trivial case:
  // If there are more clusters than actual points, then the trivial answer
  // is to just place a cluster at each point.
  if (num_clusters_ >= num_points) {
    int jj = 0;

    for (int ii = 0; ii < num_points; ++ii) {
      cluster_centers_->add_point(*(data[ii]));
      membership_[jj] = jj;
      ++jj;
    }

    done_ = true;
    return;
  }  // End trivial case

  // Compute initial clusters: we'll pick random points in the data set
  // to put the clusters on.
  vector<int> random_indices(num_clusters_);
  for (int ii = 0; ii < num_clusters_; ++ii) {
    // We don't want to put two cluster centers on the same point.
    bool has_dupes = false;
    int random_index_candidate;
    do {
      has_dupes = false;
      random_index_candidate = rand() % num_points;
      for (int kk = 0; kk < ii; ++kk) {
        if (random_index_candidate == random_indices[kk]) {
          has_dupes = true;
          break;
        }
      }
    } while(has_dupes);
    random_indices[ii] = random_index_candidate;
  }

  // Now that we have a bunch of random indices that are not dupes,
  // populate the cluster_centers_ with the actual points.
  for (int ii = 0; ii < (int)random_indices.size(); ++ii) {
    int random_index = random_indices[ii];
    cluster_centers_->add_point(*(data[random_index]));
  }

  int num_iters = 0;
  bool changed = true;

  // This is where the actual clustering is done:
  while ((num_iters < max_iterations_) && changed) {
    ComputeMembership(data);
    changed = ComputeMeans(data);
    ++num_iters;
  }

  // If there are any clusters that have 0 members, simply remove
  // them.
  vector<int> counts(cluster_centers_->size());
  for (int ii = 0; ii < (int)membership_.size(); ++ii) {
    ++counts[membership_[ii]];
  }

  for (int ii = (int)counts.size() - 1; ii >= 0; --ii) {
    if (counts[ii] == 0) {
      cluster_centers_->remove_point(ii);
    }
  }
  ComputeMembership(data);

  done_ = true;
}


void KMeansClusterer::ComputeMembership(const vector<const Point*>& data) {
  for (int ii = 0; ii < (int)data.size(); ++ii) {
    int best = 0;

    // Find the cluster center closest to this point
    double min_distance =
      distance_computer_.ComputeDistance(*(data[ii]),
                                         cluster_centers_->point(0));

    for (int jj = 1; jj < cluster_centers_->size(); ++jj) {
      double new_distance =
        distance_computer_.ComputeDistance(*(data[ii]),
                                           cluster_centers_->point(jj),
                                           min_distance);

      if (new_distance < min_distance) {
        best = jj;
        min_distance = new_distance;
      }
    }
    membership_[ii] = best;
  }
}

bool KMeansClusterer::ComputeMeans(const vector<const Point*>& data) {
  // Counts the number of points belonging to each cluster.
  vector<int> counts(num_clusters_);
  PointSet new_clusters(data[0]->size());
  bool clusters_changed = false;

  // Fill the new point set with a bunch of 0 points.
  for (int ii = 0; ii < cluster_centers_->size(); ++ii) {
    new_clusters.add_point();
  }

  // Compute the counts. Also, in the same loop, sum up all of the feature
  // values belonging to any single cluster and place them in
  // new_clusters. Later on, we'll divide these values by the counts,
  // resulting in the mean, which may become the new cluster center.
  for (int ii = 0; ii < (int)data.size(); ++ii) {
    ++counts[membership_[ii]];

    Point* new_cluster = new_clusters.mutable_point(membership_[ii]);
    const Point& cluster_member(*(data[ii]));
    for (int jj = 0; jj < new_clusters.point_dim(); ++jj) {
      new_cluster->set_feature(jj,
                               new_cluster->feature(jj) + cluster_member[jj]);
    }
  }

  for (int ii = 0; ii < num_clusters_; ++ii) {
    // If this cluster no longer has any members, reassign it
    // to a random point.
    if (counts[ii] == 0) {
      int random_index = rand() % (int)data.size();

      // TODO: should this be new_centers and not cluster_centers_?
      Point* reassigned = new_clusters.mutable_point(ii);
      reassigned->CopyFrom(*(data[random_index]));
      clusters_changed = true;
    } else {
      // This is where we divide by the counts, resulting in the average.
      Point* new_cluster = new_clusters.mutable_point(ii);
      for (int jj = 0; jj < new_clusters.point_dim(); ++jj) {
        new_cluster->set_feature(jj, (new_cluster->feature(jj) /
                                      static_cast<double>(counts[ii])));

        // Is this new value any different from the old value? We
        // check clusters_changed first to avoid doing too many
        // floating-point comparisons if we know the clusters have
        // changed anyway.
        if (!clusters_changed &&
            (new_cluster->feature(jj) != cluster_centers_->point(ii)[jj])) {
          clusters_changed = true;
        }
      }
    }
  }

  // If the clusters have indeed changed, we have to copy them over.
  if (clusters_changed) {
    for (int ii = 0; ii < cluster_centers_->size(); ++ii) {
      cluster_centers_->mutable_point(ii)->CopyFrom(new_clusters[ii]);
    }
  }
  return clusters_changed;
}







void KMeansClusterer::DoClustering(const vector<PointRef>& data) {
  done_ = false;

  int num_points = data.size();
  for (int ii = 0; ii < num_points; ++ii) {
    membership_[ii] = 0;
  }

  // Trivial case:
  // If there are more clusters than actual points, then the trivial answer
  // is to just place a cluster at each point.
  if (num_clusters_ >= num_points) {
    int jj = 0;

    for (int ii = 0; ii < num_points; ++ii) {
      cluster_centers_->add_point(data[ii].point());
      membership_[jj] = jj;
      ++jj;
    }

    done_ = true;
    return;
  }  // End trivial case

  // Compute initial clusters: we'll pick random points in the data set
  // to put the clusters on.
  vector<int> random_indices(num_clusters_);
  for (int ii = 0; ii < num_clusters_; ++ii) {
    // We don't want to put two cluster centers on the same point.
    bool has_dupes = false;
    int random_index_candidate;
    do {
      has_dupes = false;
      random_index_candidate = rand() % num_points;
      for (int kk = 0; kk < ii; ++kk) {
        if (random_index_candidate == random_indices[kk]) {
          has_dupes = true;
          break;
        }
      }
    } while(has_dupes);
    random_indices[ii] = random_index_candidate;
  }

  // Now that we have a bunch of random indices that are not dupes,
  // populate the cluster_centers_ with the actual points.
  for (int ii = 0; ii < (int)random_indices.size(); ++ii) {
    int random_index = random_indices[ii];
    cluster_centers_->add_point(data[random_index].point());
  }

  int num_iters = 0;
  bool changed = true;

  // This is where the actual clustering is done:
  while ((num_iters < max_iterations_) && changed) {
    ComputeMembership(data);
    changed = ComputeMeans(data);
    ++num_iters;
  }

  // If there are any clusters that have 0 members, simply remove
  // them.
  vector<int> counts(cluster_centers_->size());
  for (int ii = 0; ii < (int)membership_.size(); ++ii) {
    ++counts[membership_[ii]];
  }

  for (int ii = (int)counts.size() - 1; ii >= 0; --ii) {
    if (counts[ii] == 0) {
      cluster_centers_->remove_point(ii);
    }
  }
  ComputeMembership(data);

  done_ = true;
}


void KMeansClusterer::ComputeMembership(const vector<PointRef>& data) {
  for (int ii = 0; ii < (int)data.size(); ++ii) {
    int best = 0;

    // Find the cluster center closest to this point
    double min_distance =
      distance_computer_.ComputeDistance(data[ii].point(),
                                         cluster_centers_->point(0));

    for (int jj = 1; jj < cluster_centers_->size(); ++jj) {
      double new_distance =
        distance_computer_.ComputeDistance(data[ii].point(),
                                           cluster_centers_->point(jj),
                                           min_distance);

      if (new_distance < min_distance) {
        best = jj;
        min_distance = new_distance;
      }
    }
    membership_[ii] = best;
  }
}

bool KMeansClusterer::ComputeMeans(const vector<PointRef>& data) {
  // Counts the number of points belonging to each cluster.
  vector<int> counts(num_clusters_);
  PointSet new_clusters(data[0].data()->point_dim());
  bool clusters_changed = false;

  // Fill the new point set with a bunch of 0 points.
  for (int ii = 0; ii < cluster_centers_->size(); ++ii) {
    new_clusters.add_point();
  }

  // Compute the counts. Also, in the same loop, sum up all of the feature
  // values belonging to any single cluster and place them in
  // new_clusters. Later on, we'll divide these values by the counts,
  // resulting in the mean, which may become the new cluster center.
  for (int ii = 0; ii < (int)data.size(); ++ii) {
    ++counts[membership_[ii]];

    Point* new_cluster = new_clusters.mutable_point(membership_[ii]);
    const Point& cluster_member(data[ii].point());
    for (int jj = 0; jj < new_clusters.point_dim(); ++jj) {
      new_cluster->set_feature(jj,
                               new_cluster->feature(jj) + cluster_member[jj]);
    }
  }

  for (int ii = 0; ii < num_clusters_; ++ii) {
    // If this cluster no longer has any members, reassign it
    // to a random point.
    if (counts[ii] == 0) {
      int random_index = rand() % (int)data.size();

      // TODO: should this be new_centers and not cluster_centers_?
      Point* reassigned = new_clusters.mutable_point(ii);
      reassigned->CopyFrom(data[random_index].point());
      clusters_changed = true;
    } else {
      // This is where we divide by the counts, resulting in the average.
      Point* new_cluster = new_clusters.mutable_point(ii);
      for (int jj = 0; jj < new_clusters.point_dim(); ++jj) {
        new_cluster->set_feature(jj, (new_cluster->feature(jj) /
                                      static_cast<double>(counts[ii])));

        // Is this new value any different from the old value? We
        // check clusters_changed first to avoid doing too many
        // floating-point comparisons if we know the clusters have
        // changed anyway.
        if (!clusters_changed &&
            (new_cluster->feature(jj) != cluster_centers_->point(ii)[jj])) {
          clusters_changed = true;
        }
      }
    }
  }

  // If the clusters have indeed changed, we have to copy them over.
  if (clusters_changed) {
    for (int ii = 0; ii < cluster_centers_->size(); ++ii) {
      cluster_centers_->mutable_point(ii)->CopyFrom(new_clusters[ii]);
    }
  }
  return clusters_changed;
}

}  // namespace libpmk
