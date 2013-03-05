// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This displays a grid of image/label pairs. The number of columns is
// fixed and the number of rows grows as needed.

#ifndef MULTIOBJ_UI_TRAINING_SET_VIEW_H
#define MULTIOBJ_UI_TRAINING_SET_VIEW_H

#include <gtkmm.h>
#include <vector>
#include <string>

using namespace std;

class TrainingSetView : public Gtk::ScrolledWindow {
public:
  TrainingSetView();
  virtual ~TrainingSetView();

  static const int NUM_COLS = 4;

  void AddExample(const string& label, const Glib::RefPtr<Gdk::Pixbuf>& image);

  // Removes the currently-selected example; if none are selected, this
  // does nothing.
  void DeleteSelectedExample();

  int size() const { return (int)images_.size(); }

  const FeaturedImage& image(int index) const { return *(images_[index]); }
  FeaturedImage* mutable_image(int index) { return images_.at(index); }

protected:
  // This is used for the table view.
  class TrainingExampleColumns : public Gtk::TreeModel::ColumnRecord {
  public:
    TrainingExampleColumns() {
      add(image_);
      add(label_);
    }
    
    Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > image_;
    Gtk::TreeModelColumn<Glib::ustring> label_;
  };

  Gtk::TreeView image_grid_;
  Glib::RefPtr<Gtk::ListStore> tree_model_;
  TrainingExampleColumns tree_columns_;

private:
  // These are parallel vectors containing the training data.
  // The ImageBox pointers are owned by this object.
  vector<FeaturedImage*> images_;
  vector<string> labels_;

  void RefreshGrid();
};

#endif  // MULTIOBJ_UI_TRAINING_SET_VIEW_H
