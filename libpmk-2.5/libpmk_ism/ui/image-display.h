// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#ifndef MULTIOBJ_UI_IMAGE_DISPLAY_H
#define MULTIOBJ_UI_IMAGE_DISPLAY_H

#include <gtkmm.h>
#include <vector>
#include "interest_point.h"

class Frame;

using namespace std;
using namespace libpmk_features;
using namespace Gdk;

class ImageDisplay : public Gtk::DrawingArea {
public:
  ImageDisplay(Frame* frame);
  virtual ~ImageDisplay() { }

  void LoadNewImage(const Glib::ustring& filename);
  void ExtractFeatures();

protected:
  Glib::RefPtr<Pixbuf> image_;
  Frame* frame_;
  Gtk::Image* preview_;
  bool OnButtonPress(GdkEventButton* press_event);
  bool OnButtonRelease(GdkEventButton* release_event);
  bool OnMouseMove(GdkEventMotion* motion_event);
  bool on_expose_event(GdkEventExpose* event);
  bool is_dragging_;
  int drag_start_x_;
  int drag_start_y_;
  int drag_end_x_;
  int drag_end_y_;
  bool extracted_features_;
  vector<InterestPoint> interest_points_;

private:
  void Redraw();
};

#endif  // MULTIOBJ_UI_IMAGE_DISPLAY_H
