#include "BoardViewerWidget.h"

#include "YesNoAskerDialog.h"
#include "StringAskerDialog.h"
#include "icon_paths.h"
#include <iostream>
#include <gtkmm-3.0/gtkmm/messagedialog.h>
#include <gdkmm-3.0/gdkmm/pixbuf.h>


BoardViewerWidget::BoardViewerWidget(boardList_t *boards)
    :
    m_titleFont{"24"},
    m_listTitleFont{"16"},
    m_listItemFont{"12"},
    m_editButtonGridLabelFont{"14"},

    m_titleLabel{std::make_unique<Gtk::Label>()},
    m_scrolledWindow{std::make_unique<Gtk::ScrolledWindow>()},
    m_scrolledGrid{std::make_unique<Gtk::Grid>()},

    m_itemEditButtonGrid{std::make_unique<Gtk::Grid>()},
    m_itemEditButtonGridLabel{std::make_unique<Gtk::Label>()},
    m_itemCreateButton{std::make_unique<Gtk::Button>()},
    m_itemMarkDoneButton{std::make_unique<Gtk::Button>()},
    m_itemRenameButton{std::make_unique<Gtk::Button>()},
    m_itemDeleteButton{std::make_unique<Gtk::Button>()},
    // TODO: On fail: catch exceptions, print error message and set built-in icons
    m_itemCreateButtonImage{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_CREATE, 16, 16))},
    m_itemMarkDoneButtonImage{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_MDONE, 16, 16))},
    m_itemRenameButtonImage{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_RENAME, 16, 16))},
    m_itemDeleteButtonImage{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_DELETE, 16, 16))},

    m_listEditButtonGrid{std::make_unique<Gtk::Grid>()},
    m_listEditButtonGridLabel{std::make_unique<Gtk::Label>()},
    m_listCreateButton{std::make_unique<Gtk::Button>()},
    m_listRenameButton{std::make_unique<Gtk::Button>()},
    m_listDeleteButton{std::make_unique<Gtk::Button>()},
    m_listCreateButtonImage{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_CREATE, 16, 16))},
    m_listRenameButtonImage{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_RENAME, 16, 16))},
    m_listDeleteButtonImage{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_DELETE, 16, 16))},

    m_boardsPtr{boards}
{
    // -------------------------------------------------------------------------

    m_titleLabel->override_font(m_titleFont);
    m_titleLabel->set_hexpand();
    m_titleLabel->set_margin_bottom(20);

    m_scrolledWindow->set_vexpand();
    m_scrolledWindow->set_hexpand();

    // -------------------------------------------------------------------------

    m_itemEditButtonGridLabel->set_text("Item");
    m_itemEditButtonGridLabel->override_font(m_editButtonGridLabelFont);
    m_itemEditButtonGridLabel->set_hexpand();

    m_itemCreateButton->set_image((Gtk::Widget&)*m_itemCreateButtonImage);
    m_itemCreateButton->set_label("New");
    m_itemCreateButton->set_tooltip_text("Create new item in the selected list");
    m_itemCreateButton->set_hexpand();

    m_itemMarkDoneButton->set_image((Gtk::Widget&)*m_itemMarkDoneButtonImage);
    m_itemMarkDoneButton->set_label("Done");
    m_itemMarkDoneButton->set_tooltip_text("Mark the selected item as done");
    m_itemMarkDoneButton->set_hexpand();

    m_itemRenameButton->set_image((Gtk::Widget&)*m_itemRenameButtonImage);
    m_itemRenameButton->set_label("Rename");
    m_itemRenameButton->set_tooltip_text("Rename the selected item");
    m_itemRenameButton->set_hexpand();

    m_itemDeleteButton->set_image((Gtk::Widget&)*m_itemDeleteButtonImage);
    m_itemDeleteButton->set_label("Delete");
    m_itemDeleteButton->set_tooltip_text("Delete the selected item");
    m_itemDeleteButton->set_hexpand();

    // -------------------------------------------------------------------------

    m_listEditButtonGridLabel->set_text("List");
    m_listEditButtonGridLabel->override_font(m_editButtonGridLabelFont);
    m_listEditButtonGridLabel->set_hexpand();

    m_listCreateButton->set_image((Gtk::Widget&)*m_listCreateButtonImage);
    m_listCreateButton->set_label("New");
    m_listCreateButton->set_tooltip_text("Create new list");
    m_listCreateButton->set_hexpand();

    m_listRenameButton->set_image((Gtk::Widget&)*m_listRenameButtonImage);
    m_listRenameButton->set_label("Rename");
    m_listRenameButton->set_tooltip_text("Rename the selected list");
    m_listRenameButton->set_hexpand();

    m_listDeleteButton->set_image((Gtk::Widget&)*m_listDeleteButtonImage);
    m_listDeleteButton->set_label("Delete");
    m_listDeleteButton->set_tooltip_text("Delete the selected list");
    m_listDeleteButton->set_hexpand();

    // -------------------------------------------------------------------------

    rebuildWidgets(-1);

    // -------------------------------------------------------------------------

    attach(*m_titleLabel, 0, 0);
    attach(*m_scrolledWindow, 0, 1);
    attach(*m_itemEditButtonGrid, 0, 2);
    attach(*m_listEditButtonGrid, 0, 3);
    m_scrolledWindow->add(*m_scrolledGrid);

    child_property_width(*m_itemEditButtonGrid) = 2;
    child_property_width(*m_listEditButtonGrid) = 2;

    m_itemEditButtonGrid->attach(*m_itemEditButtonGridLabel, 0, 0);
    m_itemEditButtonGrid->child_property_width(*m_itemEditButtonGridLabel) = 4;
    m_itemEditButtonGrid->attach(*m_itemMarkDoneButton, 0, 1);
    m_itemEditButtonGrid->attach(*m_itemCreateButton, 1, 1);
    m_itemEditButtonGrid->attach(*m_itemRenameButton, 2, 1);
    m_itemEditButtonGrid->attach(*m_itemDeleteButton, 3, 1);

    m_listEditButtonGrid->attach(*m_listEditButtonGridLabel, 0, 0);
    m_listEditButtonGrid->child_property_width(*m_listEditButtonGridLabel) = 4;
    m_listEditButtonGrid->attach(*m_listCreateButton, 1, 1);
    m_listEditButtonGrid->attach(*m_listRenameButton, 2, 1);
    m_listEditButtonGrid->attach(*m_listDeleteButton, 3, 1);

    // -------------------------------------------------------------------------

    m_itemMarkDoneButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardViewerWidget::onItemMarkDoneButtonClicked));
    m_itemCreateButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardViewerWidget::onItemCreateButtonClicked));
    m_itemRenameButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardViewerWidget::onItemRenameButtonClicked));
    m_itemDeleteButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardViewerWidget::onItemDeleteButtonClicked));

    // -------------------------------------------------------------------------

    m_listCreateButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardViewerWidget::onListCreateButtonClicked));
    m_listRenameButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardViewerWidget::onListRenameButtonClicked));
    m_listDeleteButton->signal_clicked().connect(sigc::mem_fun(*this, &BoardViewerWidget::onListDeleteButtonClicked));

    // -------------------------------------------------------------------------

    show_all_children();
}

