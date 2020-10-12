#pragma once

#include "Board.h"
#include <memory>
#include <gtkmm-3.0/gtkmm/scrolledwindow.h>
#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm-3.0/gtkmm/label.h>
#include <gtkmm-3.0/gtkmm/listbox.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/image.h>
#include <gtkmm-3.0/gtkmm/dialog.h>
#include <glibmm-2.4/glibmm/ustring.h>
#include <pangomm-1.4/pangomm/fontdescription.h>
#include <gdkmm-3.0/gdkmm/event.h>


class BoardViewerWidget final : public Gtk::Grid
{
private:
    Pango::FontDescription m_titleFont;
    Pango::FontDescription m_listTitleFont;
    Pango::FontDescription m_listItemFont;
    Pango::FontDescription m_editButtonGridLabelFont;

    std::unique_ptr<Gtk::Label> m_titleLabel;
    std::unique_ptr<Gtk::ScrolledWindow> m_scrolledWindow;
    std::unique_ptr<Gtk::Grid> m_scrolledGrid;

    std::vector<std::unique_ptr<Gtk::Label>> m_listTitleLabels;
    std::vector<std::unique_ptr<Gtk::ListBox>> m_listListBoxes;
    std::vector<std::unique_ptr<Gtk::Label>> m_itemLabels;

    std::unique_ptr<Gtk::Grid> m_itemEditButtonGrid;
    std::unique_ptr<Gtk::Label> m_itemEditButtonGridLabel;
    std::unique_ptr<Gtk::Button> m_itemCreateButton;
    std::unique_ptr<Gtk::Button> m_itemMarkDoneButton;
    std::unique_ptr<Gtk::Button> m_itemRenameButton;
    std::unique_ptr<Gtk::Button> m_itemDeleteButton;
    std::unique_ptr<Gtk::Image> m_itemDeleteButtonImage;
    std::unique_ptr<Gtk::Image> m_itemCreateButtonImage;
    std::unique_ptr<Gtk::Image> m_itemMarkDoneButtonImage;
    std::unique_ptr<Gtk::Image> m_itemRenameButtonImage;

    std::unique_ptr<Gtk::Grid> m_listEditButtonGrid;
    std::unique_ptr<Gtk::Label> m_listEditButtonGridLabel;
    std::unique_ptr<Gtk::Button> m_listCreateButton;
    std::unique_ptr<Gtk::Button> m_listRenameButton;
    std::unique_ptr<Gtk::Button> m_listDeleteButton;
    std::unique_ptr<Gtk::Image> m_listCreateButtonImage;
    std::unique_ptr<Gtk::Image> m_listRenameButtonImage;
    std::unique_ptr<Gtk::Image> m_listDeleteButtonImage;

    boardList_t *m_boardsPtr;

    int m_boardIndex{-1};
    int m_selectedListIndex{-1};
    int m_selectedItemIndex{-1};

    void onItemSelected(Gtk::ListBoxRow *selectedRow);

    bool showErrorIfNoSelectedItem();
    bool showErrorIfNoSelectedList();
    bool showErrorIfNoOpenBoard();

    void onItemCreateButtonClicked();
    void onItemMarkDoneButtonClicked();
    void onItemRenameButtonClicked();
    void onItemDeleteButtonClicked();

    void onListCreateButtonClicked();
    void onListRenameButtonClicked();
    void onListDeleteButtonClicked();

public:
    BoardViewerWidget(boardList_t *boards);

    void rebuildWidgets(int boardIndex);

    inline Glib::ustring getCurrentBoardName()
    {
        if (m_boardIndex >= 0 && m_boardsPtr->size())
            return m_boardsPtr->operator[](m_boardIndex)->getName();

        return "";
    }
};
