// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// OnDiskPointSets are read-only. They are meant to be used for large
// point set lists which do not fit in memory.
// OnDiskPointSet operates with two caches: an "area" cache and an LRU cache.
// The area cache is a small subsequence of the entire point set list.
// When we want to access something in the point set list, we do the following:
//   1. Check the area cache for it. If it's there, return it.
//   2. Check the LRU cache for it.
//        2a. If it's there, return it, and also move it to the front of
//            the LRU cache. The area cache is unchanged.
//        2b. If it's not there, then add it to the LRU cache, evicting the
//            least-recently-used item if needed. ALSO, we reset the area
//            cache, such that the first element of the area cache is the
//            requested item.
//
// WARNING: the [] operator returns a const ref, which may become invalid
// the next time you invoke the [] operator. BE CAREFUL!
//

#ifndef POINT_SET_ON_DISK_POINT_SET_LIST_H
#define POINT_SET_ON_DISK_POINT_SET_LIST_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include "point_set/point-set-list.h"
#include "point_set/point-set.h"

using namespace std;

namespace libpmk {
/// A read-only PointSetList which reads information from disk
/**
 * This class is suitable for loading data that is too large to fit in
 * memory. For performance, OnDiskPointSetList operates using two
 * caches: an LRU cache (of sorts) which remembers the most recently
 * used PointSets, and what we call an "area" cache, which caches a
 * contiguous block of PointSets. In general, if it is known that a
 * particular application access data in a <b>sequential</b> manner,
 * it is generally more useful to make the LRU cache small (size 1)
 * and the area cache large. If the application uses mostly <b>random
 * access</b>, it is better to have a large LRU cache and a very small
 * area cache.
 */
class OnDiskPointSetList : public PointSetList {
public:
  /// Load from a particular file, using default cache sizes.
  /**
   * Makes one pass through the entire data set for
   * preprocessing. Aborts if <filename> is not valid or not
   * readable.
   */
  OnDiskPointSetList(string filename);

  /// Load from a particular file, with specified cache sizes.
  /**
   * Makes one pass through the entire data set for
   * preprocessing. Aborts if <filename> is not valid or not
   * readable.
   */
  OnDiskPointSetList(string filename, int lru_cache_size,
                     int area_cache_size);
  virtual ~OnDiskPointSetList();

  virtual int point_set_size() const;
  virtual int point_size() const;
  virtual int point_dim() const;
  virtual bool GetSetCardinalities(vector<int>* destination) const;

  /// \brief Return a reference to the <index>th PointSet.
  /// <b>READ WARNING BELOW!</b>
  /**
   * Let A be the size of the area cache, and L be the size of the
   * LRU cache.  First, check to see if it's in the area cache, and
   * return it if it's there(O(1)). If it's not there, we check the
   * LRU cache (O(L)). If it's there, move it to the front of the LRU
   * cache and return it. Otherwise, we insert it into the LRU cache,
   * and also fill up the area cache starting at the element we've
   * just retrieved (O(A)).
   *
   * <b>Warning</b>: The reference returned
   * by this operator is no longer valid the next time operator[] is
   * called. This means that whenever you want to access more than
   * one PointSet at a time, you must make a copy of them!
   */
  virtual const PointSet& point_set(int index) const;

  /// Default value: 1500
  static const int DEFAULT_LRU_CACHE_SIZE;

  /// Default value: 1500
  static const int DEFAULT_AREA_CACHE_SIZE;

private:
  // Returns true iff the <index>th point set in the whole list is
  // in the area cache.
  bool IsInAreaCache(int index) const;

  // Seeks to the <index>th point set list on the disk and fills up
  // the area cache with
  //   <index>, <index> + 1, ..., <index> + cache_size_ - 1.
  //
  // If the end is reached, then cache_ will just end there, so
  // the value of cache_.size() may not always be equal to
  // this.cache_size_ .
  // The resulting location of the file pointer will be the very next
  // point set (i.e., the one that would follow the point set at the
  // end of the current cache). It will be EOF if we've hit the end.
  // This function sets cache_offset_ appropriately.
  void SeekAndFillAreaCache(int index) const;


  // Seeks to the start, then scans through and computes
  // num_point_sets_, num_points_, and point_dim_.  It also
  // populates the vectors <pointers_> and <num_points_> It then
  // returns the file pointer to where it was originally.
  void GetListInfo() const;

  // A vector of size <num_point_sets_>. The value of pointers_[ii]
  // gives you the position in the stream where you can find the
  // iith point set.
  mutable vector<streampos> pointers_;

  // A vector of size <num_point_sets_> which tells you how many points
  // there are in each point set.
  mutable vector<int> num_points_per_set_;

  // Useful metadata
  mutable int num_point_sets_;
  mutable int num_points_;
  mutable int point_dim_;
  mutable ifstream input_stream_;

  // cache_[0] is the <cache_offset_>th point set in the whole list.
  int max_lru_size_;
  int max_area_size_;

  mutable int area_cache_offset_;

  // The LRU cache is such that the most-recently-used is at the front
  // and least-recently-used (to evict) is at the back. We need to store
  // pairs. The first element in each pair is the index of that point in
  // the entire point set list, and the second element in the pair is
  // the point set itself.
  mutable list<pair<int, PointSet> > lru_cache_;
  mutable vector<PointSet> area_cache_;

  // Disallow evil constructors (don't allow users to make copies of
  // OnDiskPointSetLists)
  OnDiskPointSetList& operator=(const OnDiskPointSetList& foo);
  OnDiskPointSetList(const OnDiskPointSetList& foo);

};
}  // namespace libpmk

#endif  // POINT_SET_ON_DISK_POINT_SET_LIST_H
