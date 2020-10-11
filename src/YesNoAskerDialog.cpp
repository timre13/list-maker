#include "YesNoAskerDialog.h"

YesNoAskerDialog::YesNoAskerDialog(const Glib::ustring &question, Gtk::ResponseType defaultButton/*=Gtk::ResponseType::RESPONSE_YES*/)
    : m_textLabel{std::make_unique<Gtk::Label>()}
{
    get_vbox()->add(*m_textLabel);

    m_textLabel->set_text(question);

    add_button("Yes", Gtk::ResponseType::RESPONSE_YES);
    add_button("No", Gtk::ResponseType::RESPONSE_NO);

    set_default_response(defaultButton);

    show_all_children();
}

bool askYesNo(const Glib::ustring &question, Gtk::ResponseType defaultButton/*=Gtk::ResponseType::RESPONSE_YES*/)
{
    auto dialog{std::make_unique<YesNoAskerDialog>(question, defaultButton)};
    return dialog->run() == Gtk::ResponseType::RESPONSE_YES;
}
