// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// A GTK widget for displaying an image that may contain features.
//

#ifndef MULTIOBJ_UI_IMAGE_BOX_H
#define MULTIOBJ_UI_IMAGE_BOX_H

#include <gtkmm.h>
#include <vector>
#include "ui/featured-image.h"
#include "ui/image-overlay.h"

using namespace std;
using namespace libpmk_features;
using namespace Gdk;

class ImageBox : public Gtk::DrawingArea {
public:
  ImageBox();
  virtual ~ImageBox();

  const FeaturedImage& image() { return image_; }
  FeaturedImage* mutable_image() { return &image_; }

  void LoadNewImage(const Glib::ustring& filename);
  void LoadNewImage(const Glib::RefPtr<Gdk::Pixbuf>& image);

  bool has_image() const { return image_.has_image(); }
  // These return 0 if there is no image loaded, otherwise
  // they return the size in pixels.
  int image_width() const;
  int image_height() const;

  ImageOverlay* mutable_overlay(int ii) { return overlays_[ii]; }
  ImageOverlay* add_overlay();
  int overlays_size() const { return (int)overlays_.size(); }
  void ClearAllOverlays();

  virtual void Redraw();

protected:
  FeaturedImage image_;

  bool on_expose_event(GdkEventExpose* event);
  void RedrawImage();
  void ClearImage();

private:
  // These pointers are owned by this image box.
  vector<ImageOverlay*> overlays_;
};

#endif  // MULTIOBJ_UI_IMAGE_BOX_H
