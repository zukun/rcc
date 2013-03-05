// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of FeaturedImage.


#include <gtkmm.h>
#include "ui/featured-image.h"

const string FeaturedImage::TEMPORARY_FILE_NAME = "/tmp/MULTIOBJ_TMP.png";

FeaturedImage::FeaturedImage() : image_(NULL),
                                 features_(NULL), locations_(NULL) { }

FeaturedImage::FeaturedImage(const Glib::RefPtr<Gdk::Pixbuf>& image) :
  image_(image), features_(NULL), locations_(NULL) { }

void FeaturedImage::set_image(const Glib::ustring& filename) {
  image_.clear();
  interest_points_.clear();
  features_.reset(NULL);
  locations_.reset(NULL);
  image_ = Gdk::Pixbuf::create_from_file(filename);
}

void FeaturedImage::set_image(const Glib::RefPtr<Gdk::Pixbuf>& image) {
  image_.clear();
  interest_points_.clear();
  features_.reset(NULL);
  locations_.reset(NULL);
  image_ = image;
}

void FeaturedImage::DetectInterestPoints(const Detector& detector,
                                         double max_radius) {
  interest_points_.clear();
  vector<InterestPoint> temp;
  image_->save(TEMPORARY_FILE_NAME, "png");
  detector.DetectInterestPoints(TEMPORARY_FILE_NAME, &temp);

  for (int ii = 0; ii < (int)temp.size(); ++ii) {
    if (temp[ii].IsCircle() &&
        (max_radius < 0 || temp[ii].CircleRadius() <= max_radius)) {
      interest_points_.push_back(temp[ii]);
      feature_scales_.push_back(temp[ii].CircleRadius());
    }
  }
  
}

void FeaturedImage::ExtractFeatures(const Extractor& extractor) {
  if (has_interest_points()) {
    image_->save(TEMPORARY_FILE_NAME, "png");
    PointSet extracted = extractor.ExtractFeatures(TEMPORARY_FILE_NAME,
                                                   &interest_points_);
    features_.reset(NULL);
    locations_.reset(NULL);
    if (extracted.size() > 0) {
      features_.reset(new PointSet(extracted.point_dim()));
      locations_.reset(new PointSet(2));
      for (int ii = 0; ii < extracted.size(); ++ii) {
        features_->add_point(extracted.point(ii));
        Point* added_point = locations_->add_point();
        added_point->set_feature(0, interest_points_[ii].x);
        added_point->set_feature(1, interest_points_[ii].y);
      }
    }
  }
}
