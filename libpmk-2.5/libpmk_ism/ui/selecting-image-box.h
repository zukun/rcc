// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This is a subclass of ImageBox that allows the user to select regions
// by drawing rectangles. It responds to UI clicks.

#ifndef MULTIOBJ_UI_SELECTING_IMAGE_BOX_H
#define MULTIOBJ_UI_SELECTING_IMAGE_BOX_H

#include <gtkmm.h>
#include <vector>
#include "ui/image-box.h"
#include "ui/featured-image.h"

using namespace std;
using namespace libpmk_features;
using namespace Gdk;

class SelectingImageBox : public ImageBox {
public:
  SelectingImageBox();
  virtual ~SelectingImageBox() { }

  typedef sigc::signal<void, Glib::RefPtr<Pixbuf> > RegionSelectSignal;
  RegionSelectSignal signal_region_selected();
  virtual void Redraw();

  void DrawLine(int from_x, int from_y,
                 int to_x, int to_y);
protected:
  bool OnButtonPress(GdkEventButton* press_event);
  bool OnButtonRelease(GdkEventButton* release_event);
  bool OnMouseMove(GdkEventMotion* motion_event);

private:
  bool is_dragging_;
  int drag_start_x_;
  int drag_start_y_;
  int drag_end_x_;
  int drag_end_y_;
  RegionSelectSignal signal_region_selected_;

};

#endif  // MULTIOBJ_UI_SELECTING_IMAGE_BOX_H
