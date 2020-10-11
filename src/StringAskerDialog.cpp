#include "StringAskerDialog.h"


StringAskerDialog::StringAskerDialog(const Glib::ustring &title, const Glib::ustring &question, const Glib::ustring &defaultValue)
    :
    Dialog(title, true),
    m_grid{std::make_unique<Gtk::Grid>()},
    m_questionLabel{std::make_unique<Gtk::Label>()},
    m_entry{std::make_unique<Gtk::Entry>()}
{
    set_resizable(false);

    m_questionLabel->set_text(question);
    m_entry->set_text(defaultValue);

    get_vbox()->add(*m_grid);
    m_grid->attach(*m_questionLabel, 0, 0);
    m_grid->attach(*m_entry, 0, 1);

    add_button("OK", Gtk::ResponseType::RESPONSE_OK);
    add_button("Cancel", Gtk::ResponseType::RESPONSE_CANCEL);

    show_all_children();
}

void StringAskerDialog::on_response(int responseId)
{
    if (responseId == Gtk::ResponseType::RESPONSE_OK)
        m_answer = m_entry->get_text();
    else
        m_answer.clear();
}

Glib::ustring askString(const Glib::ustring &dialogTitle, const Glib::ustring &question, const Glib::ustring &defaultValue)
{
    auto dialog{std::make_unique<StringAskerDialog>(dialogTitle, question, defaultValue)};

    if (dialog->run() != Gtk::ResponseType::RESPONSE_OK || dialog->getAnswer().size() == 0)
        return defaultValue;

    return dialog->getAnswer();
}

Glib::ustring askStringOrCancel(const Glib::ustring &dialogTitle, const Glib::ustring &question, const Glib::ustring &defaultValue)
{
    auto dialog{std::make_unique<StringAskerDialog>(dialogTitle, question, defaultValue)};

    if (dialog->run() != Gtk::ResponseType::RESPONSE_OK)
        return "<CANCEL>";

    return dialog->getAnswer();
}
