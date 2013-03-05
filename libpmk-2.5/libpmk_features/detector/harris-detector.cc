// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of HarrisDetector.
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

#include "detector/harris-detector.h"

#include "core/interest-point.h"

using namespace std;
using namespace Magick;

namespace libpmk_features {

HarrisDetector::HarrisDetector() :
  ImageDetector("png"), binary_path_("./h_affine.ln"), threshold_(0),
  detector_(kHarrisAffine) { }

HarrisDetector::HarrisDetector(const string& binary_path, int threshold,
                               int detector) :
  ImageDetector("png"), binary_path_(binary_path), threshold_(threshold),
  detector_(detector) { }


void HarrisDetector::SetBinaryPath(const string& binary_path) {
  binary_path_ = binary_path;
}

void HarrisDetector::SetThreshold(int threshold) {
  threshold_ = threshold;
}

void HarrisDetector::SetDetectorType(int detector) {
  detector_ = detector;
}

void HarrisDetector::GetInterestPoints(
  const string& image_filename, const Image&,
  vector<InterestPoint>* out_points) const {
  assert(NULL != out_points);
  out_points->clear();

  char cmd[1024];
  char fifo[1024];

  string detector;
  switch (detector_) {
  case kHarris:
    detector = "-har";
    break;
  case kHarrisLaplace:
    detector = "-harlap";
    break;
  case kHessianLaplace:
    detector = "-heslap";
    break;
  case kHarrisAffine:
    detector = "-haraff";
    break;
  case kHessianAffine:
    detector = "-hesaff";
    break;
  default:
    fprintf(stderr, "Warning: invalid detector. Using Harris-affine.\n");
    detector = "-haraff";
    break;
  }

  // Create a pipe. We will read from this pipe, and have
  // the detector binary send its output to this pipe.
  sprintf(fifo, "/tmp/DETECTOR.%d.XXXXXX", getpid());

  // Must open for non-blocking IO, so the fdopen does not wait for input
  int fd = mkstemp(fifo);
  fcntl(fd, F_SETFL, O_RDONLY | O_NONBLOCK);
  mkfifo(fifo, 0600);

  FILE *f = fdopen(fd, "r");

  // Set up the command line:
  if (threshold_ != 0) {
    sprintf(cmd, "%s %s -i %s -o %s -thres %d",
            binary_path_.c_str(), detector.c_str(), image_filename.c_str(),
            fifo, threshold_);
  } else {
    sprintf(cmd, "%s %s -i %s -o %s",
            binary_path_.c_str(), detector.c_str(),
            image_filename.c_str(), fifo);
  }

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
    remove(fifo);
    exit(0);
  }

  int num_points;
  assert(fscanf(f, "%d", &num_points) != 0);

  for (int ii = 0; ii < num_points; ++ii) {
    InterestPoint p;
    assert(fscanf(f, "%f %f %f %f %f", &p.x, &p.y, &p.a, &p.b, &p.c) != 0);
    out_points->push_back(p);
  }

  fclose(f);
  close(fd);
  unlink(fifo);
}


}  // end namespace libpmk_features
