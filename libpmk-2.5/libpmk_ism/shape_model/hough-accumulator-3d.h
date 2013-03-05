// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This is a 3d hough accumulator array, which stores weights for a
// grid where the grid size in each dimension is defined at
// construction. You do not need to provide min/max values or the
// diameter of the data set beforehand; the size of the array will
// adjust dynamically as more votes are added (via AddVote).
//
// All values must be >= 0. Using negative coordinates will cause
// errors.
//
// As this is in 3d, the coordinates are named x, y, and s. The "s"
// generally stands for "scale" as this is intended for use in the
// scale-invariant ISM.
//

#ifndef MULTIOBJ_SHAPE_MODEL_HOUGH_ACCUMULATOR_3D_H
#define MULTIOBJ_SHAPE_MODEL_HOUGH_ACCUMULATOR_3D_H

#include <vector>

using namespace std;

namespace libpmk_shape_model {

class HoughAccumulator3D {
public:
  // The grid sizes are the length of the side of the box in that
  // dimension. They must be strictly greater than 0.
  HoughAccumulator3D(double x_grid_size,
                     double y_grid_size,
                     double s_grid_size);
  ~HoughAccumulator3D() { }

  void AddVote(double x_coord, double y_coord, double s_coord, double weight);

  // Computes the grid coordinates that (x, y, s) would be placed in,
  // but does not actually place it.
  void GetGridCoordinates(double x_coord, double y_coord, double s_coord,
                          int* out_x_grid, int* out_y_grid,
                          int* out_s_grid) const;

  // If the grid coordinate is beyond the bounds of the currently
  // known grid (i.e., greater than GetNumGrids() or less than 0),
  // this will return 0.
  double GetWeight(int x_grid, int y_grid, int s_grid) const;

  // Returns the number of grids in the given direction. This output
  // depends on the maximum coordinate along that axis of course.
  int GetNumGridsX() const;
  int GetNumGridsY() const;
  int GetNumGridsS() const;

  // Given the grid identifier, return the coordinates of the
  // center of that grid.
  void GetCoordinatesOfGrid(int x_grid, int y_grid, int s_grid,
                            double* out_x_coord,
                            double* out_y_coord,
                            double* out_s_coord) const;

  // Searches for local maxima in a 3x3 neighborhood. The maxima are
  // returned as parallel arrays in out_x, out_y, and out_s, along
  // with their respective weights. All of these vectors will be
  // cleared first (and they must not be NULL).
  void GetLocalMaxima(vector<int>* out_x,
                      vector<int>* out_y,
                      vector<int>* out_s,
                      vector<double>* out_weight) const;
  void clear();
  

private:
  double x_grid_size_;
  double y_grid_size_;
  double s_grid_size_;

  // We also maintain the maximum number of Y grids and S grids along
  // any direction. This is so that GetNumGridsY and GetNumGridsS can
  // operate efficiently. GetNumGridsX is already efficient since it
  // is equal to accumulator_.size().
  int max_grids_y_;
  int max_grids_s_;
  vector<vector<vector<double> > > accumulator_;

};
}  // end namespace libpmk_shape_model

#endif  // MULTIOBJ_SHAPE_MODEL_HOUGH_ACCUMULATOR_3D_H
