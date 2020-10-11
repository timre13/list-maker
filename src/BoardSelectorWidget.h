#pragma once

#include "Board.h"
#include <memory>
#include <vector>
#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm-3.0/gtkmm/label.h>
#include <gtkmm-3.0/gtkmm/listbox.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <pangomm-1.4/pangomm/fontdescription.h>


class BoardSelectorWidget final : public Gtk::Grid
{
private:
    Pango::FontDescription m_titleFont;
    Pango::FontDescription m_boardNameFont;

    std::unique_ptr<Gtk::Label>   m_Titlelabel;
    std::unique_ptr<Gtk::ListBox> m_listBox;

    std::unique_ptr<Gtk::Grid>   m_editButtonGrid;
    std::unique_ptr<Gtk::Button> m_createButton;
    std::unique_ptr<Gtk::Button> m_renameButton;
    std::unique_ptr<Gtk::Button> m_colorModifierButton;
    std::unique_ptr<Gtk::Button> m_deleteButton;

    std::vector<std::unique_ptr<Gtk::Grid>> m_listBoxRows;

    std::vector<std::unique_ptr<Gtk::Label>> m_boardNameLabels;
    std::vector<std::unique_ptr<Gtk::Label>> m_boardMDateLabels;

    boardList_t *m_boardsPtr{nullptr};

    void showBoards();

    void onBoardSelected(Gtk::ListBoxRow *row);

    void onCreateButtonClicked();
    void onRenameButtonClicked();
    void onColorModifierButtonClicked();
    void onDeleteButtonClicked();

public:
    BoardSelectorWidget(boardList_t *boards);
};
