// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// Implementation of LabelInputDialog.
//

#include <iostream>
#include "ui/label-input-dialog.h"

using namespace std;

LabelInputDialog::LabelInputDialog(Gtk::Window& parent) : 
  Dialog("Enter a label", parent, true),
  parent_(parent) {
  Gtk::Button* ok_button = add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  Gtk::Button* cancel_button =
    add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

  label_.set_text("Enter a label");
  get_vbox()->pack_start(label_);
  get_vbox()->pack_start(label_field_);

  ok_button->signal_clicked().connect(
    sigc::mem_fun(*this, &LabelInputDialog::OnClickOK));
  cancel_button->signal_clicked().connect(
    sigc::mem_fun(*this, &LabelInputDialog::OnClickCancel));
}

Glib::ustring LabelInputDialog::label() const {
  return label_field_.get_text();
}

int LabelInputDialog::run() {
  show();
  show_all_children();
  Gtk::Main::run();
  return result_;
}

void LabelInputDialog::OnClickOK() {
  if (label_field_.get_text().size() == 0) {
    Gtk::MessageDialog dialog(parent_, "Error");
    dialog.set_secondary_text("Label cannot be empty.");
    dialog.run();
  } else {
    result_ = Gtk::RESPONSE_OK;
    hide();
    Gtk::Main::quit();
  }
}

void LabelInputDialog::OnClickCancel() {
  result_ = Gtk::RESPONSE_CANCEL;
  hide();
  Gtk::Main::quit();
}

