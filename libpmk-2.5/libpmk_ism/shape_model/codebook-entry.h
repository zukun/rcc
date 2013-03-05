// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This class records all of the occurrences of a single visual word
// in a training set. It stores a list of features, and for each one,
// records its position relative to the center.  The position is given
// with X increasing to the right and Y increasing *downward* (not
// upward!).
//
// The implicit shape model stores a set of these, one for each visual
// word.
//

#ifndef MULTIOBJ_SHAPE_MODEL_CODEBOOK_ENTRY_H
#define MULTIOBJ_SHAPE_MODEL_CODEBOOK_ENTRY_H

#include <iostream>
#include <vector>
#include "point_set/point.h"
#include "point_set/point-set.h"

using namespace std;
using namespace libpmk;

namespace libpmk_shape_model {

class CodebookEntry {
public:
  // Creates an empty model.
  CodebookEntry();
  ~CodebookEntry() { }

  // These methods will abort if the index is out-of-bounds.
  const libpmk::Point& feature(int ii) const;
  const libpmk::Point& location(int ii) const;
  double scale(int ii) const;
  int size() const;

  // Add or remove an entry. When appending, it is always placed at
  // the end of the list. When adding an entry, a copy of the feature
  // and the location are made.
  //
  // If the model is empty, it will accept any feature. If it is not
  // empty, the dim of the feature must be the same as the dim of all
  // other features.
  // The dim of the location must always be 2.
  void AppendOccurrence(const libpmk::Point& feature,
                        const libpmk::Point& location,
                        double scale);
  void RemoveOccurrence(int ii);

  /// Writes the codebook entry to a stream.
  /**
   * The format is as follows:
   * <ul>
   * <li>(int32_t) num occurrences, N
   * <li>(int32_t) the feature dim, d
   * <li>(PointSet[d]) the features (there are N Points in this PointSet)
   * <li>(PointSet[2]) the locations (there are N Points in this PointSet)
   * <li>(double * N) the scales of each Point
   * </ul>
   */
  void WriteToStream(ostream& output_stream) const;

  /// Reads the codebook entry from a stream.
  /**
   * This function will clear any data it currently has.
   */
  void ReadFromStream(istream& input_stream);

private:
  // These three structures are all parallel to one another.
  // The locations_ are always 2-dimensional, and (0,0) for a
  // location means it's at the center of the object, with X
  // increasing to the right and Y increasing *DOWNWARD*.
  PointSet features_;
  PointSet locations_;
  vector<double> scales_;
};

}  // end namespace libpmk_shape_model

#endif  // MULTIOBJ_SHAPE_MODEL_CODEBOOK_ENTRY_H
