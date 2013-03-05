// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// An abstract superclass of Extractors that operate on images (as
// opposed to, say, videos).
//

#ifndef EXTRACTOR_IMAGE_EXTRACTOR_H
#define EXTRACTOR_IMAGE_EXTRACTOR_H

#include <string>
#include <vector>
#include <Magick++.h>

#include "extractor/extractor.h"

#include "core/interest-point.h"
#include "images/image-loader.h"
#include "point_set/point-set.h"
#include "point_set/point.h"

using namespace std;
using namespace libpmk;
using namespace Magick;

namespace libpmk_features {

/// Superclass for converting images to features.
/**
 * This abstract class has only one method to implement: GetFeatures().
 *
 * This class takes care of things like converting images to the
 * proper format and loading it. Thus, you do not need to override
 * ExtractFeatures(); instead, override GetFeatures(), which will
 * provide the image data itself. You can get useful information from
 * this, such as the width or height.
 *
 * On image formats: the ImageExtractor has the notions of *accepted* and
 * *preferred* image formats. An accepted image format is one that the
 * extractor is able to read; for example, some of the Oxford VGG
 * binaries used by this code require the image to be a PNG or PNM,
 * so those would be the two accepted formats.
 *
 * If the input image is not in one of the accepted formats, ImageExtractor
 * will automatically convert the image to the preferred format.  The
 * preferred format is defined in the constructor of ImageExtractor. All
 * subclasses must provide a preferred format. This preferred format,
 * obviously, automatically gets treated as one of the accepted
 * formats as well.
 *
 * The formats are specified by a string such as "png", "jpeg", or
 * "pnm". The exact list of formats supported depends on your
 * ImageMagick library; these strings are simply passed to
 * ImageMagick, so if ImageMagick can understand the format string,
 * then so will the ImageExtractor.
 *
 */
class ImageExtractor : public Extractor {
public:
  ImageExtractor(const string& preferred_format);
  virtual ~ImageExtractor() { }

  // Returns a PointSet containing the features for this image. The
  // interest point vector <points> may be modified-- some points may
  // be deleted from it if features cannot be extracted from that
  // patch. The resulting PointSet will have a size equal to that of
  // the resulting <points> vector and they will be in the same order.
  virtual PointSet ExtractFeatures(
    const string& image,
    vector<InterestPoint>* points,
    vector<InterestPoint>* removed_points = NULL) const;

  void AddAcceptedFormat(const string& format);

protected:
  // Given a path to an image and a set of interest points, populate
  // out_features with the features, one per InterestPoint and in the
  // same order. out_features is not NULL. It must be cleared first.
  // It is recommended to put the following at the top of your GetFeatures()
  // implementation:
  //
  // out_features->set_point_dim(YOUR_POINT_DIM);
  virtual void GetFeatures(const string& filename,
                           const Magick::Image& image,
                           const vector<InterestPoint>& points,
                           PointSet* out_features) const = 0;

private:
  ImageLoader image_loader_;
};

}  // namespace libpmk_features

#endif  // EXTRACTOR_IMAGE_EXTRACTOR_H
