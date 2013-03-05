// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Encapsulates the functionality for transparently converting between
// image formats when needed.
//

#ifndef IMAGES_IMAGE_LOADER_H
#define IMAGES_IMAGE_LOADER_H

#include <set>
#include <string>
#include <Magick++.h>

using namespace std;
using namespace Magick;

namespace libpmk_features {

/// Convenience class for loading and converting images.
/**
 * ImageLoader knows how to read images from files and write to them
 * in any format known by the ImageMagick library. There are two
 * concepts that ImageLoader rests on: the *accepted* formats and the
 * *preferred* format.
 *
 * In both cases, the "format" is simply a string, such as "gif" or
 * "jpeg" or "png" describing the image format. The string is
 * case-insensitive. The exact list of valid strings is defined by the
 * ImageMagick library on your machine.
 *
 * If ImageLoader reads an image that is in an accepted format, it
 * will just load it normally without doing anything else.
 *
 * If it's not in an accepted format, it will first convert the image
 * to the preferred format before loading. (Obviously, the preferred
 * format is automatically going to be one of the accepted formats).
 *
 * You can specify which formats are accepted formats with
 * AddAcceptedFormat(). The preferred format is specified in the
 * constructor.
 *
 */
class ImageLoader {
public:
  /// \brief Constructor; automatically adds <preferred_format> as an
  /// accepted format.
  ImageLoader(const string& preferred_format);

  /// Returns true if this Loader can accept images in the given format.
  /**
   * The string is case-insensitive and can be things like "jpeg", "png",
   * etc. The exact list of valid strings is defined by the ImageMagick
   * library. Some known ones:
   * jpeg
   * png
   * tiff
   * gif
   * pnm    [this includes pgm, ppm]
   */
  bool AcceptsFormat(const string& format) const;

  /// Accept another format.
  /**
   * The format string is case-insensitive.
   */
  void AddAcceptedFormat(const string& format);

  /// Loads a new image into <out_image>.
  /**
   * This loads the image at the given filename into <out_image>. It
   * checks the format of that image. If that image is NOT one of the
   * accepted formats, it will do the following:
   *
   * 1. Create a temp file<br>
   * 2. Convert the image to the preferred format and save it in
   *    that temp file<br>
   * 3. Return the image data in <out_image> and the path to the temp
   *    file in <out_filename><br>
   * 4. Return FALSE.<br><br>
   *
   * It will return true if no conversion was needed (and thus no temp
   * file), i.e., the image was already in one of the accepted
   * formats. In this case, <out_filename> is set in such a way that
   * it is equal to <filename>.
   *
   * IMPORTANT NOTE: this creates temp files, but does not delete
   * them.  It is the caller's responsibility to delete them. That is,
   * whenever this method returns false, the caller should take care
   * to call unlink(out_filename).
   *
   * For an example of this happening, see the code for
   * ImageExtractor::ExtractFeatures(). It handles the automatic deletion
   * of the converted temp-image.
   */
  bool LoadImage(const string& filename,
                 Magick::Image* out_image, string* out_filename) const;

private:
  // Stores, in lower case, the set of accepted image formats, such as
  // "jpg", "png", etc. The set of valid strings is defined by the
  // ImageMagick library; this class simply passes these strings to
  // libmagick++.
  set<string> accepted_formats_;

  // If the image isn't in an accepted format, the detector will convert
  // the image to this format.
  string preferred_format_;
};

}  // end namespace libpmk_features
#endif  // IMAGES_IMAGE_LOADER_H
