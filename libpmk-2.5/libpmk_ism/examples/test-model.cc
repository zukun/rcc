// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Given a ModelGroup and PointSets (features + locs) of test images,
// outputs for each one the predicted label, the predicted X, 
// predicted Y coordinate, and score.
//
// N.B.: in this program, the paths to the detector/extractor binaries
// are hardcoded. See lines 87 and 90.
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

// Note that this one differs from the one in train-model.cc in that
// it does not need to shift the X/Y coordinates to make 0,0 the
// center.  When this gets integrated into the LIBPMK library it should
// be factored out.
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

    for (int ii = 0; ii < (int)interest_points->size(); ++ii) {
      Point* added_point = returned.add_point();
      added_point->set_feature(0, interest_points->at(ii).x);
      added_point->set_feature(1, interest_points->at(ii).y);
      added_point->set_feature(2, interest_points->at(ii).CircleRadius());
    }
    return returned; 
  }
};


void usage(const char* argv) {
  cerr << "Usage: " << argv << " <models_file> <rnn_threshold> <test_image> <OUTPUT_FILE>" << endl;
}

int main(int argc, char** argv) {
  if (argc != 5) {
    usage(argv[0]);
    exit(0);
  }

  string model_file(argv[1]);
  double threshold = atof(argv[2]);
  string image_file(argv[3]);
  string result_file(argv[4]);

  // Feature extraction:
  MutablePointSetList features;
  MutablePointSetList locations;
  HarrisDetector detector;
  detector.SetDetectorType(HarrisDetector::kHarrisLaplace);
  detector.SetBinaryPath("features/bin/h_affine.ln");

  SIFTExtractor extractor;
  extractor.SetBinaryPath("features/bin/compute_descriptors.ln");

  LocationScaleExtractor loc_extractor;

  cout << "Extracting features from " << image_file << "... " << flush;
  vector<InterestPoint> interest_points;
  detector.DetectInterestPoints(image_file, &interest_points);
  PointSet feat = extractor.ExtractFeatures(image_file, &interest_points);
  PointSet locs = loc_extractor.ExtractFeatures(image_file, &interest_points);
  features.add_point_set(feat);
  locations.add_point_set(locs);
  cout << "done. " << (int)interest_points.size() << " features." << endl;
  

  // Read the model:
  cout << "Reading trained model from " << model_file << "... " << flush;
  ModelGroup model_group;
  ifstream input_stream(model_file.c_str(), ios::binary);
  model_group.ReadFromStream(input_stream);
  input_stream.close();
  cout << "done." << endl;

  for (int ii = 0; ii < features.point_set_size(); ++ii) {
    const PointSet& feat(features.point_set(ii));
    const PointSet& locs(locations.point_set(ii));
    vector<double> scales;
    for (int jj = 0; jj < locs.size(); ++jj) {
      scales.push_back(locs.point(jj).feature(2));
    }

    int label;
    double x, y;
    double score;

    cout << "Running ISM... " << flush;
    model_group.Test(feat, locs, scales, threshold, &label, &x, &y, &score);
    cout << "done." << endl;

    cout << "Writing result to " << result_file << "... " << flush;
    FILE* f = fopen(result_file.c_str(), "w+");
    fprintf(f, "%f %f %f\n", x, y, score);
    fclose(f);
    cout << "done." << endl;
  }
  return 0;
}