void BoardViewerWidget::rebuildWidgets(int boardIndex)
{
    m_boardIndex = boardIndex;

    // Remove the listboxes and list title labels
    for (auto &child : m_scrolledGrid->get_children())
        m_scrolledGrid->remove(*child);

    m_listListBoxes.clear();
    m_itemLabels.clear();
    m_listTitleLabels.clear();

    // -------------------------------------------------------------------------

    if (boardIndex >= 0 && m_boardsPtr->size())
    {
        m_titleLabel->set_text(m_boardsPtr->operator[](boardIndex)->getName());
        m_titleLabel->override_color(m_boardsPtr->operator[](boardIndex)->getColor());


        auto &board{m_boardsPtr->operator[](boardIndex)};

        for (size_t listI{}; listI < board->getNumOfLists(); ++listI)
        {
            auto &list{board->getList(listI)};

            size_t listTitleLabelI{m_listTitleLabels.size()};
            m_listTitleLabels.push_back(std::make_unique<Gtk::Label>());

            m_listTitleLabels[listTitleLabelI]->set_text(list.getTitle());
            m_listTitleLabels[listTitleLabelI]->override_font(m_listTitleFont);
            m_scrolledGrid->attach(*m_listTitleLabels[listTitleLabelI], listI, 0);
            m_listTitleLabels[listTitleLabelI]->show();

            size_t listBoxI{m_listListBoxes.size()};
            m_listListBoxes.push_back(std::make_unique<Gtk::ListBox>());

            m_listListBoxes[listBoxI]->set_hexpand();
            m_listListBoxes[listBoxI]->set_vexpand();
            m_scrolledGrid->attach(*m_listListBoxes[listBoxI], listI, 1);
            m_listListBoxes[listBoxI]->show();

            m_listListBoxes[listBoxI]->signal_row_selected().connect(sigc::mem_fun(*this, &BoardViewerWidget::onItemSelected));

            if (list.getNumOfItems() == 0)
            {
                size_t itemLabelI{m_itemLabels.size()};
                m_itemLabels.push_back(std::make_unique<Gtk::Label>());

                m_itemLabels[itemLabelI]->set_text("<empty>");
                m_itemLabels[itemLabelI]->override_font(m_listItemFont);
                m_listListBoxes[listBoxI]->append(*m_itemLabels[itemLabelI]);
                m_itemLabels[itemLabelI]->show();
            }
            else
            {
                for (size_t itemI{}; itemI < list.getNumOfItems(); ++itemI)
                {
                    auto &item{list.getItem(itemI)};

                    size_t itemLabelI{m_itemLabels.size()};
                    m_itemLabels.push_back(std::make_unique<Gtk::Label>());

                    m_itemLabels[itemLabelI]->set_text(item.getText());
                    m_itemLabels[itemLabelI]->override_font(m_listItemFont);
                    if (item.isDone())
                        m_itemLabels[itemLabelI]->override_color(Gdk::RGBA("lime"));
                    m_listListBoxes[listBoxI]->append(*m_itemLabels[itemLabelI]);
                    m_itemLabels[itemLabelI]->show();
                }
            }
        }
    }
    else
    {
        m_titleLabel->set_text("Select a board");

        // FIXME: use the default color, not a hardcoded one
        m_titleLabel->override_color(Gdk::RGBA("white"));
    }
}

