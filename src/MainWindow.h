#pragma once

#include "Board.h"
#include "BoardSelectorWidget.h"
#include "BoardViewerWidget.h"
#include "icon_paths.h"
#include <memory>
#include <gtkmm-3.0/gtkmm/applicationwindow.h>
#include <gtkmm-3.0/gtkmm/grid.h>
#include <gtkmm-3.0/gtkmm/stack.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/image.h>
#include <gdkmm-3.0/gdkmm/pixbuf.h>


class MainWindow final : public Gtk::ApplicationWindow
{
private:
    std::unique_ptr<Gtk::Grid> m_grid;
    std::unique_ptr<Gtk::Grid> m_stackSwitcherButtonGrid;

    std::unique_ptr<Gtk::Button> m_stackSwitcherButton1;
    std::unique_ptr<Gtk::Button> m_stackSwitcherButton2;
    std::unique_ptr<Gtk::Image> m_stackSwitcherButton1Image;
    std::unique_ptr<Gtk::Image> m_stackSwitcherButton2Image;

    std::unique_ptr<Gtk::Stack> m_stack;
    std::unique_ptr<BoardSelectorWidget> m_boardSelectorWidget;
    std::unique_ptr<BoardViewerWidget> m_boardViewerWidget;

    std::unique_ptr<Gtk::Button> m_aboutButton;
    std::unique_ptr<Gtk::Image> m_aboutButtonImage;

    boardList_t *m_boardsPtr;

    void onAboutButtonClicked();
    void onStackSwitcherButton1Clicked();
    void onStackSwitcherButton2Clicked();

public:
    MainWindow(boardList_t *boards);

    void rebuildBoardViewerWidgets(int boardIndex);
};
