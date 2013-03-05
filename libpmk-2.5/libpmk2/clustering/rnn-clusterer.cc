// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <cfloat>
#include <ext/hash_set>

#include "clustering/rnn-clusterer.h"

#include "point_set/point-ref.h"
#include "point_set/point.h"
#include "util/distance-computer.h"

using namespace __gnu_cxx;

namespace libpmk {

RNNClusterer::RNNClusterer(double threshold) :
  threshold_(threshold) { }

RNNClusterer::~RNNClusterer() {
  clear();
}

void RNNClusterer::clear() {
  for (int ii = 0; ii < (int)current_chain_.size(); ++ii) {
    delete current_chain_[ii];
  }
  current_chain_.clear();
  current_chain_similarities_.clear();

  for (int ii = 0; ii < (int)unused_groups_.size(); ++ii) {
    delete unused_groups_[ii];
  }
  unused_groups_.clear();
  discarded_indices_.clear();
}

double RNNClusterer::cluster_variance(int ii) const {
  assert(done_);
  if (ii >= (int)current_chain_.size()) {
    return 0;
  }
  return current_chain_.at(ii)->variance_sum_;
}

void RNNClusterer::DoClustering(const vector<PointRef>& data) {
  done_ = false;
  clear();

  int point_dim = data[0].point().size();
  // Initially, there are a number of groups equal to the number
  // of data points (each point is one cluster).
  for (int ii = 0; ii < (int)data.size(); ++ii) {
    NNGroup* new_nn_group = new NNGroup;
    new_nn_group->point_indices_.insert(ii);
    new_nn_group->center_.reset(new Point(point_dim));
    new_nn_group->center_->CopyFrom(data[ii].point());
    new_nn_group->variance_.reset(new Point(point_dim));
    new_nn_group->variance_sum_ = 0;
    unused_groups_.push_back(new_nn_group);
  }

  PerformAgglomeration();
  PerformAssignment(data);
  done_ = true;
}

void RNNClusterer::DoClustering(const vector<const Point*>& data) {
  done_ = false;
  clear();
  int point_dim = data[0]->size();

  // Initially, there are a number of groups equal to the number
  // of data points (each point is one cluster).
  for (int ii = 0; ii < (int)data.size(); ++ii) {
    NNGroup* new_nn_group = new NNGroup;
    new_nn_group->point_indices_.insert(ii);
    new_nn_group->center_.reset(new Point(point_dim));
    new_nn_group->center_->CopyFrom(*(data[ii]));
    new_nn_group->variance_.reset(new Point(point_dim));
    new_nn_group->variance_sum_ = 0;
    unused_groups_.push_back(new_nn_group);
  }  
  PerformAgglomeration();
  PerformAssignment(data);
  done_ = true;
}

void RNNClusterer::PerformAgglomeration() {
  // Initialize the chain with a random point.
  int random_index = rand() % (int)unused_groups_.size();

  // <last> simply keeps track of the index of the last element in the
  // chain.  It must always be equal to current_chain_.size() - 1.
  int last = 0;
  current_chain_.push_back(unused_groups_[random_index]);
  current_chain_similarities_.push_back(-DBL_MAX);
  unused_groups_.erase(unused_groups_.begin() + random_index);

  while ((int)unused_groups_.size() > 0) {
    assert(last == (int)current_chain_.size() - 1);
    assert(last == (int)current_chain_similarities_.size() - 1);

    // Search for the next NN in unused_groups_, retrieving its
    // similarity.
    double similarity;
    int nn_index;
    GetNearestNeighbor(*(current_chain_[last]), unused_groups_,
                       &nn_index, &similarity);
    if (similarity > current_chain_similarities_[last]) {
      // There are no RNNs, so we add this element to the current
      // chain.
      ++last;
      current_chain_.push_back(unused_groups_[nn_index]);
      unused_groups_.erase(unused_groups_.begin() + nn_index);
      current_chain_similarities_.push_back(similarity);
    } else {
      // If we found an RNN for the last element in the chain,
      // we agglomerate those two links, remove them from the
      // chain, and throw them back into unused_groups_. Note
      // that we only do the agglomeration if the two RNNs
      // are similar enough (past the threshold).
      // We found an RNN if the nearest neighbor is further than
      // the previous element.
      if (current_chain_similarities_[last] > threshold_) {
        NNGroup* prev = current_chain_[last];
        NNGroup* prevprev = current_chain_[last - 1];
        current_chain_.pop_back();
        current_chain_.pop_back();
        current_chain_similarities_.pop_back();
        current_chain_similarities_.pop_back();
        MergeGroups(prev, prevprev);
        unused_groups_.push_back(prev);
        delete prevprev;
        last -= 2;
      } else {
        // If the RNN doesn't beat the threshold, that means it's way
        // too far away, and everything else in the chain also has no
        // hope of ever getting agglomerated with anything. So we
        // throw it out.
        //
        // We don't actually just delete it; we first save the indices
        // of the points that are being discarded, so that they can be
        // used later. They are saved into discarded_indices_.
        for (int jj = 0; jj < (int)current_chain_.size(); ++jj) {
          const NNGroup* group = current_chain_[jj];
          for (hash_set<int>::iterator iter = group->point_indices_.begin();
               iter != group->point_indices_.end(); ++iter) {
            discarded_indices_.insert(*iter);
          }
          delete current_chain_[jj];
        }
        current_chain_.clear();
        current_chain_similarities_.clear();
        last = -1;
      }
    }

    // If the chain is empty at this point, initialize it with
    // another random point.
    if (last < 0) {
      int random_index = rand() % (int)unused_groups_.size();
      last = 0;
      current_chain_.push_back(unused_groups_[random_index]);
      current_chain_similarities_.push_back(-DBL_MAX);
      unused_groups_.erase(unused_groups_.begin() + random_index);
    }
  }  
}

void RNNClusterer::ReportClusters() {
  // Each group in the chain is one cluster.
  for (int ii = 0; ii < (int)current_chain_.size(); ++ii) {
    const NNGroup* group = current_chain_[ii];
    cluster_centers_->add_point(*(group->center_.get()));
    for (hash_set<int>::iterator iter = group->point_indices_.begin();
         iter != group->point_indices_.end(); ++iter) {
      membership_[*iter] = ii;
    }
  }
}

void RNNClusterer::PerformAssignment(const vector<PointRef>& data) {
  assert((int)current_chain_.size() > 0);
  ReportClusters();


  // Experiment: for all discarded points, create a new cluster for
  // each of them.
  for (hash_set<int>::iterator iter = discarded_indices_.begin();
       iter != discarded_indices_.end(); ++iter) {
    cluster_centers_->add_point(data[*iter].point());
    membership_[*iter] = cluster_centers_->size() - 1;
  }


  // For all of the points we discarded in the previous
  // step, we will simply assign them to the nearest cluster center.
//   for (hash_set<int>::iterator iter = discarded_indices_.begin();
//        iter != discarded_indices_.end(); ++iter) {
//     const Point& discarded_point(data[*iter].point());

//     double best_distance = DBL_MAX;
//     int best_cluster = -1;

//     for (int ii = 0; ii < (int)current_chain_.size(); ++ii) {
//       const NNGroup* group = current_chain_[ii];
//       const Point& cluster_center(*(group->center_.get()));
//       double distance = euclidean_distance_computer_.ComputeDistance(
//         discarded_point, cluster_center);
//       if (distance < best_distance) {
//         best_distance = distance;
//         best_cluster = ii;
//       }
//     }

//     assert(best_cluster != -1);    
//     membership_[*iter] = best_cluster;
//   }  
}


void RNNClusterer::PerformAssignment(const vector<const Point*>& data) {
  assert((int)current_chain_.size() > 0);
  ReportClusters();

  // For all of the points we discarded in the previous step, we
  // will simply assign them to the nearest cluster center.
  for (hash_set<int>::iterator iter = discarded_indices_.begin();
       iter != discarded_indices_.end(); ++iter) {
    const Point& discarded_point(*(data[*iter]));

    double best_distance = DBL_MAX;
    int best_cluster = -1;

    for (int ii = 0; ii < (int)current_chain_.size(); ++ii) {
      const NNGroup* group = current_chain_[ii];
      const Point& cluster_center(*(group->center_.get()));
      double distance = euclidean_distance_computer_.ComputeDistance(
        discarded_point, cluster_center);
      if (distance < best_distance) {
        best_distance = distance;
        best_cluster = ii;
      }
    }

    assert(best_cluster != -1);    
    membership_[*iter] = best_cluster;
  }  
}

void RNNClusterer::GetNearestNeighbor(
  const NNGroup& group, const vector<NNGroup*>& candidates,
  int* out_candidate_index, double* out_similarity) const {
  assert(out_candidate_index != NULL);
  assert(out_similarity != NULL);

  double best_similarity = -DBL_MAX;
  int best_index = -1;

  for (int ii = 0; ii < (int)candidates.size(); ++ii) {
    double similarity = ComputeSimilarity(group, *(candidates[ii]));
    if (similarity > best_similarity) {
      best_similarity = similarity;
      best_index = ii;
    }
  }

  assert(best_index != -1);
  *out_candidate_index = best_index;
  *out_similarity = best_similarity;
}


double RNNClusterer::ComputeSimilarity(const NNGroup& first,
                                       const NNGroup& second) const {
  // The similarity is -(sigma_x^2 + sigma_y^2 + (mean_x - mean_y)^2).
  return - (first.variance_sum_ +
            second.variance_sum_ +
            euclidean_distance_computer_.ComputeDistance(
              *(first.center_.get()),
              *(second.center_.get())));
}

void RNNClusterer::MergeGroups(NNGroup* first, NNGroup* second) {
  int first_size = (int)first->point_indices_.size();
  int second_size = (int)second->point_indices_.size();

  // Put the point indices from <second> into <first>:
  for (hash_set<int>::iterator iter = second->point_indices_.begin();
       iter != second->point_indices_.end(); ++iter) {
    first->point_indices_.insert(*iter);
  }

  first->variance_sum_ = 0;

  // Update the variance of <first>:
  for (int ii = 0; ii < first->variance_->size(); ++ii) {
    double new_value =
      (first_size * first->variance_->feature(ii)) +
      (second_size * second->variance_->feature(ii));

    double diff = first->center_->feature(ii) - second->center_->feature(ii);
    double mean_contribution =
      ((first_size * second_size) / (double)(first_size + second_size)) *
      (diff * diff);
    new_value += mean_contribution;
    new_value /= (first_size + second_size);
    first->variance_->set_feature(ii, new_value);
    first->variance_sum_ += new_value;
  }

  // Update the mean of <first>:
  for (int ii = 0; ii < first->center_->size(); ++ii) {
    double new_value =
      ((first_size * first->center_->feature(ii)) +
       (second_size * second->center_->feature(ii))) /
      (double)(first_size + second_size);
    first->center_->set_feature(ii, new_value);
  }
}
}  // end namespace lipmk
