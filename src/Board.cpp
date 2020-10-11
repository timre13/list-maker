#include "Board.h"


Board::Board(const Glib::ustring &name, const Gdk::RGBA &color)
    :
    m_name{name},
    m_color{color}
{
    updateModificationDate();
}

Board::~Board()
{
}
