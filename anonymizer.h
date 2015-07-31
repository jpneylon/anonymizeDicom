#ifndef __TOOLBOX_CLASS_H__
#define __TOOLBOX_CLASS_H__

#include <gtkmm.h>
#include "dicomrt_set.h"

class DICOM_ANON : public Gtk::Window
{
  public:
    DICOM_ANON();
    virtual ~DICOM_ANON();

  protected:
    // Signal Handlers
    void file_close_directory();
    void file_open_directory();
    void file_print_directory();
    void delete_event();

    // Child Widgets
    Gtk::Box        viewBox;
    Gtk::Label      cwdLabel;
    Gtk::Box        mainBox;
    Gtk::Label      label;
    Gtk::Box        cwdBox;

    Gtk::Widget *menuBar;
    Gtk::Widget *toolBar;

    Glib::RefPtr<Gtk::UIManager> manager;
    Glib::RefPtr<Gtk::ActionGroup> actionGroup;

    // Variables
    DICOMRT_Set *dicomrt;
    bool dicomrt_loaded;
};


#endif // __TOOLBOX_CLASS_H__
