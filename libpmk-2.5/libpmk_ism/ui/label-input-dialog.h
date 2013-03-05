// Copyright 2008, Massachusetts Institute of Technology.
// The use of this code is permitted for research only. There is
// absolutely no warranty for this software.
//
// Author: John Lee (jjl@mit.edu)
//
// This dialog asks a user for a short text label and stores it.
//

#ifndef MULTIOBJ_UI_LABEL_INPUT_DIALOG_H
#define MULTIOBJ_UI_LABEL_INPUT_DIALOG_H

#include <gtkmm.h>

class LabelInputDialog : public Gtk::Dialog {
public:
  LabelInputDialog(Gtk::Window& parent);
  virtual ~LabelInputDialog() { }
  virtual int run();
  Glib::ustring label() const;

protected:
  Gtk::Window& parent_;
  int result_;
  Gtk::Entry label_field_;
  Gtk::Label label_;
  void OnClickOK();
  void OnClickCancel();
};

#endif  // MULTIOBJ_UI_LABEL_INPUT_DIALOG_H
