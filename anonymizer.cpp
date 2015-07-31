
#include "anonymizer.h"
#include <gtkmm/application.h>


int main (int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create( argc, argv, "jacko.anonymizer" );
    DICOM_ANON anonymizer;
    return app->run(anonymizer);
}


DICOM_ANON::DICOM_ANON() :
    mainBox( Gtk::ORIENTATION_VERTICAL )
{
    dicomrt_loaded = false;

    /* Create the main window */
    set_title("DICOM Toolbox");
    set_position( Gtk::WIN_POS_CENTER );
    set_size_request(180,180);
    set_resizable(true);
    add( mainBox );

    mainBox.set_border_width(1);

    viewBox.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    viewBox.set_border_width(1);
    viewBox.set_size_request(-1, -1);

    actionGroup = Gtk::ActionGroup::create();

    // File Sub Menu Items
    actionGroup->add( Gtk::Action::create("MenuFile", "_File") );
    actionGroup->add( Gtk::Action::create("Open", Gtk::Stock::OPEN),
        sigc::mem_fun( *this, &DICOM_ANON::file_open_directory) );
    actionGroup->add( Gtk::Action::create("Save", Gtk::Stock::SAVE),
        sigc::mem_fun( *this, &DICOM_ANON::file_print_directory) );
    actionGroup->add( Gtk::Action::create("Close", Gtk::Stock::CLOSE),
        sigc::mem_fun( *this, &DICOM_ANON::file_close_directory) );
    actionGroup->add( Gtk::Action::create("Quit", Gtk::Stock::QUIT),
        sigc::mem_fun( *this, &DICOM_ANON::delete_event) );

    // View Sub Menu Items
    //actionGroup->add( Gtk::Action::create("MenuView", "_View") );
    //actionGroup->add( Gtk::ToggleAction::create( "toggle_contour", "Toggle _Contours"),
    //    sigc::mem_fun( *this, &DICOM_ANON::toggle_contour_display_box) );

    manager = Gtk::UIManager::create();
    manager->insert_action_group( actionGroup );
    add_accel_group( manager->get_accel_group() );

    Glib::ustring ui_info =
        "<ui>"
        "   <menubar name='MenuBar'>"
        "       <menu action='MenuFile'>"
        "           <menuitem action='Open'/>"
        "           <menuitem action='Save'/>"
        "           <menuitem action='Close'/>"
        "           <separator/>"
        "           <menuitem action='Quit'/>"
        "       </menu>"
        //"       <menu action='MenuView'>"
        //"           <menuitem action='toggle_contour'/>"
        //"       </menu>"
        "   </menubar>"
        "   <toolbar name='ToolBar'>"
        "       <toolitem action='Open'/>"
        "       <toolitem action='Close'/>"
        "       <toolitem action='Quit'/>"
        "   </toolbar>"
        "</ui>";
    manager->add_ui_from_string(ui_info);

    menuBar = manager->get_widget("/MenuBar");
    mainBox.pack_start( *menuBar, Gtk::PACK_SHRINK );

    toolBar = manager->get_widget("/ToolBar");
    mainBox.pack_start( *toolBar, Gtk::PACK_SHRINK );

    //label.set_text("CWD:");
    //cwdLabel.set_text("...");

    //cwdBox.set_orientation( Gtk::ORIENTATION_HORIZONTAL );
    //cwdBox.set_border_width(0);
    //cwdBox.pack_start( label, false, false, 2 );
    //cwdBox.pack_start( cwdLabel, false, false, 2);

    mainBox.pack_start( viewBox, true, true, 2);
    //mainBox.pack_start( cwdBox,  false, false, 2);

    show_all_children();
}

DICOM_ANON::~DICOM_ANON()
{
    delete_event();
}


/////////////// Callback Functions to Create and Control Axial Display Windows
void
DICOM_ANON::file_close_directory()
{
    if (dicomrt_loaded)
        delete dicomrt;

    dicomrt_loaded = false;

    //cwdLabel.set_text( "..." );
    viewBox.set_size_request(32, 32);
    set_size_request(180, 180);
    show_all_children();
}
void
DICOM_ANON::file_open_directory()
{
    if (!dicomrt_loaded)
    {
        dicomrt = new DICOMRT_Set;
        dicomrt->open_dicom_directory();

        if (dicomrt->get_rtimage_open() )
        {
            dicomrt_loaded = true;
            viewBox.pack_start( *dicomrt );
            //cwdLabel.set_text( dicomrt->get_dicom_directory() );
            show_all_children();
        }
    }
    show_all_children();
}
void
DICOM_ANON::file_print_directory()
{
    if (dicomrt_loaded)
        dicomrt->print_dicom_directory();
}
void
DICOM_ANON::delete_event()
{
    if (dicomrt_loaded)
        file_close_directory();
    hide();
}



