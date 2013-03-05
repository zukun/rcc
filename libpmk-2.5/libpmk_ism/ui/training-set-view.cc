// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of TrainingSetView.

#include <gtkmm.h>
#include <iostream>
#include <vector>
#include <string>

#include "ui/featured-image.h"
#include "ui/training-set-view.h"

using namespace std;

TrainingSetView::TrainingSetView() {
  add(image_grid_);
  tree_model_ = Gtk::ListStore::create(tree_columns_);
  image_grid_.set_model(tree_model_);

  Gtk::CellRendererPixbuf* cell = new Gtk::CellRendererPixbuf;
  image_grid_.append_column("Image", *cell);
  Gtk::TreeViewColumn* column = image_grid_.get_column(0);
  if(column) {
    column->add_attribute(cell->property_pixbuf(), tree_columns_.image_);
  }

  image_grid_.append_column("Label", tree_columns_.label_);
  delete cell;
}

TrainingSetView::~TrainingSetView() {
  for (int ii = 0; ii < (int)images_.size(); ++ii) {
    delete images_[ii];
  }
}

void TrainingSetView::AddExample(const string& label,
                                 const Glib::RefPtr<Gdk::Pixbuf>& image) {
  FeaturedImage* new_image = new FeaturedImage(image);
  images_.push_back(new_image);
  labels_.push_back(label);
  RefreshGrid();
}

void TrainingSetView::RefreshGrid() {
  tree_model_->clear();
  

  for (int ii = 0; ii < (int)images_.size(); ++ii) {
    Gtk::TreeModel::Row row = *(tree_model_->append());
    row[tree_columns_.image_] = images_[ii]->mutable_pixbuf();
    row[tree_columns_.label_] = labels_[ii];
  }
}

void TrainingSetView::DeleteSelectedExample() {
  Glib::RefPtr<Gtk::TreeSelection> selection = image_grid_.get_selection();
  Gtk::TreeModel::iterator iter = selection->get_selected();
  if (iter) {
    // Find the index of the selected item by iterating through all rows.
    int index = 0;

    typedef Gtk::TreeModel::Children type_children;
    type_children children = tree_model_->children();
    for(type_children::iterator iter2 = children.begin();
        iter2 != children.end(); ++iter2) {
      if (iter == iter2) {
        break;
      }
      ++index;
    }

    delete images_[index];
    images_.erase(images_.begin() + index);
    labels_.erase(labels_.begin() + index);
    RefreshGrid();
  }
}
