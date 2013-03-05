// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This class encapsulates a Gdk Pixbuf (an image), along with a
// set of interest points and descriptors.

#ifndef MULTIOBJ_UI_FEATURED_IMAGE_H
#define MULTIOBJ_UI_FEATURED_IMAGE_H

#include <gtkmm.h>
#include <memory>
#include <vector>
#include "core/interest-point.h"
#include "detector/detector.h"
#include "extractor/extractor.h"
#include "point_set/point-set.h"

using namespace libpmk;
using namespace libpmk_features;
using namespace std;

class FeaturedImage {
public:
  FeaturedImage();
  FeaturedImage(const Glib::RefPtr<Gdk::Pixbuf>& image);
  ~FeaturedImage() { }

  // This is the name of the temp file that is used to store the
  // image data while the feature extraction code runs. This application
  // will only have one instance of this at a time, so it's OK for it
  // to be a constant. Beware, though, if running multiple instances
  // of this application.
  static const string TEMPORARY_FILE_NAME;

  // These simply populate interest_points_ and features_
  // respectively, using libpmk_features. After using the detector, it
  // will filter out the interest points so that only the circular
  // regions remain. Also, it will remove any interest points whose
  // radius is greater than <max_radius> (so max_radius, naturally,
  // should be > 0).
  //
  // Setting max_radius to a negative number will make it accept
  // everything.
  void DetectInterestPoints(const Detector& detector,
                            double max_radius);

  // This will fail silently if we have not called
  // DetectInterestPoints yet. So make sure has_interest_points()
  // returns true before populating this. This will make features()
  // and locations() available to the caller on success.
  void ExtractFeatures(const Extractor& extractor);

  // Getters and setters follow:
  const PointSet& features() const { return *(features_.get()); }
  const PointSet& locations() const { return *(locations_.get()); }
  const vector<InterestPoint>& interest_points() const {
    return interest_points_;
  }
  const vector<double>& feature_scales() const { return feature_scales_; }

  Glib::RefPtr<Gdk::Pixbuf> mutable_pixbuf() { return image_; }
  const Glib::RefPtr<Gdk::Pixbuf>& pixbuf() const { return image_; }

  void set_image(const Glib::RefPtr<Gdk::Pixbuf>& image);
  void set_image(const Glib::ustring& filename);

  // Glib::RefPtr has an operator bool() that returns true if it
  // points to anything and false otherwise.
  bool has_image() const { return image_; }
  bool has_interest_points() { return interest_points_.size() > 0; }
  bool has_features() { return features_.get() != NULL; }
  bool has_locations() { return locations_.get() != NULL; }

private:
  Glib::RefPtr<Gdk::Pixbuf> image_;
  vector<InterestPoint> interest_points_;

  // Records the radius of each patch.
  vector<double> feature_scales_;
  auto_ptr<PointSet> features_;
  auto_ptr<PointSet> locations_;
};

#endif  // MULTIOBJ_UI_FEATURED_IMAGE_H