// -------------------------------------------------------------------------

void BoardViewerWidget::onItemSelected(Gtk::ListBoxRow *selectedRow)
{
    for (int i{}; i < 2; ++i)
    {
        for (size_t listBoxI{}; listBoxI < m_listListBoxes.size(); ++listBoxI)
        {
            auto &listBox{m_listListBoxes[listBoxI]};

            bool isMother{};

            for (auto &row : listBox->get_children())
            {
                if (row == selectedRow)
                {
                    isMother = true;
                    break;
                }
            }

            if (isMother)
            {
                listBox->select_row(*selectedRow);
                // Mark the selected list's title
                m_listTitleLabels[listBoxI]->override_color(Gdk::RGBA("#77bbff"));

                m_selectedListIndex = listBoxI;
                m_selectedItemIndex = selectedRow->get_index();
            }
            else
            {
                listBox->unselect_all();
                // TODO: don't use hardcoded color
                m_listTitleLabels[listBoxI]->override_color(Gdk::RGBA("white"));
            }
        }
    }
}

// -------------------------------------------------------------------------

bool BoardViewerWidget::showErrorIfNoSelectedItem()
{
    if (showErrorIfNoSelectedList()) return true;

    if (m_boardsPtr->operator[](m_boardIndex)->getList(m_selectedListIndex).getNumOfItems() == 0)
    {
        auto dialog{std::make_unique<Gtk::MessageDialog>("No selected item", false, Gtk::MESSAGE_ERROR)};
        dialog->run();
        return true;
    }

    return false;
}

bool BoardViewerWidget::showErrorIfNoSelectedList()
{
    if (showErrorIfNoOpenBoard()) return true;

    if (m_selectedListIndex == -1 ||
        m_selectedItemIndex == -1)
    {
        auto dialog{std::make_unique<Gtk::MessageDialog>("No selected list", false, Gtk::MESSAGE_ERROR)};
        dialog->run();
        return true;
    }

    return false;
}

bool BoardViewerWidget::showErrorIfNoOpenBoard()
{
    if (m_boardIndex == -1)
    {
        auto dialog{std::make_unique<Gtk::MessageDialog>("No open board", false, Gtk::MESSAGE_ERROR)};
        dialog->run();
        return true;
    }

    return false;
}

