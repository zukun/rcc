// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of ImplicitShapeModel.
//

#include <math.h>
#include <cfloat>
#include <iostream>
#include <vector>

#include "shape_model/implicit-shape-model.h"

#include "clustering/rnn-clusterer.h"
#include "clustering/k-means-clusterer.h"
#include "point_set/point.h"
#include "point_set/point-ref.h"
#include "point_set/point-set.h"
#include "point_set/point-set-list.h"
#include "shape_model/codebook-entry.h"
#include "shape_model/hough-accumulator-3d.h"
#include "shape_model/object-position-vote.h"
#include "shape_model/uniform-balloon-density-estimator.h"
#include "util/distance-computer.h"

using namespace std;
using namespace libpmk;

namespace libpmk_shape_model {

ImplicitShapeModel::ImplicitShapeModel() : trained_(false),
                                           cluster_centers_(NULL) { }

ImplicitShapeModel::~ImplicitShapeModel() {
  clear();
}

void ImplicitShapeModel::clear() {
  for (int ii = 0; ii < (int)codebook_.size(); ++ii) {
    delete codebook_[ii];
  }
  codebook_.clear();
  cluster_variances_.clear();
  cluster_centers_.reset(NULL);
}

void ImplicitShapeModel::Train(const PointSetList& features,
                               const PointSetList& locations,
                               const vector<double>& scales,
                               double threshold) {
  clear();
  RNNClusterer clusterer(threshold);
  L2DistanceComputer euclidean_dist_computer;

  vector<PointRef> point_refs;
  features.GetPointRefs(&point_refs);
  clusterer.Cluster(point_refs);

  // Store a copy of the cluster centers:
  cluster_centers_.reset(new PointSet(clusterer.centers().point_dim()));
  for (int ii = 0; ii < clusterer.centers().size(); ++ii) {
    cluster_centers_->add_point(clusterer.centers().point(ii));
    cluster_variances_.push_back(clusterer.cluster_variance(ii));
  }

  // Initialize the codebook:
  for (int ii = 0; ii < (int)cluster_centers_->size(); ++ii) {
    codebook_.push_back(new CodebookEntry());
  }

  // Populate the codebook. For each training feature, we'll see which
  // clusters it matches. For all matching clusters, record an
  // occurrence of that feature in that cluster.
  for (int ii = 0; ii < (int)point_refs.size(); ++ii) {
    const PointRef& point_ref(point_refs[ii]);
    int which_point_set = point_ref.which_point_set();
    int which_point = point_ref.which_point();

    for (int jj = 0; jj < cluster_centers_->size(); ++jj) {
      const Point& center(cluster_centers_->point(jj));
      double variance = cluster_variances_[jj];
      double similarity = - (variance +
                             euclidean_dist_computer.ComputeDistance(
                               center, point_ref.point()));
      if (similarity >= threshold) {
        const PointSet& location(locations.point_set(which_point_set));
        Point point(2);
        point[0] = location.point(which_point).feature(0);
        point[1] = location.point(which_point).feature(1);
        codebook_[jj]->AppendOccurrence(
          features.point_set(which_point_set).point(which_point),
          point, scales[ii]);
      }
    }
  }

  trained_ = true;
}

void ImplicitShapeModel::GetProbabalisticVotes(
  const PointSet& features, const PointSet& locations,
  const vector<double>& scales,
  double threshold,
  vector<ObjectPositionVote>* out_votes) const {
  assert(out_votes != NULL);
  assert(features.size() == locations.size());
  out_votes->clear();

  // For each feature, find its matching codebook entry, then
  // have it cast a vote.
  L2DistanceComputer distance_computer;
  for (int ii = 0; ii < features.size(); ++ii) {
    const Point& feature(features.point(ii));
    const Point& location(locations.point(ii));

    // This stores, for the current feature point in question, the
    // indices of the matching codebook entries.
    vector<int> matching_codebook_entries;

    for (int jj = 0; jj < cluster_centers_->size(); ++jj) {
      double distance =
        distance_computer.ComputeDistance(cluster_centers_->point(jj),
                                          feature);
      distance += cluster_variances_[jj];
      double similarity = -distance;
      if (similarity >= threshold) {
        matching_codebook_entries.push_back(jj);
      }
    }

    // If there were no matching codebook entries, this feature
    // casts no votes.
    if ((int)matching_codebook_entries.size() == 0) {
      continue;
    }

    // Each occurrence in each matching codebook entry casts a vote,
    // weighted both by the number of matches and the number of
    // occurrences per match.
    double match_weight = 1.0 / matching_codebook_entries.size();
    for (int jj = 0; jj < (int)matching_codebook_entries.size(); ++jj) {
      int matching_entry = matching_codebook_entries[jj];
      CodebookEntry* occurrences = codebook_[matching_entry];
      for (int kk = 0; kk < occurrences->size(); ++kk) {
        double scale_factor = scales[ii] / occurrences->scale(kk);
        int vote_x =
          (int)(location[0] - (scale_factor * occurrences->location(kk)[0]));
        int vote_y =
          (int)(location[1] - (scale_factor * occurrences->location(kk)[1]));

        if (vote_x > 0 && vote_y > 0) {
          double occurrence_weight = 1.0 / occurrences->size();
          double vote_weight = occurrence_weight * match_weight;
          int source_x = (int)location[0];
          int source_y = (int)location[1];          
          ObjectPositionVote vote(source_x, source_y,
                                  vote_x, vote_y, scale_factor, vote_weight);
          out_votes->push_back(vote);
        }
      } 
    }
  }
}

void ImplicitShapeModel::GetObjectHypotheses(
  const vector<ObjectPositionVote>& votes,
  int, int, int,
  double threshold,
  vector<pair<int, int> >* hypotheses,
  vector<double>* scores) const {
  assert(hypotheses != NULL);
  hypotheses->clear();

  if (scores != NULL) {
    scores->clear();
  }

  // Apply a uniform grid and accumulate the votes in the x-y-s space
  // to quickly find candidate maxima.
  HoughAccumulator3D accumulator(10, 10, 0.1);
  for (int ii = 0; ii < (int)votes.size(); ++ii) {
    const ObjectPositionVote& vote(votes[ii]);
    accumulator.AddVote(vote.vote_x(), vote.vote_y(),
                        vote.scale(), vote.weight());
  }

  UniformBalloonDensityEstimator estimator(10, 10, 0.1);

  // Search for local maxima in the grid in 3x3x3 neighborhoods. Store
  // the local maxima as (x,y) pairs where x and y identify the grid
  // location.
  vector<int> candidate_x;
  vector<int> candidate_y;
  vector<int> candidate_s;
  vector<double> candidate_weight;
  accumulator.GetLocalMaxima(&candidate_x, &candidate_y, &candidate_s,
                             &candidate_weight);


  // For each candidate grid location, apply the MSME search.
  for (int ii = 0; ii < (int)candidate_x.size(); ++ii) {
    // Store the (x,y,s) coordinates of the center point of the grid
    // in question. This is where we start the search.
    double mean_x, mean_y, mean_s;
    accumulator.GetCoordinatesOfGrid(candidate_x[ii], candidate_y[ii],
                                     candidate_s[ii],
                                     &mean_x, &mean_y, &mean_s);

    while (true) {  // While we're not converged yet:
      double score = 0;
      double new_x = 0;
      double new_y = 0;
      double new_s = 0;
      double sum = 0;

      for (int jj = 0; jj < (int)votes.size(); ++jj) {
        const ObjectPositionVote& vote(votes[jj]);
        // 5% of hypothesized object size
        double balloon_factor = vote.scale();
        if (vote.scale() < 1) {
          balloon_factor = 1;
        }
        //double balloon_factor = 1;

        double kernel_value =
          estimator.ComputeValue(mean_x - vote.vote_x(),
                                 mean_y - vote.vote_y(),
                                 mean_s - vote.scale(),
                                 balloon_factor);

//        cout << "Mean is " << mean_x << " " << mean_y << " " << mean_s;
 //        cout << " Vote is " << vote.vote_x() << " " <<
//          vote.vote_y() << " " << vote.scale() << endl;
        //cout << "Kernel value is " << kernel_value << endl;
        if (kernel_value > 0) {
          score += vote.weight() * kernel_value;
          new_x += vote.vote_x() * kernel_value;
          new_y += vote.vote_y() * kernel_value;
          new_s += vote.scale() * kernel_value;
          sum += kernel_value;
        }
      }

      new_x /= sum;
      new_y /= sum;
      new_s /= sum;
      if (fabs(mean_x - new_x) < 0.0001 &&
          fabs(mean_y - new_y) < 0.0001 &&
          fabs(mean_s - new_s) < 0.0001) {
        if (score > threshold) {
          hypotheses->push_back(pair<int, int>((int)mean_x, (int)mean_y));
          if (scores != NULL) {
            scores->push_back(score);
          }
        }
        break;
      } else {
        mean_x = new_x;
        mean_y = new_y;
        mean_s = new_s;
      }
    }
  }
}




void ImplicitShapeModel::GetBestHypothesisWithVotes(
  const vector<ObjectPositionVote>& votes,
  int, int, int,
  double* best_score,
  pair<int, int>* hypothesis,
  vector<double>* weights) const {
  assert(hypothesis != NULL);
  *best_score = 0;
  weights->clear();
  weights->resize(votes.size());
  // Apply a uniform grid and accumulate the votes in the x-y-s space
  // to quickly find candidate maxima.
  HoughAccumulator3D accumulator(10, 10, 0.1);
  for (int ii = 0; ii < (int)votes.size(); ++ii) {
    const ObjectPositionVote& vote(votes[ii]);
    accumulator.AddVote(vote.vote_x(), vote.vote_y(),
                        vote.scale(), vote.weight());
  }

  UniformBalloonDensityEstimator estimator(10, 10, 0.1);

  // Search for local maxima in the grid in 3x3x3 neighborhoods. Store
  // the local maxima as (x,y) pairs where x and y identify the grid
  // location.
  vector<int> candidate_x;
  vector<int> candidate_y;
  vector<int> candidate_s;
  vector<double> candidate_weight;
  accumulator.GetLocalMaxima(&candidate_x, &candidate_y, &candidate_s,
                             &candidate_weight);

  // This measures, for each ObjectPositionVote, its contribution to
  // the current mean.
  vector<double> current_vote_weights;
  current_vote_weights.resize(votes.size());

  // For each candidate grid location, apply the MSME search.
  for (int ii = 0; ii < (int)candidate_x.size(); ++ii) {
    current_vote_weights.clear();
    current_vote_weights.resize(votes.size());

    // Store the (x,y,s) coordinates of the center point of the grid
    // in question. This is where we start the search.
    double mean_x, mean_y, mean_s;
    accumulator.GetCoordinatesOfGrid(candidate_x[ii], candidate_y[ii],
                                     candidate_s[ii],
                                     &mean_x, &mean_y, &mean_s);

    while (true) {  // While we're not converged yet:
      double score = 0;
      double new_x = 0;
      double new_y = 0;
      double new_s = 0;
      double sum = 0;

      for (int jj = 0; jj < (int)votes.size(); ++jj) {
        const ObjectPositionVote& vote(votes[jj]);
        current_vote_weights[jj] = 0;
        // 5% of hypothesized object size
        double balloon_factor = vote.scale();
        if (vote.scale() < 1) {
          balloon_factor = 1;
        }
        //double balloon_factor = 1;

        double kernel_value =
          estimator.ComputeValue(mean_x - vote.vote_x(),
                                 mean_y - vote.vote_y(),
                                 mean_s - vote.scale(),
                                 balloon_factor);
//        cout << "Mean is " << mean_x << " " << mean_y << " " << mean_s;
 //        cout << " Vote is " << vote.vote_x() << " " <<
//          vote.vote_y() << " " << vote.scale() << endl;
        //cout << "Kernel value is " << kernel_value << endl;
        if (kernel_value > 0) {
          current_vote_weights[jj] = vote.weight() * kernel_value;
          score += vote.weight() * kernel_value;
          new_x += vote.vote_x() * kernel_value;
          new_y += vote.vote_y() * kernel_value;
          new_s += vote.scale() * kernel_value;
          sum += kernel_value;
        }
      }

      new_x /= sum;
      new_y /= sum;
      new_s /= sum;
      if (fabs(mean_x - new_x) < 0.0001 &&
          fabs(mean_y - new_y) < 0.0001 &&
          fabs(mean_s - new_s) < 0.0001) {
        if (score > *best_score) {
          hypothesis->first = (int)mean_x;
          hypothesis->second = (int)mean_y;
          for (int jj = 0; jj < (int)current_vote_weights.size(); ++jj) {
            weights->at(jj) = current_vote_weights[jj];
          }
          *best_score = score;
        }
        break;
      } else {
        mean_x = new_x;
        mean_y = new_y;
        mean_s = new_s;
      }
    }
  }
}





void ImplicitShapeModel::WriteToStream(ostream& output_stream) const {
  assert(trained_);
  int32_t num_entries = cluster_centers_->size();
  output_stream.write((char *)(&num_entries), sizeof(int32_t));

  int32_t feature_dim = cluster_centers_->point_dim();
  output_stream.write((char *)(&feature_dim), sizeof(int32_t));

  cluster_centers_->WriteToStream(output_stream);

  for (int ii = 0; ii < num_entries; ++ii) {
    double variance = cluster_variances_[ii];
    output_stream.write((char *)(&variance), sizeof(double));
  }

  for (int ii = 0; ii < num_entries; ++ii) {
    codebook_[ii]->WriteToStream(output_stream);
  }
}

void ImplicitShapeModel::ReadFromStream(istream& input_stream) {
  clear();

  int32_t num_entries;
  input_stream.read((char *)(&num_entries), sizeof(int32_t));

  int32_t feature_dim;
  input_stream.read((char *)(&feature_dim), sizeof(int32_t));
  cluster_centers_.reset(new PointSet(feature_dim));
  cluster_centers_->ReadFromStream(input_stream);

  for (int ii = 0; ii < num_entries; ++ii) {
    double variance;
    input_stream.read((char *)(&variance), sizeof(double));
    cluster_variances_.push_back(variance);
  }

  for (int ii = 0; ii < num_entries; ++ii) {
    CodebookEntry* entry = new CodebookEntry();
    codebook_.push_back(entry);
    entry->ReadFromStream(input_stream);
  }
}

}  // end namespace libpmk_shape_model
