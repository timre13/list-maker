#include "AboutDialog.h"
#include "version.h"
#include "icon_paths.h"
#include <vector>
#include <iostream>
#include <gdkmm-3.0/gdkmm/pixbuf.h>


AboutDialog::AboutDialog()
{
    set_program_name("List Maker");
    set_version(PROGRAM_VERSION);
    set_authors({"timre13"});
    set_license_type(Gtk::License::LICENSE_BSD);

#ifdef GIT_COMMIT
    set_comments("Compiled on " __DATE__ " at " __TIME__ " from commit " GIT_COMMIT);
#else
    set_comments("Compiled on " __DATE__ " at " __TIME__);
#endif

    try
    {
        set_logo(Gdk::Pixbuf::create_from_file(ICON_PATH_APP_ICON, 128, 128));
    }
    catch (...)
    {
        std::cerr << "Failed to load icon" << std::endl;
    }
}
