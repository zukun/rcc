// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This takes some images (on the command line) and outputs a trained
// implicit shape model for those images to a file. It is written as a
// ModelGroup of size 1.
//
// N.B.: in this program, the paths to the detector/extractor binaries
// are hardcoded. See lines 84 and 87.
//

#include <Magick++.h>
#include <iostream>
#include <fstream>

#include "detector/harris-detector.h"
#include "extractor/sift-extractor.h"
#include "images/image-loader.h"

#include "point_set/mutable-point-set-list.h"
#include "point_set/point-set-list.h"

#include "shape_model/implicit-shape-model.h"
#include "shape_model/model-group.h"

using namespace Magick;
using namespace libpmk;
using namespace libpmk_features;
using namespace libpmk_shape_model;
using namespace std;

class LocationScaleExtractor : public Extractor {
public:
  virtual ~LocationScaleExtractor() { }
  virtual PointSet ExtractFeatures(
    const string& file, vector<InterestPoint>* interest_points,
    vector<InterestPoint>* removed = NULL) const {
    assert(interest_points != NULL);
    PointSet returned(3);
    ImageLoader loader("png");
    loader.AddAcceptedFormat("jpeg");
    Magick::Image image;
    string read_filename;
    bool in_format = loader.LoadImage(file, &image, &read_filename);
    if (!in_format) {
      unlink(read_filename.c_str());
    }
    int halfwidth = (int)(image.columns() / 2.0);
    int halfheight = (int)(image.rows() / 2.0);
    for (int ii = 0; ii < (int)interest_points->size(); ++ii) {
      Point* added_point = returned.add_point();
      added_point->set_feature(0, interest_points->at(ii).x - halfwidth);
      added_point->set_feature(1, interest_points->at(ii).y - halfheight);
      added_point->set_feature(2, interest_points->at(ii).CircleRadius());
    }
    return returned; 
  }
};



void usage(const char* argv) {
  cerr << "Usage: " << argv << " <output_model_file> <rnn_threshold> <input_files>" << endl;
}

int main(int argc, char** argv) {
  if (argc < 4) {
    usage(argv[0]);
    exit(0);
  }

  string output_file(argv[1]);
  double threshold = atof(argv[2]);

  // Populate the PSLs with the image features:
  MutablePointSetList features;
  MutablePointSetList loc_scales;

  HarrisDetector detector;
  detector.SetDetectorType(HarrisDetector::kHarrisLaplace);
  detector.SetBinaryPath("../../libpmk_features/bin/h_affine.ln");

  SIFTExtractor extractor;
  extractor.SetBinaryPath("../../libpmk_features/bin/compute_descriptors.ln");

  LocationScaleExtractor loc_extractor;

  for (int ii = 3; ii < argc; ++ii) {
    cout << "Extracting features from " << argv[ii] << "... " << flush;
    vector<InterestPoint> interest_points;
    string filename(argv[ii]);
    detector.DetectInterestPoints(filename, &interest_points);

    PointSet feat = extractor.ExtractFeatures(filename, &interest_points);
    PointSet locs = loc_extractor.ExtractFeatures(filename, &interest_points);

    features.add_point_set(feat);
    loc_scales.add_point_set(locs);
    cout << "done. " << (int)interest_points.size() << " features." << endl;
  }

  cout << "Training model... " << flush;
  ModelGroup model_group;
  vector<int> labels;
  for (int ii = 0; ii < (int)features.point_set_size(); ++ii) {
    labels.push_back(1);  // Adjust this as needed
  }

  model_group.Train(features, loc_scales, labels, threshold);
  cout << "done." << endl;

  cout << "Saving model to " << output_file << "... " << flush;
  ofstream output_stream(output_file.c_str(), ios::binary | ios::trunc);
  model_group.WriteToStream(output_stream);
  output_stream.close();
  cout << "done." << endl;
  return 0;
}
