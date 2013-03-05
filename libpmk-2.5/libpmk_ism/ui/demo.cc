#include <gtkmm/main.h>
#include "ui/frame.h"

int main(int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);

  Frame frame;
  //Shows the window and returns when it is closed.
  Gtk::Main::run(frame);

  return 0;
}
