
#include "dicomrt_set.h"

DICOMRT_Set::DICOMRT_Set()
{
    set_orientation( Gtk::ORIENTATION_HORIZONTAL );
    set_size_request( 128, 128 );

    rtimage_open = false;
    rtdose_open = false;
    rtstruct_open = false;
    rtplan_open = false;

    text_length = 0;
    dicomLabel = new Gtk::Label();

    dicomBox = new Gtk::Box( Gtk::ORIENTATION_VERTICAL );
    dicomBox->pack_start( dicomLabel[0], true, true, 0 );

    dicomWindow = new Gtk::ScrolledWindow();
    dicomWindow->set_policy( Gtk::POLICY_AUTOMATIC, Gtk::POLICY_ALWAYS );
    dicomWindow->add( dicomBox[0] );

    pack_start( dicomWindow[0], true, true, 0 );
}

DICOMRT_Set::~DICOMRT_Set()
{
    close_dicom_directory();
    hide();
}

void
DICOMRT_Set::close_dicom_directory()
{
    if (rtimage_open)
    {
        if (rtplan_open)
        {
            delete rtplan;
            rtplan_open = false;
        }
        if (rtstruct_open)
        {
            delete rtstruct;
            rtstruct_open = false;
        }
        if (rtdose_open)
        {
            delete rtdose;
            rtdose_open = false;
        }

        delete rtimage;
        rtimage_open = false;
    }
    hide();
}
void
DICOMRT_Set::select_input_directory()
{
    GtkWidget *file;

    file = gtk_file_chooser_dialog_new( "Choose Input Directory", NULL,
                                        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                        "CANCEL", GTK_RESPONSE_CANCEL,
                                        "OPEN", GTK_RESPONSE_ACCEPT,
                                        NULL);
    gtk_file_chooser_set_filename( GTK_FILE_CHOOSER (file), STARTDIR );

    if (gtk_dialog_run ( GTK_DIALOG (file)) == GTK_RESPONSE_ACCEPT)
    {
        inputdir = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file));
    }
    gtk_widget_destroy (file);
}
void
DICOMRT_Set::select_output_directory()
{
    GtkWidget *file;

    file = gtk_file_chooser_dialog_new( "Choose Output Directory", NULL,
                                        GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                        "CANCEL", GTK_RESPONSE_CANCEL,
                                        "OPEN", GTK_RESPONSE_ACCEPT,
                                        NULL);
    gtk_file_chooser_set_filename( GTK_FILE_CHOOSER (file), inputdir );

    if (gtk_dialog_run ( GTK_DIALOG (file)) == GTK_RESPONSE_ACCEPT)
    {
        outputdir = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (file));
    }
    gtk_widget_destroy (file);
}
void
DICOMRT_Set::open_dicom_directory()
{
    ///////////////////////////////////////// Select Planning CT /////////////////////////////////
    select_input_directory();
    select_output_directory();

        rtimage = new RTImage();
        rtimage->setDicomDirectory( inputdir );
        printf("\n Plan_CT Dicom Dir: %s",  rtimage->getDicomDirectory() );

        if ( rtimage->loadDicomInfo() )
        {
            rtimage->loadRTImageData();
            rtimage_open = true;
            rtimage->saveRTImageData( outputdir, true );

            text_length = strlen(inputdir) + 16;
            for (uint slice = 0; slice < rtimage->getImageCount(); slice++)
                text_length += strlen( rtimage->getSliceFilename(slice) );

            text_length += 12 * rtimage->getImageCount();
            dicomText = new char[ text_length ];
            sprintf( dicomText, "%s\n", inputdir );
            for (uint slice = 0; slice < rtimage->getImageCount(); slice++)
            {
                strcat( dicomText, "   image: ");
                strcat( dicomText, rtimage->getSliceFilename(slice) );
                strcat( dicomText, "\n");
            }
            dicomLabel->set_text( dicomText );

            rtstruct = new RTStruct;
            rtstruct->setDicomDirectory(  inputdir );
            if ( rtstruct->loadDicomInfo() )
            {
                rtstruct->loadRTStructInfo();
                rtstruct_open = true;
            }
            else printf("\n Moving on from RTSTRUCT...\n");

            rtplan = new RTPlan();
            rtplan->setDicomDirectory(  inputdir );
            if (  rtplan->loadDicomInfo() )
            {
                rtplan->loadRTPlanData();
                rtplan_open = true;
            }
            else printf("\n Moving on from RTPLAN...\n");

            rtdose = new RTDose();
            rtdose->setDicomDirectory(  inputdir );
            if ( rtdose->loadDicomInfo() )
            {
                rtdose->loadRTDoseData();
                rtdose_open = true;
                rtdose->saveRTDoseData( outputdir, rtdose->getDataArray(), true );

                char *temp_text = new char[text_length];
                sprintf( temp_text, "%s", dicomText);

                delete [] dicomText;
                text_length += strlen( rtdose->getDicomFilename() ) + 12;
                dicomText = new char[ text_length ];
                sprintf( dicomText, "%s", temp_text );
                delete [] temp_text;
                strcat( dicomText, "   dose: ");
                strcat( dicomText, rtdose->getDicomFilename() );
                strcat( dicomText, "\n" );
            }
            else printf("\n Moving on from RTDOSE...\n");

            show_all_children();
        }
        else
        {
            // pop up error window
            printf("\n Error loading DICOM information.\n");
        }
}
void
DICOMRT_Set::print_dicom_directory()
{
    if (rtimage_open)
    {
        printf("\n Current Directory: %s\n", rtimage->getDicomDirectory() );
    }
}
char*
DICOMRT_Set::get_dicom_directory()
{
    if (rtimage_open)
    {
         inputdir = NULL;
         inputdir = rtimage->getDicomDirectory();
         return inputdir;
    }
    else
        return "No DICOM Directory Open.";
}
