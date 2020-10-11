#pragma once

#include <memory>
#include <glibmm-2.4/glibmm/ustring.h>
#include <gtkmm-3.0/gtkmm/dialog.h>
#include <gtkmm-3.0/gtkmm/label.h>


class YesNoAskerDialog final : public Gtk::Dialog
{
private:
    std::unique_ptr<Gtk::Label> m_textLabel;

public:
    YesNoAskerDialog(const Glib::ustring &question, Gtk::ResponseType defaultButton=Gtk::ResponseType::RESPONSE_YES);
};

bool askYesNo(const Glib::ustring &question, Gtk::ResponseType defaultButton=Gtk::ResponseType::RESPONSE_YES);
