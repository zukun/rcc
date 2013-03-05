// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of ImageBox.
//

#include <iostream>
#include "ui/image-box.h"


using namespace Gdk;
using namespace std;
using namespace libpmk_features;

ImageBox::ImageBox() { }

ImageBox::~ImageBox() {
  ClearAllOverlays();
}

void ImageBox::ClearAllOverlays() {
  for (int ii = 0; ii < (int)overlays_.size(); ++ii) {
    delete overlays_[ii];
  }
  overlays_.clear();
}

void ImageBox::LoadNewImage(const Glib::ustring& filename) {
  if (has_image()) {
    ClearImage();
  }

  ClearAllOverlays();
  image_.set_image(filename);
  Redraw();
}

void ImageBox::LoadNewImage(const Glib::RefPtr<Gdk::Pixbuf>& image) {
  if (has_image()) {
    ClearImage();
  }

  ClearAllOverlays();
  image_.set_image(image);
  Redraw();
}

bool ImageBox::on_expose_event(GdkEventExpose*) {
  Redraw();
  return true;
}

void ImageBox::Redraw() {
  RedrawImage();
  
  Glib::RefPtr<Gdk::Window> window = get_window();
  Glib::RefPtr<Gdk::GC> context = get_style()->get_black_gc();
  for (int ii = 0; ii < (int)overlays_.size(); ++ii) {
    overlays_[ii]->DrawToWindow(window, context);
  }
}

void ImageBox::RedrawImage() {
  if (has_image()) {
    Glib::RefPtr<Gdk::Window> window = get_window();
    Glib::RefPtr<Gdk::GC> context = get_style()->get_black_gc();
    if (window) {
      image_.pixbuf()->render_to_drawable(window, context,
                                          0, 0, 0, 0,
                                          image_.pixbuf()->get_width(),
                                          image_.pixbuf()->get_height(),
                                          Gdk::RGB_DITHER_NONE, 0, 0);
      // Draw the feature points, if any:
      if (image_.has_interest_points()) {
        context->set_rgb_fg_color(Color("Yellow"));
        const vector<InterestPoint>& interest_points(image_.interest_points());
        for (int ii = 0; ii < (int)interest_points.size(); ++ii) {
          window->draw_point(context,
                             (int)interest_points[ii].x,
                             (int)interest_points[ii].y);
        }
        context->set_rgb_fg_color(Color("Black"));
      }
    }
  }
}

void ImageBox::ClearImage() {
  if (has_image()) {
    Glib::RefPtr<Gdk::Window> window = get_window();
    window->clear();
  }
}

int ImageBox::image_width() const {
  if (has_image()) {
    return image_.pixbuf()->get_width();
  }
  return 0;
}

int ImageBox::image_height() const {
  if (has_image()) {
    return image_.pixbuf()->get_height();
  }
  return 0;
}

ImageOverlay* ImageBox::add_overlay() {
  ImageOverlay* returned_overlay = new ImageOverlay();
  overlays_.push_back(returned_overlay);
  return returned_overlay;
}
