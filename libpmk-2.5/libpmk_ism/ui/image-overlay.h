// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This class stores a list of lines and points, along with their
// colors. It is responsible for drawing itself onto a window when
// called to do so.
//

#ifndef MULTIOBJ_UI_IMAGE_OVERLAY_H
#define MULTIOBJ_UI_IMAGE_OVERLAY_H

#include <gtkmm.h>
#include <vector>

using namespace std;

class ImageOverlay {
public:
  ImageOverlay();
  ~ImageOverlay() { }

  void clear();
  void AddLine(const Gdk::Color& color, int start_x, int start_y,
               int end_x, int end_y);
  void AddPoint(const Gdk::Color& color, int x, int y);

  // This method will reset the context to have a black color.
  void DrawToWindow(Glib::RefPtr<Gdk::Window> window,
                    Glib::RefPtr<Gdk::GC> context) const;

private:
  // This is a list of (start_x, start_y) (end_x, end_y).
  vector<pair<int, int> > line_starts_;
  vector<pair<int, int> > line_ends_;
  vector<Gdk::Color> line_colors_;

  vector<pair<int, int> > points_;
  vector<Gdk::Color> point_colors_;
};

#endif  // MULTIOBJ_UI_IMAGE_OVERLAY_H
