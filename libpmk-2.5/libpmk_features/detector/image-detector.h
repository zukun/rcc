// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// An abstract superclass of Detectors that operate on images (as
// opposed to, say, videos).
//

#ifndef DETECTOR_IMAGE_DETECTOR_H
#define DETECTOR_IMAGE_DETECTOR_H

#include <string>
#include <vector>
#include <Magick++.h>
#include "detector/detector.h"
#include "images/image-loader.h"
#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

/// The ImageDetector superclass converts images to lists of InterestPoints.
/**
 * This abstract class only one method to implement: GetInterestPoints.
 *
 * GetInterestPoints performs the actual interest point detection. No
 * matter what the input image format is, as long as ImageMagick can
 * read it, the Detector will automatically convert it to one of the
 * accepted formats so that the ImageDetector can process it.
 *
 * On image formats: the ImageDetector has the notions of *accepted* and
 * *preferred* image formats. An accepted image format is one that the
 * detector is able to read; for example, some of the Oxford VGG
 * binaries used by this code require the image to be a PNG or PNM,
 * so those would be the two accepted formats.
 *
 * If the input image is not in one of the accepted formats, Detector
 * will automatically convert the image to the preferred format.  The
 * preferred format is defined in the constructor of Detector. All
 * subclasses must provide a preferred format. This preferred format,
 * obviously, automatically gets treated as one of the accepted
 * formats as well.
 *
 * The formats are specified by a string such as "png", "jpeg", or
 * "pnm". The exact list of formats supported depends on your
 * ImageMagick library; these strings are simply passed to
 * ImageMagick, so if ImageMagick can understand the format string,
 * then so will Detector.
 *
 * In summary, to create your own Detector:
 * 1. Choose a preferred format and call Detector's constructor with it
 * 2. In your constructor, also call AddAcceptedFormat() to list the
 *    other formats compatible with your Detector
 * 3. Implement GetInterestPoints(), which provides you with a path to
 *    the image and the image data itself (in a Magick::Image object).
 *    The path will be to a file in one of the accepted formats (again,
 *    the conversion to an accepted format happens transparently, so
 *    you will not have to worry about conversion as long as you have
 *    specified the formats in steps 1 and 2).
 */
class ImageDetector : public Detector {
public:
  ImageDetector(const string& preferred_format);
  virtual ~ImageDetector() { }

  /// Populates out_points with interest points. It must not be NULL.
  /**
   * out_points will be cleared first. The given image filename must
   * exist and be a valid image.
   *
   * If out_width and out_height are not NULL, they are populated with
   * the width and height of the image. If they are NULL, nothing will
   * happen to them.
   */
  virtual void DetectInterestPoints(const string& image_filename,
                                    vector<InterestPoint>* out_points) const;

  void AddAcceptedFormat(const string& format);

  /// Override this to make your own ImageDetector.
  /**
   * Detector::GetInterestPoints() calls this to get its data. The specs
   * for this method are the same, except that you will also have access
   * to the width and height of the image.
   */
  virtual void GetInterestPoints(const string& image_filename,
                                 const Magick::Image& image,
                                 vector<InterestPoint>* out_points) const = 0;

private:
  ImageLoader image_loader_;
};

}  // end namespace libpmk_features
#endif  // DETECTOR_IMAGE_DETECTOR_H
