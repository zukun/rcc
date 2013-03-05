// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of SelectingImageBox.
//

#include "ui/selecting-image-box.h"
#include "ui/image-box.h"

SelectingImageBox::SelectingImageBox() : ImageBox(),
                                         is_dragging_(false),
                                         drag_start_x_(0), drag_start_y_(0),
                                         drag_end_x_(0), drag_end_y_(0) {
  set_events(Gdk::BUTTON_PRESS_MASK |
             Gdk::BUTTON_MOTION_MASK |
             Gdk::BUTTON_RELEASE_MASK);
  signal_button_press_event().connect(
    sigc::mem_fun(*this, &SelectingImageBox::OnButtonPress));
  signal_button_release_event().connect(
    sigc::mem_fun(*this, &SelectingImageBox::OnButtonRelease));
  signal_motion_notify_event().connect(
    sigc::mem_fun(*this, &SelectingImageBox::OnMouseMove));
}



bool SelectingImageBox::OnButtonPress(GdkEventButton* press_event) {
  if (has_image()) {
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
    
    if (drag_start_x_ >= image_.pixbuf()->get_width()) {
      drag_start_x_ = image_.pixbuf()->get_width() - 1;
      drag_end_x_ = image_.pixbuf()->get_width() - 1;
    }
    if (drag_start_y_ >= image_.pixbuf()->get_height()) {
      drag_start_y_ = image_.pixbuf()->get_height() - 1;
      drag_end_y_ = image_.pixbuf()->get_height() - 1;
    }
    
    Redraw();
    return true;
  } else {
    return false;
  }
}

bool SelectingImageBox::OnMouseMove(GdkEventMotion* event) {
  if (has_image()) {
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
      
      if (drag_end_x_ >= image_.pixbuf()->get_width()) {
        drag_end_x_ = image_.pixbuf()->get_width() - 1;
      }
      if (drag_end_y_ >= image_.pixbuf()->get_height()) {
        drag_end_y_ = image_.pixbuf()->get_height() - 1;
      }
      
      Redraw();
      return true;
    }
  }

  return false;
}

bool SelectingImageBox::OnButtonRelease(GdkEventButton*) {
  if (has_image()) {
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
        Pixbuf::create_subpixbuf(image_.pixbuf(),
                                 drag_start_x_, drag_start_y_,
                                 width, height));
      signal_region_selected_.emit(cropped_image);
    }
    Redraw();
    return true;
  }
  return false;
}

void SelectingImageBox::Redraw() {
  if (has_image()) {
    ImageBox::Redraw();
    Glib::RefPtr<Gdk::Window> window = get_window();
    Glib::RefPtr<Gdk::GC> context = get_style()->get_black_gc();
    
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
  }
}

void SelectingImageBox::DrawLine(int from_x, int from_y,
                                 int to_x, int to_y) {
    Glib::RefPtr<Gdk::Window> window = get_window();
    Glib::RefPtr<Gdk::GC> context = get_style()->get_black_gc();
    
    window->draw_line(context, from_x, from_y,
                      to_x, to_y);
}

SelectingImageBox::RegionSelectSignal
SelectingImageBox::signal_region_selected() {
  return signal_region_selected_;
}
