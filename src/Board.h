#pragma once

#include "List.h"
#include <vector>
#include <memory>
#include <chrono>
#include <cassert>
#include <glibmm-2.4/glibmm/ustring.h>
#include <gdkmm-3.0/gdkmm/rgba.h>


class Board final
{
public:
    using clock_t = std::chrono::system_clock;
    using timePoint_t = std::chrono::time_point<clock_t>;

private:
    Glib::ustring  m_name;
    Gdk::RGBA      m_color;
    timePoint_t    m_modificationDate;
    std::vector<std::unique_ptr<List>> m_lists{};

public:
    Board(const Glib::ustring &name, const Gdk::RGBA &color);

    inline Board(const Board& rhs) = delete;
    inline Board& operator=(const Board& rhs) = delete;

    //--------------------------------------------------------------------------

    inline const Glib::ustring& getName() const { return m_name; }
    inline Glib::ustring& getName() { return m_name; }

    //--------------------------------------------------------------------------

    inline const Gdk::RGBA& getColor() const { return m_color; }
    inline Gdk::RGBA& getColor() { return m_color; }

    //--------------------------------------------------------------------------

    inline const timePoint_t& getModificationDate() const { return m_modificationDate; }
    inline timePoint_t& getModificationDate() { return m_modificationDate; }
    
    inline const time_t getModificationDateTimeT() const { return clock_t::to_time_t(m_modificationDate); }

    inline const Glib::ustring getModificationDateStr() const { time_t time{clock_t::to_time_t(m_modificationDate)}; return std::ctime(&time); }

    inline void updateModificationDate() { m_modificationDate = clock_t::now(); }

    //--------------------------------------------------------------------------

    inline size_t getNumOfLists() const { return m_lists.size(); }

    inline const List& getList(size_t index) const { assert(index < m_lists.size()); return *(m_lists[index]); }
    inline List& getList(size_t index) { assert(index < m_lists.size()); return *(m_lists[index]); }

    inline void addNewList(const Glib::ustring &title) { m_lists.push_back(std::make_unique<List>(title)); }

    inline void removeList(size_t index) { assert(index < m_lists.size()); m_lists.erase(m_lists.begin()+index); }

    //--------------------------------------------------------------------------

    virtual ~Board();
};

using boardList_t = std::vector<std::unique_ptr<Board>>;
