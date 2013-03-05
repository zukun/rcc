// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
#ifndef POINT_SET_POINT_SET_LIST_H
#define POINT_SET_POINT_SET_LIST_H

#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include "point_set/point-set.h"
#include "point_set/point-ref.h"
#include "point_set/point.h"

using namespace std;

namespace libpmk {

class PointRef;

/// Abstract interface for a list of PointSet.
/**
 * All Points contained in any PointSetList must have the same
 * dimension. When serialized, a PointSetList takes on the following format:
 * <ul>
 * <li>(int32) N, the number of PointSets
 * <li>(int32) f, the dim of every point in this PointSetList
 * <li>For each PointSet:
 *  <ul>
 *     <li>(int32) The number of Points in this PointSet</li>
 *     <li>For each point in this PointSet: <ul>
 *        <li>(Point) The point itself (generally, float * f)
 *     </ul>
 *     <li>Then, again for each Point in this PointSet:
 *        <ul><li>(float) The weight of the Point</ul>
 *  </ul>
 * </ul>
 *
 * Equivalently, at a higher level:
 * <ul>
 * <li>(int32) N, the number of PointSets
 * <li>(int32) f, the dim of every point in this PointSetList
 * <li>(N * PointSet) the N PointSets.
 * </ul>
 * \sa PointSet
 */

class PointSetList {
public:
  virtual ~PointSetList() { }

  /// Get the total number of PointSets in this PointSetList.
  virtual int point_set_size() const = 0;

  /// Get the total number of Points in this PointSetList.
  /**
   * This is the sum of PointSet::size() over all PointSets in in
   * this PointSetList.
   */
  virtual int point_size() const = 0;

  /// Get the dim of every Point in this PointSetList.
  /**
   * Defined to be 0 if the PointSetList is empty.
   */
  virtual int point_dim() const = 0;

  /// Get the number of Points in each PointSet.
  /**
   * Fills <destination> with a vector of size this.point_set_size(),
   * where the nth element is the number of Points in the nth PointSet
   * in this PointSetList. It is cleared first, and must not be NULL.
   *
   * Returns true on success and false otherwise, although currently
   * there is no reason that this method will fail.
   */
  virtual bool GetSetCardinalities(vector<int>* destination) const;

  /// Returns a const ref to the <index>th PointSet.
  const PointSet& operator[](int index) { return point_set(index); }

  /// Returns a const ref to the <index>th PointSet.
  virtual const PointSet& point_set(int index) const = 0;

  /// Returns the <index>th Point in the PointSetList.
  /**
   * We can also think of a PointSetList as a long vector of Points if
   * we ignore the PointSet boundaries, so you can reference individual
   * points of a PointSetList through this.
   */
  const Point& point(int index) const;

  /// Locate a Point in a PointSet.
  /**
   * Given an index specifying which point we are referring to,
   * report which PointSet it belongs to, as well as the index into
   * that PointSet that the PointSet belongs to.
   *
   * Returns true if <index> is a valid Point and the data
   * was successfully loaded, false otherwise.
   */
  bool GetPointIndices(int index, int* which_point_set,
                       int* which_point) const;

  /// Get PointRefs to every Point in a particular image.
  /**
   * Requires out_refs != NULL. Makes out_refs a vector of size
   * this->point_size(), where the nth element is a PointRef pointing
   * to the nth Point (i.e., points to this->point(n)). If out_refs
   * has something in it beforehand, it will be cleared prior to
   * filling it.
   * \sa PointRef
   */
  void GetPointRefs(vector<PointRef>* out_refs) const;

  /// Get PointRefs to every Point in this PointSetList.
  /**
   * Requires out_refs != NULL. Makes out_refs a vector of size
   * this->point_set(index).size(), where the nth element is a
   * PointRef pointing to the nth Point in the <index>th image. If
   * out_refs has something in it beforehand, it will be cleared prior
   * to filling it.
   * \sa PointRef
   */
  void GetPointRefs(int index, vector<PointRef>* out_refs) const;

  /// Writes the entire PointSetList to a stream.
  /** See the detailed description of PointSetList for the format. */
  void WriteToStream(ostream& output_stream) const;

  /// Writes just the serialized header to a stream.
  void WriteHeaderToStream(ostream& output_stream) const;

  /// Writes the point sets (without a header) sequentially to a stream.
  void WritePointSetsToStream(ostream& output_stream) const;

  /// Writes the entire PointSetList to a file.
  /** See detailed description of PointSetList for the format. */
  void WriteToFile(const char* output_file) const;
};
}  // namespace libpmk

#endif  // POINT_SET_POINT_SET_LIST_H
