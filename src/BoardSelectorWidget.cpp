#include "BoardSelectorWidget.h"
#include "MainWindow.h"
#include "StringAskerDialog.h"
#include "YesNoAskerDialog.h"
#include "AskRGB.h"
#include <gtkmm-3.0/gtkmm/messagedialog.h>


BoardSelectorWidget::BoardSelectorWidget(boardList_t *boards)
    :
    m_titleFont{"24"},
    m_boardNameFont{"14"},

    m_Titlelabel{std::make_unique<Gtk::Label>()},
    m_listBox{std::make_unique<Gtk::ListBox>()},

    m_editButtonGrid{std::make_unique<Gtk::Grid>()},
    m_createButton{std::make_unique<Gtk::Button>()},
    m_renameButton{std::make_unique<Gtk::Button>()},
    m_colorModifierButton{std::make_unique<Gtk::Button>()},
    m_deleteButton{std::make_unique<Gtk::Button>()},

    m_boardsPtr{boards}
{
    m_titleFont.set_weight(Pango::WEIGHT_BOLD);
    m_boardNameFont.set_weight(Pango::WEIGHT_BOLD);

    m_Titlelabel->set_text("Boards");
    m_Titlelabel->override_font(m_titleFont);
    m_Titlelabel->set_margin_bottom(20);

    m_listBox->set_hexpand();
    m_listBox->set_vexpand();

    // -------------------------------------------------------------------------

    m_createButton->set_label("Create");
    m_createButton->set_image_from_icon_name("document-new");
    m_createButton->set_tooltip_text("Create a new board");
    m_createButton->set_hexpand();

    m_renameButton->set_label("Rename");
    m_renameButton->set_image_from_icon_name("document-edit");
    m_renameButton->set_tooltip_text("Rename the selected board");
    m_renameButton->set_hexpand();

    m_colorModifierButton->set_label("Modify color");
    // TODO: find a better icon
    m_colorModifierButton->set_image_from_icon_name("colorimeter-colorhug");
    m_colorModifierButton->set_tooltip_text("Modify the color of the selected board");
    m_colorModifierButton->set_hexpand();

    m_deleteButton->set_label("Delete");
    m_deleteButton->set_image_from_icon_name("edit-delete");
    m_deleteButton->set_tooltip_text("Delete the selected board");
    m_deleteButton->set_hexpand();

    // -------------------------------------------------------------------------

    for (size_t i{}; i < boards->size(); ++i)
    {
        // Make a new listbox row
        m_listBoxRows.push_back(std::move(std::make_unique<Gtk::Grid>()));
        m_listBox->append(*m_listBoxRows[i]);


        // Create and place the board name label
        m_boardNameLabels.push_back(std::move(std::make_unique<Gtk::Label>()));
        m_boardNameLabels[i]->set_text(boards->operator[](i)->getName());
        m_boardNameLabels[i]->set_tooltip_text("Name");
        m_boardNameLabels[i]->override_font(m_boardNameFont);
        m_boardNameLabels[i]->override_color(boards->at(i)->getColor());
        m_boardNameLabels[i]->set_hexpand();
        m_boardNameLabels[i]->set_margin_start(10);
        m_boardNameLabels[i]->set_margin_end(10);
        m_listBoxRows[i]->attach(*m_boardNameLabels[i], 0, 0);


        // Create and place the board modification date label
        m_boardMDateLabels.push_back(std::move(std::make_unique<Gtk::Label>()));
        m_boardMDateLabels[i]->set_text(boards->operator[](i)->getModificationDateStr());
        m_boardMDateLabels[i]->set_tooltip_text("Modification date");
        m_boardMDateLabels[i]->set_hexpand();
        m_boardMDateLabels[i]->set_margin_start(10);
        m_boardMDateLabels[i]->set_margin_end(10);
        m_boardMDateLabels[i]->set_margin_bottom(10);
        m_listBoxRows[i]->attach(*m_boardMDateLabels[i], 0, 1);
    }

    // -------------------------------------------------------------------------

    attach(*m_Titlelabel, 0, 0);
    attach(*m_listBox, 0, 1);
    attach(*m_editButtonGrid, 0, 2);

    m_editButtonGrid->attach(*m_createButton, 0, 0);
    m_editButtonGrid->attach(*m_renameButton, 1, 0);
    m_editButtonGrid->attach(*m_colorModifierButton, 2, 0);
    m_editButtonGrid->attach(*m_deleteButton, 3, 0);

    child_property_width(*m_editButtonGrid) = 2;

    // -------------------------------------------------------------------------

    m_listBox->signal_row_selected().connect(sigc::mem_fun(*this, &BoardSelectorWidget::onBoardSelected));

    m_createButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardSelectorWidget::onCreateButtonClicked));
    m_renameButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardSelectorWidget::onRenameButtonClicked));
    m_colorModifierButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardSelectorWidget::onColorModifierButtonClicked));
    m_deleteButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardSelectorWidget::onDeleteButtonClicked));

    // -------------------------------------------------------------------------

    show_all_children();
}

void BoardSelectorWidget::onBoardSelected(Gtk::ListBoxRow *row)
{
    auto selectedRow{m_listBox->get_selected_row()};
    int selectedBoardIndex{};
    if (selectedRow) selectedBoardIndex = selectedRow->get_index();
    else selectedBoardIndex = -1;

    dynamic_cast<MainWindow*>(get_toplevel())->rebuildBoardViewerWidgets(selectedBoardIndex);
}

