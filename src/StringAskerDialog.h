#pragma once

#include <memory>
#include <glibmm-2.4/glibmm/ustring.h>
#include <gtkmm-3.0/gtkmm/dialog.h>
#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm-3.0/gtkmm/entry.h>
#include <gtkmm-3.0/gtkmm/label.h>


class StringAskerDialog final : public Gtk::Dialog
{
private:
    std::unique_ptr<Gtk::Grid>  m_grid;
    std::unique_ptr<Gtk::Label> m_questionLabel;
    std::unique_ptr<Gtk::Entry> m_entry;

    Glib::ustring m_answer;

public:
    StringAskerDialog(const Glib::ustring &title, const Glib::ustring &question, const Glib::ustring &defaultValue);

    inline const Glib::ustring& getAnswer() const { return m_answer; }

    virtual void on_response(int responseId) override;
};

/* Returns the answered string or the default */
Glib::ustring askString(const Glib::ustring &dialogTitle, const Glib::ustring &question, const Glib::ustring &defaultValue);
/* Returns the answered string or "<CANCEL>" */
Glib::ustring askStringOrCancel(const Glib::ustring &dialogTitle, const Glib::ustring &question, const Glib::ustring &defaultValue);