// -------------------------------------------------------------------------

void BoardViewerWidget::onItemCreateButtonClicked()
{
    if (showErrorIfNoSelectedList()) return;

    auto itemName{askStringOrCancel("New item", "New item name:", "New item")};
    if (itemName.compare("<CANCEL>") == 0)
        return;

    // Append the new item to the same list as the selected
    m_boardsPtr->operator[](m_boardIndex)->getList(m_selectedListIndex).addNewItem(itemName);
    m_boardsPtr->operator[](m_boardIndex)->updateModificationDate();

    // TODO: Create a label, don't rebuild everything
    rebuildWidgets(m_boardIndex);

    //m_selectedItemIndex = m_boardsPtr->operator[](m_boardIndex)->getList(m_selectedListIndex).getNumOfItems()-1;
    m_selectedItemIndex = -1;
}

void BoardViewerWidget::onItemRenameButtonClicked()
{
    if (showErrorIfNoSelectedItem()) return;

    auto &selectedItem{m_boardsPtr->operator[](m_boardIndex)->getList(m_selectedListIndex).getItem(m_selectedItemIndex)};
    m_boardsPtr->operator[](m_boardIndex)->updateModificationDate();

    auto itemName{askString("Rename item", "New name of item:", selectedItem.getText())};

    selectedItem.getText() = itemName;
    m_boardsPtr->operator[](m_boardIndex)->updateModificationDate();

    // TODO: Modify the label, don't rebuild everything
    rebuildWidgets(m_boardIndex);

    m_selectedItemIndex = -1;
}

void BoardViewerWidget::onItemMarkDoneButtonClicked()
{
    if (showErrorIfNoSelectedItem()) return;

    m_boardsPtr->operator[](m_boardIndex)->getList(m_selectedListIndex).getItem(m_selectedItemIndex).isDone() = true;
    m_boardsPtr->operator[](m_boardIndex)->updateModificationDate();

    // TODO: Modify color of widget, don't rebuild everything
    rebuildWidgets(m_boardIndex);

    m_selectedItemIndex = -1;
}

void BoardViewerWidget::onItemDeleteButtonClicked()
{
    if (showErrorIfNoSelectedItem()) return;

    if (!askYesNo("Delete item?"))
        return;

    m_boardsPtr->operator[](m_boardIndex)->getList(m_selectedListIndex).removeItem(m_selectedItemIndex);
    m_boardsPtr->operator[](m_boardIndex)->updateModificationDate();

    // TODO: Delete the label, don't rebuild everything
    rebuildWidgets(m_boardIndex);

    m_selectedItemIndex = -1;
}

// -------------------------------------------------------------------------

void BoardViewerWidget::onListCreateButtonClicked()
{
    if (showErrorIfNoOpenBoard()) return;

    auto listName{askStringOrCancel("New list", "New list name:", "New list")};
    if (listName.compare("<CANCEL>") == 0)
        return;

    m_boardsPtr->operator[](m_boardIndex)->addNewList(listName);
    m_boardsPtr->operator[](m_boardIndex)->updateModificationDate();

    // TODO: Create a label and list, don't rebuild everything
    rebuildWidgets(m_boardIndex);

    m_selectedItemIndex = -1;
}

void BoardViewerWidget::onListRenameButtonClicked()
{
    if (showErrorIfNoSelectedList()) return;

    auto &selectedList{m_boardsPtr->operator[](m_boardIndex)->getList(m_selectedListIndex)};

    auto newName{askString("Rename list", "New name of list:", selectedList.getTitle())};

    selectedList.getTitle() = newName;
    m_boardsPtr->operator[](m_boardIndex)->updateModificationDate();

    // TODO: Modifiy the label, don't rebuild everything
    rebuildWidgets(m_boardIndex);

    m_selectedItemIndex = -1;
}

void BoardViewerWidget::onListDeleteButtonClicked()
{
    if (showErrorIfNoSelectedList()) return;

    if (!askYesNo("Delete list?"))
        return;

    m_boardsPtr->operator[](m_boardIndex)->removeList(m_selectedListIndex);
    m_boardsPtr->operator[](m_boardIndex)->updateModificationDate();

    // TODO: Remove the label and list, don't rebuild everything
    rebuildWidgets(m_boardIndex);

    m_selectedItemIndex = -1;
}

