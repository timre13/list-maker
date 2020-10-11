#pragma once

#include <glibmm-2.4/glibmm/ustring.h>


class ListItem final
{
private:
    Glib::ustring m_text;
    bool m_isDone{};

public:
    ListItem(const Glib::ustring &text);

    const Glib::ustring& getText() const { return m_text; }
    Glib::ustring& getText() { return m_text; }

    bool isDone() const { return m_isDone; }
    bool& isDone() { return m_isDone; }
};

