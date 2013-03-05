// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of MSERDetector.
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

#include "detector/mser-detector.h"

#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

MSERDetector::MSERDetector() : ImageDetector("png"),
                               binary_path_("./mser.ln") {
  SetAcceptedFormats();
}

MSERDetector::MSERDetector(const string& binary_path) :
  ImageDetector("png"), binary_path_(binary_path) {
  SetAcceptedFormats();
}

void MSERDetector::SetBinaryPath(const string& binary_path) {
  binary_path_ = binary_path;
}

void MSERDetector::SetAcceptedFormats() {
  AddAcceptedFormat("png");
  AddAcceptedFormat("tiff");
  AddAcceptedFormat("jpeg");
}

void MSERDetector::GetInterestPoints(
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

  // Set up the command line:
  sprintf(cmd, "%s -i %s -o %s -t 2",
          binary_path_.c_str(), image_filename.c_str(), fifo);


  // Actually run the command, sending the output to the fifo:
  FILE *p = popen(cmd, "r");
  char buffer;
  while (!feof(p)) {
    assert(fscanf(p, "%c", &buffer) != 0);
  }
  pclose(p);


  // Read data from the pipe and store it as InterestPoints:
  float temp;
  assert(fscanf(f, "%f", &temp) != 0);
  if((int)temp != 1)    {
    fprintf(stderr, "FATAL: malformed output from %s: invalid header %f.\n",
            binary_path_.c_str(), temp);
    close(fd);
    fclose(f);
    unlink(fifo);
    exit(0);
  }

  int num_points;
  assert(fscanf(f, "%d", &num_points) != 0);

  for (int ii = 0; ii < num_points; ++ii) {
    InterestPoint point;
    assert(fscanf(f, "%f %f %f %f %f", &point.x, &point.y, &point.a,
                  &point.b, &point.c) != 0);
    out_points->push_back(point);
  }


  close(fd);
  fclose(f);
  unlink(fifo);
}

}  // end namespace libpmk_features
