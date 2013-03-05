// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <iostream>
#include <gtkmm.h>

#include "ui/frame.h"

#include "detector/harris-detector.h"
#include "extractor/sift-extractor.h"
#include "point_set/mutable-point-set-list.h"
#include "shape_model/implicit-shape-model.h"
#include "shape_model/object-position-vote.h"
#include "ui/label-input-dialog.h"

using namespace libpmk_features;
using namespace libpmk_shape_model;

Frame::Frame() :
  clustering_spinner_adjustment_(-30000, -1000000, 0, 10000, 100000),
  hypothesis_spinner_adjustment_(0.07, 0.0001, 3, 0.001, 0.1),
  clustering_threshold_spinner_(clustering_spinner_adjustment_),
  hypothesis_threshold_spinner_(hypothesis_spinner_adjustment_, 0.01, 5) {
  // Set up the window parameters:
  set_border_width(10);
  resize(1024,768);

  add(frame_box_);

  // Set up the UI element layout:
  top_level_pane_.add1(main_display_box_);
  top_level_pane_.add2(description_pane_);
  description_pane_.add1(object_preview_box_);
  description_pane_.add2(training_set_box_);

  // The left pane:
  main_display_box_.pack_start(image_display_, Gtk::PACK_EXPAND_WIDGET, 0);
  main_display_box_.pack_start(main_controls_box_, Gtk::PACK_SHRINK, 0);
  main_controls_box_.pack_start(detect_objects_button_, Gtk::PACK_SHRINK, 0);
  main_controls_box_.pack_start(clustering_threshold_spinner_,
                                Gtk::PACK_SHRINK, 0);
  main_controls_box_.pack_start(hypothesis_threshold_spinner_,
                                Gtk::PACK_SHRINK, 0);

  detect_objects_button_.set_label("Detect objects");

  // The top-right pane:
  object_preview_label_.set_text("Selection");
  object_preview_box_.pack_start(object_preview_label_, Gtk::PACK_SHRINK, 0);
  object_preview_box_.pack_start(object_preview_details_box_,
                                 Gtk::PACK_EXPAND_WIDGET, 0);
  object_preview_details_box_.pack_start(object_preview_,
                                         Gtk::PACK_EXPAND_WIDGET, 0);
  object_preview_details_box_.pack_start(object_preview_button_box_,
                                         Gtk::PACK_SHRINK, 0);
  add_to_training_set_button_.set_label("Add to training set");
  show_features_button_.set_label("Show features");
  object_preview_button_box_.pack_start(add_to_training_set_button_,
                                        Gtk::PACK_SHRINK, 0);
  object_preview_button_box_.pack_start(show_features_button_,
                                        Gtk::PACK_SHRINK, 0);
  object_preview_button_box_.set_homogeneous(false);

  // The bottom-right pane:
  training_set_view_label_.set_text("Training Set");
  training_set_box_.pack_start(training_set_view_label_, Gtk::PACK_SHRINK, 0);
  training_set_box_.add(training_set_view_);
  training_set_box_.pack_end(training_set_button_box_, Gtk::PACK_SHRINK, 0);
  delete_training_example_button_.set_label("Remove from training set");
  training_set_button_box_.pack_start(delete_training_example_button_,
                                      Gtk::PACK_SHRINK, 0);

  // Set up the toolbar:
  action_group_ = Gtk::ActionGroup::create();
  action_group_->add(Gtk::Action::create("FileNewStandard",
                                         Gtk::Stock::NEW, "Open",
                                         "Open a new image"),
                     sigc::mem_fun(*this, &Frame::OnMenuFileNew));
  action_group_->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
                     sigc::mem_fun(*this, &Frame::OnMenuFileQuit));
  
  ui_manager_ = Gtk::UIManager::create();
  ui_manager_->insert_action_group(action_group_);
  Glib::ustring ui_info =
    "<ui>"
    "  <toolbar name='ToolBar'>"
    "    <toolitem action='FileNewStandard'/>"
    "    <toolitem action='FileQuit'/>"
    "  </toolbar>"
    "</ui>";

  ui_manager_->add_ui_from_string(ui_info);  

  Gtk::Widget* toolbar = ui_manager_->get_widget("/ToolBar");
  if (toolbar) {
    frame_box_.pack_start(*toolbar, Gtk::PACK_SHRINK);
  }


  // Signals:
  // Set up the main image display so that we can appropriately respond
  // when someone selects a subregion of that.
  image_display_.signal_region_selected().connect(
    sigc::mem_fun(*this, &Frame::OnSelectImageRegion));

  // "Add to training set" button
  add_to_training_set_button_.signal_clicked().connect(
    sigc::mem_fun(*this, &Frame::OnClickAddToTrainingSet));

  // "Show features" button
  show_features_button_.signal_clicked().connect(
    sigc::mem_fun(*this, &Frame::OnClickShowFeatures));
  
  // "Remove from training set" button
  delete_training_example_button_.signal_clicked().connect(
    sigc::mem_fun(*this, &Frame::OnClickDeleteTrainingExample));

  // "Detect objects" button
  detect_objects_button_.signal_clicked().connect(
    sigc::mem_fun(*this, &Frame::OnClickDetectObjects));

  frame_box_.pack_end(top_level_pane_);
  top_level_pane_.set_position(640);
  description_pane_.set_position(300);
  show_all_children();  
}


void Frame::LoadNewImage(const Glib::ustring& filename) {
  image_display_.LoadNewImage(filename);
  if (top_level_pane_.get_position() < image_display_.image_width()) {
    top_level_pane_.set_position(image_display_.image_width());
  }
}

void Frame::OnMenuFileQuit() {
  hide();
}

