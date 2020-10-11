#include "AboutDialog.h"
#include "version.h"
#include "icon_paths.h"
#include <vector>
#include <iostream>
#include <gdkmm-3.0/gdkmm/pixbuf.h>


#define COMP_DATE_TIME " on " __DATE__ " at " __TIME__

AboutDialog::AboutDialog()
{
    set_program_name("List Maker");
    set_version(PROGRAM_VERSION);
    set_authors({"timre13"});
    set_license_type(Gtk::License::LICENSE_BSD);

    set_comments("Compiled" COMP_DATE_TIME);

    try
    {
        set_logo(Gdk::Pixbuf::create_from_file(ICON_PATH_APP_ICON, 128, 128));
    }
    catch (...)
    {
        std::cerr << "Failed to load icon" << std::endl;
    }
}