void BoardSelectorWidget::onCreateButtonClicked()
{
    auto boardName{askStringOrCancel("New Board", "Name of new board:", "New board")};

    if (boardName.compare("<CANCEL>") == 0)
        return;

    auto boardI{m_boardsPtr->size()};

    m_boardsPtr->push_back(std::make_unique<Board>(boardName, Gdk::RGBA("white")));

    // Make a new listbox row
    m_listBoxRows.push_back(std::move(std::make_unique<Gtk::Grid>()));
    m_listBox->append(*m_listBoxRows[boardI]);

    // Create and place the board name label
    m_boardNameLabels.push_back(std::move(std::make_unique<Gtk::Label>()));
    m_boardNameLabels[boardI]->set_text(m_boardsPtr->operator[](boardI)->getName());
    m_boardNameLabels[boardI]->set_tooltip_text("Name");
    m_boardNameLabels[boardI]->override_font(m_boardNameFont);
    m_boardNameLabels[boardI]->override_color(m_boardsPtr->operator[](boardI)->getColor());
    m_boardNameLabels[boardI]->set_hexpand();
    m_boardNameLabels[boardI]->set_margin_start(10);
    m_boardNameLabels[boardI]->set_margin_end(10);
    m_listBoxRows[boardI]->attach(*m_boardNameLabels[boardI], 0, 0);


    // Create and place the board creation date label
    m_boardMDateLabels.push_back(std::move(std::make_unique<Gtk::Label>()));
    m_boardMDateLabels[boardI]->set_text(m_boardsPtr->operator[](boardI)->getModificationDateStr());
    m_boardMDateLabels[boardI]->set_tooltip_text("Creation date");
    m_boardMDateLabels[boardI]->set_hexpand();
    m_boardMDateLabels[boardI]->set_margin_start(10);
    m_boardMDateLabels[boardI]->set_margin_end(10);
    m_boardMDateLabels[boardI]->set_margin_bottom(10);
    m_listBoxRows[boardI]->attach(*m_boardMDateLabels[boardI], 0, 1);

    m_listBoxRows[boardI]->show();
    m_listBoxRows[boardI]->show_all_children();
}

void BoardSelectorWidget::onRenameButtonClicked()
{
    auto selectedRow{m_listBox->get_selected_row()};

    if (!selectedRow)
    {
        auto dialog{std::make_unique<Gtk::MessageDialog>("No selected board", false, Gtk::MESSAGE_ERROR)};
        dialog->run();
        return;
    }

    auto selectedBoardIndex{selectedRow->get_index()};

    Glib::ustring newName{askString("Rename", "New name of board", m_boardsPtr->operator[](selectedBoardIndex)->getName())};

    m_boardsPtr->operator[](selectedBoardIndex)->getName() = newName;
    m_boardNameLabels[selectedBoardIndex]->set_text(newName);
    m_boardsPtr->operator[](selectedBoardIndex)->updateModificationDate();
    m_boardMDateLabels[selectedBoardIndex]->set_text(m_boardsPtr->operator[](selectedBoardIndex)->getModificationDateStr());
    dynamic_cast<MainWindow*>(get_toplevel())->rebuildBoardViewerWidgets(selectedBoardIndex);
}

void BoardSelectorWidget::onColorModifierButtonClicked()
{
    auto selectedRow{m_listBox->get_selected_row()};

    if (!selectedRow)
    {
        auto dialog{std::make_unique<Gtk::MessageDialog>("No selected board", false, Gtk::MESSAGE_ERROR)};
        dialog->run();
        return;
    }

    auto selectedBoardIndex{selectedRow->get_index()};

    auto newColor{askRGB(m_boardsPtr->operator[](selectedBoardIndex)->getColor())};

    m_boardsPtr->operator[](selectedBoardIndex)->getColor() = newColor;
    m_boardNameLabels[selectedBoardIndex]->override_color(newColor);
    m_boardsPtr->operator[](selectedBoardIndex)->updateModificationDate();
    m_boardMDateLabels[selectedBoardIndex]->set_text(m_boardsPtr->operator[](selectedBoardIndex)->getModificationDateStr());
    dynamic_cast<MainWindow*>(get_toplevel())->rebuildBoardViewerWidgets(selectedBoardIndex);
}

void BoardSelectorWidget::onDeleteButtonClicked()
{
    auto selectedRow{m_listBox->get_selected_row()};

    if (!selectedRow)
    {
        auto dialog{std::make_unique<Gtk::MessageDialog>("No selected board", false, Gtk::MESSAGE_ERROR)};
        dialog->run();
        return;
    }

    if (!askYesNo("Are sure you want to delete the whole board with all of its lists and items?"))
        return;

    auto selectedBoardIndex{selectedRow->get_index()};

    // Remove the listbox row from the listbox
    m_listBox->remove(*m_listBox->get_children()[selectedBoardIndex]);

    // Remove the widgets from the vectors
    m_boardNameLabels.erase(m_boardNameLabels.begin()+selectedBoardIndex);
    m_boardMDateLabels.erase(m_boardMDateLabels.begin()+selectedBoardIndex);
    m_listBoxRows.erase(m_listBoxRows.begin()+selectedBoardIndex);
    // Remove the `Board` object
    m_boardsPtr->erase(m_boardsPtr->begin()+selectedBoardIndex);
}
