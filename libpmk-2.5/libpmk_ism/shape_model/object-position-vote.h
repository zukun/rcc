// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This simply encapsulates a vote that a feature at a given location
// makes, for the center of an object.
//

#ifndef MULTIOBJ_SHAPE_MODEL_OBJECT_POSITION_VOTE_H
#define MULTIOBJ_SHAPE_MODEL_OBJECT_POSITION_VOTE_H

namespace libpmk_shape_model {

class ObjectPositionVote {
public:
  ObjectPositionVote(int source_x, int source_y,
                     int vote_x, int vote_y,
                     double scale, double weight);
  ~ObjectPositionVote() { }

  int source_x() const { return source_x_; }
  int source_y() const { return source_y_; }
  int vote_x() const { return vote_x_; }
  int vote_y() const { return vote_y_; }
  double weight() const { return weight_; }
  double scale() const { return scale_; }

private:
  int source_x_;
  int source_y_;
  int vote_x_;
  int vote_y_;
  double scale_;
  double weight_;
};
}  // end namespace libpmk_shape_model

#endif  // MULTIOBJ_SHAPE_MODEL_OBJECT_POSITION_VOTE_H
