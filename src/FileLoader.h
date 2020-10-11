#pragma once

#include "Board.h"
#include <vector>
#include <memory>
#include <glibmm-2.4/glibmm/ustring.h>


class FileLoader final
{
    static Glib::ustring m_lastError;

public:
    FileLoader() = delete;

    static int load(const Glib::ustring &filename, boardList_t &boards, Glib::ustring &errorMessageOut);
    static int save(const Glib::ustring &filename, const boardList_t &boards, Glib::ustring &errorMessageOut);
};

