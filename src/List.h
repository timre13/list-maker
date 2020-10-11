#pragma once

#include "ListItem.h"
#include <vector>
#include <memory>
#include <cassert>
#include <glibmm-2.4/glibmm/ustring.h>


class List final
{
private:
    Glib::ustring m_title;
    std::vector<std::unique_ptr<ListItem>> m_items;

public:
    List(const Glib::ustring &title);

    inline const Glib::ustring& getTitle() const { return m_title; }
    inline Glib::ustring& getTitle() { return m_title; }

    inline size_t getNumOfItems() const { return m_items.size(); }

    inline const ListItem& getItem(size_t index) const { assert(index < m_items.size()); return *(m_items[index]); }
    inline ListItem& getItem(size_t index) { assert(index < m_items.size()); return *(m_items[index]); }

    inline void addNewItem(const Glib::ustring &text) { m_items.push_back(std::make_unique<ListItem>(text)); }

    inline void removeItem(size_t index) { assert(index < m_items.size()); m_items.erase(m_items.begin()+index); }
};

