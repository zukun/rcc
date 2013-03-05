// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of SURFDetector.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <Magick++.h>

#include "detector/surf-detector.h"

#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

SURFDetector::SURFDetector() : ImageDetector("pgm"),
                               binary_path_("./surf.ln"),
                               threshold_(1000) { }

SURFDetector::SURFDetector(const string& binary_path) :
  ImageDetector("pgm"), binary_path_(binary_path), threshold_(1000) { }

void SURFDetector::SetBinaryPath(const string& binary_path) {
  binary_path_ = binary_path;
}

void SURFDetector::SetThreshold(int threshold) {
  threshold_ = threshold;
}

void SURFDetector::GetInterestPoints(
  const string& image_filename, const Image&,
  vector<InterestPoint>* out_points) const {
  assert(NULL != out_points);
  out_points->clear();

  char cmd[1024];
  char fifo[1024];

  // Create a pipe. We will read from this pipe, and have
  // the detector binary send its output to this pipe.
  sprintf(fifo, "/tmp/DETECTOR.%d.XXXXXX", getpid());
  int fd = mkstemp(fifo);

  // Must open for non-blocking IO, so the fdopen does not wait for input
  fcntl(fd, F_SETFL, O_RDONLY | O_NONBLOCK);
  mkfifo(fifo, 0600);

  FILE *f = fdopen(fd, "r");

  sprintf(cmd, "%s -i %s -thres %d -q -o %s",
          binary_path_.c_str(), image_filename.c_str(),
          threshold_, fifo);


  // Actually run the command, sending the output to the fifo:
  FILE *p = popen(cmd, "r");
  char buffer;
  while (!feof(p)) {
    assert(fscanf(p, "%c", &buffer) != 0);
  }
  pclose(p);


  // Read data from the pipe and store it as InterestPoints:

  // The README for the surf binary describes the header integer to be
  // "1 + length of descriptor". The extra 1 is for the value of the
  // Laplacian. In this code though, "descriptor_length" includes the
  // Laplacian.
  int descriptor_length;
  assert(fscanf(f, "%d", &descriptor_length) != 0);

  int num_points;
  assert(fscanf(f, "%d", &num_points) != 0);

  for (int ii = 0; ii < num_points; ++ii) {
    InterestPoint point;
    assert(fscanf(f, "%f %f %f %f %f", &point.x, &point.y, &point.a,
                  &point.b, &point.c) != 0);
    out_points->push_back(point);
    for (int jj = 0; jj < descriptor_length; ++jj) {
      float dummy;
      assert(fscanf(f, "%f", &dummy) != 0);
    }
  }

  close(fd);
  fclose(f);
  unlink(fifo);
}

}  // end namespace libpmk_features
