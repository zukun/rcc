// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of ImageLoader.
//

#include <cassert>
#include <set>
#include <string>
#include <Magick++.h>

#include "images/image-loader.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

ImageLoader::ImageLoader(const string& preferred_format) :
  preferred_format_(preferred_format) {
  accepted_formats_.insert(preferred_format);
}

bool ImageLoader::AcceptsFormat(const string& format) const {
  string query = format;
  std::transform(query.begin(), query.end(), query.begin(), ::tolower);
  return accepted_formats_.find(query) != accepted_formats_.end();
}

void ImageLoader::AddAcceptedFormat(const string& format) {
  string added = format;
  std::transform(added.begin(), added.end(), added.begin(), ::tolower);
  accepted_formats_.insert(added);
}

bool ImageLoader::LoadImage(const string& filename,
                            Image* out_image, string* out_filename) const {
  assert(out_image != NULL);
  assert(out_filename != NULL);
  out_image->read(filename);

  if (AcceptsFormat(out_image->magick())) {
    *out_filename = filename;
    return true;
  } else {
    out_image->magick(preferred_format_);
  
    char temp_file_name[1024];
    sprintf(temp_file_name, "/tmp/libpmk_loader.%d.XXXXXX", getpid());
    int fd = mkstemp(temp_file_name);
    close(fd);
  
    out_image->write(temp_file_name);
    *out_filename = temp_file_name;
    return false;
  }
}
}  // end namespace libpmk_features
