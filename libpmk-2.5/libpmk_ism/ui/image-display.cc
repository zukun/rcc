// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <iostream>

#include "ui/image-display.h"

#include "detector/detector.h"
#include "interest_point.h"
#include "ui/frame.h"

using namespace Gdk;
using namespace std;
using namespace libpmk_features;

ImageDisplay::ImageDisplay(Frame* frame) :
  image_(Pixbuf::create_from_file("easyfaces_multi.png")),
  frame_(frame),
  is_dragging_(false),
  drag_start_x_(0), drag_start_y_(0),
  drag_end_x_(0), drag_end_y_(0),
  extracted_features_(false) {
  set_events(Gdk::BUTTON_PRESS_MASK |
             Gdk::BUTTON_MOTION_MASK |
             Gdk::BUTTON_RELEASE_MASK);
  signal_button_press_event().connect(
    sigc::mem_fun(*this, &ImageDisplay::OnButtonPress));
  signal_button_release_event().connect(
    sigc::mem_fun(*this, &ImageDisplay::OnButtonRelease));
  signal_motion_notify_event().connect(
    sigc::mem_fun(*this, &ImageDisplay::OnMouseMove));
  frame_->mutable_top_level_pane()->set_position(image_->get_width());
}

void ImageDisplay::LoadNewImage(const Glib::ustring& filename) {
  image_ = Pixbuf::create_from_file(filename);
  Glib::RefPtr<Gdk::Window> window = get_window();
  window->clear();
  Redraw();
  extracted_features_ = false;
  ExtractFeatures();
}


bool ImageDisplay::OnButtonPress(GdkEventButton* press_event) {
  is_dragging_ = true;
  drag_start_x_ = (int)press_event->x;
  drag_start_y_ = (int)press_event->y;
  drag_end_x_ = (int)press_event->x;
  drag_end_y_ = (int)press_event->y;
  // Fix the cases where we drag out of the image box:
  if (drag_start_x_ < 0) {
    drag_start_x_ = 0;
    drag_end_x_ = 0;
  }
  if (drag_start_y_ < 0) {
    drag_start_y_ = 0;
    drag_end_y_ = 0;
  }
  
  if (drag_start_x_ >= image_->get_width()) {
    drag_start_x_ = image_->get_width() - 1;
    drag_end_x_ = image_->get_width() - 1;
  }
  if (drag_start_y_ >= image_->get_height()) {
    drag_start_y_ = image_->get_height() - 1;
    drag_end_y_ = image_->get_height() - 1;
  }

  Redraw();

  return true;
}

bool ImageDisplay::OnMouseMove(GdkEventMotion* event) {
  if (is_dragging_) {
    drag_end_x_ = (int)event->x;
    drag_end_y_ = (int)event->y;

    // Fix the cases where we drag out of the image box:
    if (drag_end_x_ < 0) {
      drag_end_x_ = 0;
    }
    if (drag_end_y_ < 0) {
      drag_end_y_ = 0;
    }
    
    if (drag_end_x_ >= image_->get_width()) {
      drag_end_x_ = image_->get_width() - 1;
    }
    if (drag_end_y_ >= image_->get_height()) {
      drag_end_y_ = image_->get_height() - 1;
    }

    Redraw();
    return true;
  }

  return false;
}

bool ImageDisplay::OnButtonRelease(GdkEventButton* release_event) {
  is_dragging_ = false;
  int width, height;

  // Calculate the width and height of the region, and reassign
  // drag_start_x_ and drag_start_y_ to be the top-left corner of
  // the drawn box.
  if (drag_start_x_ > drag_end_x_) {
    width = drag_start_x_ - drag_end_x_;
    drag_start_x_ = drag_end_x_;
  } else {
    width = drag_end_x_ - drag_start_x_;
  }

  if (drag_start_y_ > drag_end_y_) {
    height = drag_start_y_ - drag_end_y_;
    drag_start_y_ = drag_end_y_;
  } else {
    height = drag_end_y_ - drag_start_y_;
  }

  if (width > 0 && height > 0) {
    Glib::RefPtr<Pixbuf> cropped_image(
      Pixbuf::create_subpixbuf(image_,
                               drag_start_x_, drag_start_y_,
                               width, height));
    frame_->mutable_preview_box()->set(cropped_image);
  }
  return true;
}

bool ImageDisplay::on_expose_event(GdkEventExpose* event) {
  Redraw();
  return true;
}

void ImageDisplay::Redraw() {
  Glib::RefPtr<Gdk::Window> window = get_window();
  Glib::RefPtr<Gdk::GC> context = get_style()->get_black_gc();

  if (window) {
    image_->render_to_drawable(window, context,
                               0, 0, 0, 0,
                               image_->get_width(),
                               image_->get_height(),
                               Gdk::RGB_DITHER_NONE, 0, 0);
    if (is_dragging_) {
      window->draw_line(context, drag_start_x_, drag_start_y_,
                        drag_start_x_, drag_end_y_);
      window->draw_line(context, drag_start_x_, drag_start_y_,
                        drag_end_x_, drag_start_y_);
      window->draw_line(context, drag_end_x_, drag_end_y_,
                      drag_start_x_, drag_end_y_);
      window->draw_line(context, drag_end_x_, drag_end_y_,
                        drag_end_x_, drag_start_y_);
      
    }

    if (extracted_features_) {
      context->set_rgb_fg_color(Color("Yellow"));
      for (int ii = 0; ii < (int)interest_points_.size(); ++ii) {
        window->draw_point(context,
                           (int)interest_points_[ii].x,
                           (int)interest_points_[ii].y);
      }
      context->set_rgb_fg_color(Color("Black"));
    }
  }
}

void ImageDisplay::ExtractFeatures() {
  image_->save("/tmp/MULTIOBJ_TMP.png", "png");

  HarrisDetector detector;
  detector.SetBinaryPath("~/bin/h_affine.ln");
  detector.SetDetector(HESSIAN_AFFINE);
  detector.DetectInterestPoints("/tmp/MULTIOBJ_TMP.png", &interest_points_);
  extracted_features_ = true;
  Redraw();
}
