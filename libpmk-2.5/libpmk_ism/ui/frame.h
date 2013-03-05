// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef MULTIOBJ_UI_FRAME_H
#define MULTIOBJ_UI_FRAME_H

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/stock.h>
#include <gtkmm.h>
#include "ui/image-box.h"
#include "ui/selecting-image-box.h"
#include "ui/training-set-view.h"

class Frame : public Gtk::Window {
public:
  Frame();
  virtual ~Frame() { }

  ImageBox* mutable_preview_box() { return &object_preview_; }
  Gtk::HPaned* mutable_top_level_pane() { return &top_level_pane_; }

  void LoadNewImage(const Glib::ustring& filename);

protected:
  // The paned windows are contained within the frame box.
  Gtk::VBox frame_box_;
  Gtk::HPaned top_level_pane_;
  Gtk::VPaned description_pane_;

  // This is the main image display on the left side, which is
  // clickable.
  Gtk::VBox main_display_box_;
  SelectingImageBox image_display_;
  Gtk::HBox main_controls_box_;
  Gtk::Button detect_objects_button_;
  Gtk::Adjustment clustering_spinner_adjustment_;
  Gtk::Adjustment hypothesis_spinner_adjustment_;
  Gtk::SpinButton clustering_threshold_spinner_;
  Gtk::SpinButton hypothesis_threshold_spinner_;


  // This is the upper right side, which shows a preview of the
  // selected box.
  Gtk::VBox object_preview_box_;
  Gtk::HBox object_preview_details_box_;
  Gtk::VBox object_preview_button_box_;
  Gtk::Label object_preview_label_;
  Gtk::Alignment object_preview_alignment_;
  ImageBox object_preview_;
  Gtk::Button add_to_training_set_button_;
  Gtk::Button show_features_button_;

  // This is the lower right side, which shows the training set.
  Gtk::VBox training_set_box_;
  Gtk::Label training_set_view_label_;
  Gtk::HBox training_set_button_box_;
  TrainingSetView training_set_view_;
  Gtk::Button delete_training_example_button_;

  // This is for the UI toolbars.
  Glib::RefPtr<Gtk::UIManager> ui_manager_;
  Glib::RefPtr<Gtk::ActionGroup> action_group_;

  void OnMenuFileNew();
  void OnMenuFileQuit();
  void OnSelectImageRegion(Glib::RefPtr<Gdk::Pixbuf> sub_region);
  void OnClickShowFeatures();
  void OnClickAddToTrainingSet();
  void OnClickDeleteTrainingExample();
  void OnClickDetectObjects();
};

#endif  // MULTIOBJ_UI_FRAME_H
