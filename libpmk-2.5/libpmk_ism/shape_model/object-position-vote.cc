// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of ObjectPositionVote.
//

#include "shape_model/object-position-vote.h"

namespace libpmk_shape_model {

ObjectPositionVote::ObjectPositionVote(int source_x, int source_y,
                                       int vote_x, int vote_y,
                                       double scale, double weight) :
  source_x_(source_x), source_y_(source_y),
  vote_x_(vote_x), vote_y_(vote_y), scale_(scale), weight_(weight) { }

}  // end namespace libpmk_shape_model
