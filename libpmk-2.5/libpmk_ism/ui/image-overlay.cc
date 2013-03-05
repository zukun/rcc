// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//

#include <gtkmm.h>
#include <vector>
#include <iostream>
#include "ui/image-overlay.h"

using namespace std;

ImageOverlay::ImageOverlay() { }

void ImageOverlay::clear() {
  line_starts_.clear();
  line_ends_.clear();
  line_colors_.clear();
  points_.clear();
  point_colors_.clear();
}

void ImageOverlay::AddLine(const Gdk::Color& color, int start_x, int start_y,
                           int end_x, int end_y) {
  pair<int, int> start(start_x, start_y);
  pair<int, int> end(end_x, end_y);
  line_starts_.push_back(start);
  line_ends_.push_back(end);
  line_colors_.push_back(color);
}

void ImageOverlay::AddPoint(const Gdk::Color& color, int x, int y) {
  points_.push_back(pair<int, int>(x, y));
  point_colors_.push_back(color);
}


void ImageOverlay::DrawToWindow(Glib::RefPtr<Gdk::Window> window,
                                Glib::RefPtr<Gdk::GC> context) const {
  for (int ii = 0; ii < (int)line_colors_.size(); ++ii) {
    context->set_rgb_fg_color(line_colors_[ii]);
    window->draw_line(context, line_starts_[ii].first,
                      line_starts_[ii].second,
                      line_ends_[ii].first,
                      line_ends_[ii].second);
  }

  for (int ii = 0; ii < (int)points_.size(); ++ii) {
    context->set_rgb_fg_color(point_colors_[ii]);
    window->draw_point(context, points_[ii].first, points_[ii].second);
    window->draw_point(context, points_[ii].first + 1, points_[ii].second);
    window->draw_point(context, points_[ii].first - 1, points_[ii].second);
    window->draw_point(context, points_[ii].first, points_[ii].second + 1);
    window->draw_point(context, points_[ii].first, points_[ii].second - 1);
    window->draw_point(context, points_[ii].first + 1, points_[ii].second + 1);
    window->draw_point(context, points_[ii].first + 1, points_[ii].second - 1);
    window->draw_point(context, points_[ii].first - 1, points_[ii].second + 1);
    window->draw_point(context, points_[ii].first - 1, points_[ii].second - 1);
  }

  context->set_rgb_fg_color(Gdk::Color("Black"));
}
