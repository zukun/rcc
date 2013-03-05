// Copyright 2007, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef EXTRACTOR_EXTRACTOR_H
#define EXTRACTOR_EXTRACTOR_H

#include <string>
#include <vector>
#include "core/interest-point.h"
#include "point_set/point-set.h"
#include "point_set/point.h"

using namespace std;
using namespace libpmk;

namespace libpmk_features {

/// The Extractor superclass, which extracts features from files.
/**
 * This abstract class only one method to implement: ExtractFeatures.
 *
 * ExtractFeatures performs all of the work. The input is a filename
 * and a set of InterestPoints. The output is a PointSet.
 *
 * The most common use for this class is to extract features from
 * images.  Rather than directly subclassing Extractor, it is a better
 * idea to subclass ImageExtractor for that purpose, since it provides
 * a lot more functionality. See image-extractor.h for details.
 */
class Extractor {
public:
  Extractor();
  virtual ~Extractor() { }

  /// Returns a PointSet containing features for this image.
  /**
   * The interest point vector <points> might be modified-- some
   * points may be deleted from it if features cannot be extracted
   * from that patch. The resulting PointSet will have a size equal to
   * that of the resulting <points> vector and they will be in the
   * same order.
   *
   * <removed_points> will be populated with any interest points that
   * might have been filtered out. If it is NULL, nothing will happen to
   * it. If it's not NULL, then it will be populated.
   */
  virtual PointSet ExtractFeatures(
    const string& image,
    vector<InterestPoint>* points,
    vector<InterestPoint>* removed_points = NULL) const = 0;


protected:
  /// Indicates whether an extracted feature point is bogus.
  /**
   * By default, this returns true. Override this only when there are
   * some known conditions where a returned point is no good; for
   * example, the Oxford VGG binaries will often return nans or bogus
   * values on certain kinds of patches. The OxfordVGGExtractors have
   * overridden this method to return true for those. If this returns
   * true, then ExtractFeatures() will automatically filter these
   * points out.
   */
  virtual bool IsValidPoint(const libpmk::Point& point) const;

  /// Removes points from <points> and <features> using IsValidPoint().
  /**
   * This is a convenience function for subclasses to
   * use. ImageExtractor uses this to postprocess results. Given a set
   * of interest points and a PointSet of the same size, it will test
   * each Point in the PointSet with IsValidPoint, removing the Points
   * (and the corresponding InterestPoints) for the ones that are not
   * valid.
   *
   * <removed_points> will be populated with any interest points that
   * might have been filtered out. If it is NULL, nothing will happen to
   * it. If it's not NULL, then it will be populated.
   */
  void FilterPoints(vector<InterestPoint>* points, PointSet* features,
                    vector<InterestPoint>* removed_points = NULL) const;
  
};
}  // namespace libpmk_features

#endif  // EXTRACTOR_EXTRACTOR_H


