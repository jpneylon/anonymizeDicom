#ifndef __DICOMRT_SET_CLASS_H__
#define __DICOMRT_SET_CLASS_H__

#include <gtkmm.h>

#include "../RTClasses/rtimages.h"
#include "../RTClasses/rtdose.h"
#include "../RTClasses/rtstruct.h"
#include "../RTClasses/rtplan.h"


#define STARTDIR "/home/"

class DICOMRT_Set : public Gtk::Box
{
    public:
        DICOMRT_Set();
        virtual ~DICOMRT_Set();

        void open_dicom_directory();
        void close_dicom_directory();
        void print_dicom_directory();
        char *get_dicom_directory();

        bool get_rtimage_open(){ return rtimage_open; };

    private:
        void select_input_directory();
        void select_output_directory();

        char *inputdir;
        char *outputdir;

        RTDose *rtdose;
        RTStruct *rtstruct;
        RTPlan *rtplan;
        RTImage *rtimage;

        bool rtimage_open;
        bool rtdose_open;
        bool rtstruct_open;
        bool rtplan_open;

        uint text_length;
        char *dicomText;
        Gtk::Box        *dicomBox;
        Gtk::Label      *dicomLabel;
        Gtk::ScrolledWindow *dicomWindow;
};



#endif // __DICOMRT_SET_CLASS_H__
