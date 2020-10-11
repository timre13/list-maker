#include "AskRGB.h"
#include <memory>
#include <gtkmm-3.0/gtkmm/colorchooserdialog.h>


Gdk::RGBA askRGB(const Gdk::RGBA defaultColor/*=Gdk::RGBA("black")*/)
{
    auto dialog{std::make_unique<Gtk::ColorChooserDialog>()};
    dialog->set_use_alpha(false);
    dialog->set_rgba(defaultColor);

    // If the OK button clicked, return the selected color
    if (dialog->run() == Gtk::ResponseType::RESPONSE_OK)
        return dialog->get_rgba();

    // If the window closed or the cancel button
    // clicked, return the original color.
    return defaultColor;
}
