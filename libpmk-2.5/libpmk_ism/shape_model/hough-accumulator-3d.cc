// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of HoughAccumulator3D.
//

#include <iostream>
#include <assert.h>
#include <vector>
#include "shape_model/hough-accumulator-3d.h"

using namespace std;

namespace libpmk_shape_model {

HoughAccumulator3D::HoughAccumulator3D(double x_grid_size,
                                       double y_grid_size,
                                       double s_grid_size) :
  x_grid_size_(x_grid_size), y_grid_size_(y_grid_size),
  s_grid_size_(s_grid_size), max_grids_y_(0), max_grids_s_(0) {
  assert(x_grid_size > 0);
  assert(y_grid_size > 0);
  assert(s_grid_size > 0);
}


void HoughAccumulator3D::AddVote(double x_coord, double y_coord,
                                 double s_coord, double weight) {
  int x_bin = (int)(x_coord / x_grid_size_);
  int y_bin = (int)(y_coord / y_grid_size_);
  int s_bin = (int)(s_coord / s_grid_size_);

  // Resize the arrays as needed.
  if ((int)accumulator_.size() <= x_bin) {
    accumulator_.resize(x_bin + 1);
  }

  if ((int)accumulator_[x_bin].size() <= y_bin) {
    accumulator_[x_bin].resize(y_bin + 1);
    if (y_bin + 1 > max_grids_y_) {
      max_grids_y_ = y_bin + 1;
    }
  }

  if ((int)accumulator_[x_bin][y_bin].size() <= s_bin) {
    accumulator_[x_bin][y_bin].resize(s_bin + 1);
    if (s_bin + 1 > max_grids_s_) {
      max_grids_s_ = s_bin + 1;
    }
  }

  accumulator_[x_bin][y_bin][s_bin] += weight;
}

double HoughAccumulator3D::GetWeight(int x_grid, int y_grid,
                                     int s_grid) const {
  if (x_grid < 0 || y_grid < 0 || s_grid < 0) {
    return 0;
  }

  if ((int)accumulator_.size() <= x_grid) {
    return 0;
  }

  if ((int)accumulator_[x_grid].size() <= y_grid) {
    return 0;
  }

  if ((int)accumulator_[x_grid][y_grid].size() <= s_grid) {
    return 0;
  }

  return accumulator_[x_grid][y_grid][s_grid];
}

int HoughAccumulator3D::GetNumGridsX() const {
  return (int)accumulator_.size();
}

int HoughAccumulator3D::GetNumGridsY() const {
  return max_grids_y_;
}

int HoughAccumulator3D::GetNumGridsS() const {
  return max_grids_s_;
}

void HoughAccumulator3D::clear() {
  max_grids_y_ = 0;
  max_grids_s_ = 0;
  accumulator_.clear();
}

void HoughAccumulator3D::GetLocalMaxima(vector<int>* out_x,
                                        vector<int>* out_y,
                                        vector<int>* out_s,
                                        vector<double>* out_weight) const {
  assert(out_x != NULL);
  assert(out_y != NULL);
  assert(out_s != NULL);
  assert(out_weight != NULL);
  out_x->clear();
  out_y->clear();
  out_s->clear();
  out_weight->clear();

  for (int xx = 0; xx < GetNumGridsX(); ++xx) {
    for (int yy = 0; yy < GetNumGridsY(); ++yy) {
      for (int ss = 0; ss < GetNumGridsS(); ++ss) {
        double this_bin_weight = GetWeight(xx, yy, ss);
        bool is_local_max = true;

        if (this_bin_weight <= 0) {
          is_local_max = false;
        }

        // Search the 3x3x3 neighborhood and see if this bin
        // (xx,yy,ss) is a local max. There are ways to make this more
        // efficient by keeping track of which bins we've checked.
        for (int deltax = -1; deltax <= 1; ++deltax) {
          if (!is_local_max) {
            break;
          }
          for (int deltay = -1; deltay <= 1; ++deltay) {
            if (!is_local_max) {
              break;
            }
            for (int deltas = -1; deltas <= 1; ++deltas) {
              if (deltax == 0 && deltay == 0 && deltas == 0) {
                continue;
              }
              double value = GetWeight(xx + deltax, yy + deltay, ss + deltas);
              if (value > this_bin_weight) {
                is_local_max = false;
                break;
              }
            }
          }
        }

        // If this bin is a local max, add it to the list.
        if (is_local_max) {
          out_x->push_back(xx);
          out_y->push_back(yy);
          out_s->push_back(ss);
          out_weight->push_back(this_bin_weight);
        }
      }
    }
  }
}

void HoughAccumulator3D::GetGridCoordinates(double x_coord, double y_coord,
                                            double s_coord,
                                            int* out_x_grid, int* out_y_grid,
                                            int* out_s_grid) const {
  assert(out_x_grid != NULL);
  assert(out_y_grid != NULL);
  assert(out_s_grid != NULL);
  *out_x_grid = (int)(x_coord / x_grid_size_);
  *out_y_grid = (int)(y_coord / y_grid_size_);
  *out_s_grid = (int)(s_coord / s_grid_size_);
}

void HoughAccumulator3D::GetCoordinatesOfGrid(
  int x_grid, int y_grid, int s_grid,
  double* out_x_coord, double* out_y_coord, double* out_s_coord) const {
  assert(out_x_coord != NULL);
  assert(out_y_coord != NULL);
  assert(out_s_coord != NULL);
  *out_x_coord = x_grid_size_ * (x_grid + 0.5);
  *out_y_coord = y_grid_size_ * (y_grid + 0.5);
  *out_s_coord = s_grid_size_ * (s_grid + 0.5);
}

}  // end namespace libpmk_shape_model
