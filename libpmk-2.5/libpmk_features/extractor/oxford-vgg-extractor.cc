// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of OxfordVGGExtractor.
//

#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <Magick++.h>
#include "extractor/oxford-vgg-extractor.h"
#include "core/interest-point.h"
#include "point_set/point.h"
#include "point_set/point-set.h"

using namespace libpmk;
using namespace Magick;

namespace libpmk_features {

OxfordVGGExtractor::OxfordVGGExtractor(const string& flags) :
  ImageExtractor("png"),
  binary_path_("./compute_descriptors.ln"), flags_(flags),
  truncate_vector_(false), truncated_dim_(36) {
  AddAcceptedFormat("pnm");
}

void OxfordVGGExtractor::SetBinaryPath(const string& path) {
  binary_path_ = path;
}

void OxfordVGGExtractor::SetTruncateDim(int dim) {
  assert(dim <= 36);
  truncated_dim_ = dim;
}

void OxfordVGGExtractor::GetFeatures(
  const string& filename, const Image&,
  const vector<InterestPoint>& points,
  PointSet* out_features) const {
  char cmd[4096]; 
  char input_points_fifo[1024];
  char output_descriptors_fifo[1024];

  // This pipe will send the interest points into the program:
  // TODO(jjl): right now, the input-interest-points is not sent
  // through a file, it is actually written to disk.
  sprintf(input_points_fifo, "/tmp/%d.XXXXXX", getpid());
  int input_points_fd = mkstemp(input_points_fifo);
  FILE* input_points = fdopen(input_points_fd, "w");

  // This pipe will read out the descriptors from the program:
  sprintf(output_descriptors_fifo, "/tmp/%d.XXXXXX", getpid());
  int fd = mkstemp(output_descriptors_fifo);
  close(fd);
  mkfifo(output_descriptors_fifo, 0600);

  int fd2 = open(output_descriptors_fifo, O_RDONLY | O_NONBLOCK);
  FILE* output_descriptors = fdopen(fd2, "r");

  sprintf(cmd, "%s %s -i %s -p1 %s -o1 %s",
          binary_path_.c_str(), flags_.c_str(),
          filename.c_str(), input_points_fifo,
          output_descriptors_fifo);
  
  // Write the interest points into the input fifo:
  fprintf(input_points, "1.0\n");
  fprintf(input_points, "%d\n", (int)points.size());
  for (int ii = 0; ii < (int)points.size(); ++ii) {
    fprintf(input_points, "%f %f %f %f %f\n",
            points[ii].x, points[ii].y,
            points[ii].a, points[ii].b, points[ii].c);
  }

  fclose(input_points);
   
  // Actually run the command, sending the output to the fifo:
  FILE *p = popen(cmd, "r");
  char buffer;
  while (!feof(p)) {
    assert(fscanf(p, "%c", &buffer) != 0);
  }
  pclose(p);

  int output_dim;
  assert(fscanf(output_descriptors, "%d", &output_dim) != 0);
   
  int num_descriptors;
  assert(fscanf(output_descriptors, "%d", &num_descriptors) != 0);

  float x, y, a, b, c;

  int desired_dim;

  if (truncate_vector_) {
    desired_dim = truncated_dim_;
  } else {
    desired_dim = output_dim;
  }

  out_features->set_point_dim(desired_dim);
  for (int ii = 0; ii < num_descriptors; ++ii) {
    assert(fscanf(output_descriptors, "%f %f %f %f %f",
                  &x, &y, &a, &b, &c) != 0);
    Point* added_point = out_features->add_point();
    for (int jj = 0; jj < desired_dim; ++jj) {
      float data;
      assert(fscanf(output_descriptors, "%f", &data) != 0);
      added_point->set_feature(jj, data);
    }

    // Ignore the rest of the dims if we are truncating:
    if (truncate_vector_) {
      for (int jj = desired_dim; jj < output_dim; ++jj) {
        float data;
        assert(fscanf(output_descriptors, "%f", &data) != 0);
      }
    }
  }

  fclose(output_descriptors);
  remove(input_points_fifo);
  remove(output_descriptors_fifo);
}
}  // end namespace libpmk_features
