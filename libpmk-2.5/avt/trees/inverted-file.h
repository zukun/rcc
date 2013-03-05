// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: Tom Yeh  (tomyeh@mit.edu)
//         John Lee (jjl@mit.edu)
//
// A record of points and where they came from. They are stored here
// as PointRefs, so the original point must exist somewhere in memory
// (or in an OnDiskPointSetList).
//

#ifndef TREES_INVERTED_FILE_H
#define TREES_INVERTED_FILE_H

#include <iostream>
#include <list>
#include <map>
#include <vector>
#include "point_set/point.h"
#include "point_set/point-ref.h"

using namespace std;
using namespace libpmk;

namespace incremental_vtree {

class PointRecord {
public:
  PointRecord(int image_id, const PointRef& point) : point_ref_(point),
                                                     image_id_(image_id) { }

  const PointRef& point_ref() const { return point_ref_; }
  int image_id() const { return image_id_; }

private:
  PointRef point_ref_;
  int image_id_;
};


class InvertedFile {
public:
  InvertedFile() : num_records_(0) { }
  InvertedFile(const InvertedFile& other);
  ~InvertedFile();

  /// \brief Adds a single record about the given point.
  /**
   * The image_id is provided to identify which image this point came
   * from. This ID is independent of where the point resides in
   * memory; it is simply assigned by the user.
   */
  void add_point(int image_id, const PointRef& point);

  /// Returns the number of points we stored from image <image_id>.
  int image_size(int image_id) const;

  /// Returns the total number of images we are storing points from.
  int members_size() const;

  /// Returns the total number of points stored.
  int size() const;

  /// Populates <output> with PointRecords corresponding to the stored points.
  /**
   * The output will <b>NOT</b> be cleared first, and must not be
   * NULL. The pointers returned are still owned by the InvertedFile.
   */
  void GetPointRecords(vector<PointRecord>* output) const;

  /// Populates <output> with PointRefs corresponding to the stored points.
  /**
   * The output will <b>NOT</b> be cleared first, and must not be
   * NULL. The pointers returned are still owned by the InvertedFile.
   */
  void GetPointRefs(vector<PointRef>* output) const;

  /// Return a const_iterator over the map of <image_id, count>.
  /**
   * This iterator goes in increasing order of image ID.
   */
  map<int, int>::const_iterator members_begin() const;

  /// Return a const_iterator over the map of <image_id, count>.
  /**
   * This iterator goes in increasing order of image ID.
   */
  map<int, int>::const_iterator members_end() const;

  /// Return a reverse const_iterator over the map of <image_id, count>.
  /**
   * This iterator goes in decreasing order of image ID.
   */
  map<int, int>::const_reverse_iterator members_rbegin() const;

  /// Return a reverse const_iterator over the map of <image_id, count>.
  /**
   * This iterator goes in decreasing order of image ID.
   */
  map<int, int>::const_reverse_iterator members_rend() const;

  void ReadFromStream(istream& input_stream, const PointSetList* data);

  /// \brief Serializes the data to the stream. Assumes all points
  /// have the same feature dim <dim>.
  /**
   * Output format:
   * (int32_t) n, the number of unique images stored by this file
   * For each n:
   *   (int32_t) I, the image ID
   *   (int32_t) n_I, the number of points associated with this image
   *   For each n_I:
   *     (int32_t) the PointRef's index
   *     (int32_t) which PointSet the point is in (the index)
   *     (int32_t) which Point within that pointset it is in
   */
  void WriteToStream(ostream& output_stream) const;

private:
  map<int, list<PointRecord*> > members_;
  int num_records_;
};
}  // end namespace incremental_vtree

#endif  // TREES_INVERTED_FILE_H