void Frame::OnMenuFileNew() {
  Gtk::FileChooserDialog dialog("Please choose a new image file",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  Gtk::FileFilter filter_images;
  filter_images.set_name("Image files");
  filter_images.add_pattern("*.jpg");
  filter_images.add_pattern("*.jpeg");
  filter_images.add_pattern("*.png");
  filter_images.add_pattern("*.pnm");
  filter_images.add_pattern("*.ppm");
  filter_images.add_pattern("*.pgm");
  dialog.add_filter(filter_images);
  
  Gtk::FileFilter filter_any;
  filter_any.set_name("All files");
  filter_any.add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  switch (result) {
  case Gtk::RESPONSE_OK:
    LoadNewImage(dialog.get_filename());
    break;
  case Gtk::RESPONSE_CANCEL:
    break;
  }
}

void Frame::OnSelectImageRegion(Glib::RefPtr<Gdk::Pixbuf> sub_region) {
  object_preview_.LoadNewImage(sub_region);
}

void Frame::OnClickShowFeatures() {
  if (object_preview_.has_image()) {
    HarrisDetector detector;
    detector.SetBinaryPath("~/bin/h_affine.ln");
    detector.SetDetectorType(HarrisDetector::kHarrisLaplace);
    //detector.SetDetector(HESSIAN_AFFINE);
    //GridDetector detector(8, 8, 8);
    object_preview_.mutable_image()->DetectInterestPoints(detector, 15);
    object_preview_.Redraw();
  }
}

void Frame::OnClickAddToTrainingSet() {
  if (object_preview_.has_image()) {
    LabelInputDialog dialog(*this);
    int result = dialog.run();
    switch (result) {
    case Gtk::RESPONSE_OK:
      training_set_view_.AddExample(
        dialog.label(),
        object_preview_.mutable_image()->mutable_pixbuf());
      break;
    }
  }
}

void Frame::OnClickDeleteTrainingExample() {
  training_set_view_.DeleteSelectedExample();
}

void Frame::OnClickDetectObjects() {
  // TODO: factor this code out to a better place.
  image_display_.ClearAllOverlays();

  if (image_display_.has_image() && training_set_view_.size() > 0) {
    HarrisDetector detector;
    //GridDetector detector(8, 8, 8);
    detector.SetBinaryPath("~/bin/h_affine.ln");
    detector.SetDetectorType(HarrisDetector::kHarrisLaplace);
    //detector.SetDetector(HESSIAN_AFFINE);

    SIFTExtractor extractor;
    extractor.SetBinaryPath("~/bin/compute_descriptors.ln");

    // Train the shape model:
    ImplicitShapeModel model;
    MutablePointSetList features;
    MutablePointSetList locations;
    vector<double> scales;
    
    for (int ii = 0; ii < training_set_view_.size(); ++ii) {
      FeaturedImage* image = training_set_view_.mutable_image(ii);
      int halfwidth = image->pixbuf()->get_width() / 2;
      int halfheight = image->pixbuf()->get_height() / 2;

      // Detect the interest points:
      image->DetectInterestPoints(detector, 15);
      image->ExtractFeatures(extractor);
      if (image->has_features()) {
        features.add_point_set(image->features());
        PointSet* added_locs = locations.add_point_set(image->locations());
        // We need to shift all of the locations so that they represent
        // offsets relative to the image CENTER, not the top left corner.
        for (int jj = 0; jj < added_locs->size(); ++jj) {
          libpmk::Point* point = added_locs->mutable_point(jj);
          point->set_feature(0, point->feature(0) - halfwidth);
          point->set_feature(1, point->feature(1) - halfheight);
        }

        const vector<double>& local_scales(image->feature_scales());
        for (int jj = 0; jj < (int)local_scales.size(); ++jj) {
          scales.push_back(local_scales[jj]);
        }
      }
    }

    double THRESHOLD = clustering_threshold_spinner_.get_value();
    if (features.point_size() == 0) {
      Gtk::MessageDialog dialog(*this, "No features");
      dialog.set_secondary_text(
        "The training set didn't have any extracted features.");
      
      dialog.run();
      return;
    }
    model.Train(features, locations, scales, THRESHOLD);

    // Apply it to the shown image:
    FeaturedImage* test_image = image_display_.mutable_image();
    test_image->DetectInterestPoints(detector, -1);
    test_image->ExtractFeatures(extractor);

    if (test_image->has_features()) {
      vector<ObjectPositionVote> votes;
      model.GetProbabalisticVotes(test_image->features(),
                                  test_image->locations(),
                                  test_image->feature_scales(),
                                  THRESHOLD, &votes);
      ImageOverlay* new_overlay = image_display_.add_overlay();
      
      for (int kk = 0; kk < (int)votes.size(); ++kk) {
        const ObjectPositionVote& vote(votes[kk]);
        new_overlay->AddLine(Gdk::Color("Black"),
                             vote.source_x(), vote.source_y(),
                             vote.vote_x(), vote.vote_y());
        new_overlay->AddPoint(Gdk::Color("White"),
                              vote.vote_x(), vote.vote_y());
      }
      image_display_.Redraw();

      vector<pair<int, int> > hypotheses;
      vector<double> scores;
      model.GetObjectHypotheses(votes,
                                image_display_.image_width(),
                                image_display_.image_height(),
                                15,
                                hypothesis_threshold_spinner_.get_value(),
                                &hypotheses, &scores);

      ImageOverlay* hypothesis_overlay = image_display_.add_overlay();
      for (int kk = 0; kk < (int)hypotheses.size(); ++kk) {
        hypothesis_overlay->AddPoint(Gdk::Color("Green"),
                                     hypotheses[kk].first,
                                     hypotheses[kk].second);
      }
      image_display_.Redraw();
    }
  }
}
