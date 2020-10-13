#include "MainWindow.h"
#include "AboutDialog.h"
#include "icon_paths.h"
#include <exception>
#include <iostream>


MainWindow::MainWindow(boardList_t *boards)
    :
    m_grid{std::make_unique<Gtk::Grid>()},
    m_stackSwitcherButtonGrid{std::make_unique<Gtk::Grid>()},
    m_stackSwitcherButton1{std::make_unique<Gtk::Button>()},
    m_stackSwitcherButton2{std::make_unique<Gtk::Button>()},
    m_stack{std::make_unique<Gtk::Stack>()},
    m_boardSelectorWidget{std::make_unique<BoardSelectorWidget>(boards)},
    m_boardViewerWidget{std::make_unique<BoardViewerWidget>(boards)},
    m_aboutButton{std::make_unique<Gtk::Button>()},
    // TODO: Catch exception
    m_stackSwitcherButton1Image{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_BOARDS, 16, 16))},
    m_stackSwitcherButton2Image{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_LIST, 16, 16))},
    m_aboutButtonImage{std::make_unique<Gtk::Image>(Gdk::Pixbuf::create_from_file(ICON_PATH_ABOUT, 16, 16))},
    m_boardsPtr{boards}
{
    set_title("List Maker");

    try
    {
        set_icon(Gdk::Pixbuf::create_from_file(ICON_PATH_APP_ICON));
    }
    catch (std::exception &e) { std::cerr << "Failed to load icon: " << e.what() << std::endl; }
    catch (Gdk::PixbufError &e) { std::cerr << "Failed to load icon (enum exception)" << std::endl; }
    catch (...) { std::cerr << "Failed to load icon" << std::endl; }

    m_stackSwitcherButtonGrid->set_hexpand();
    m_stackSwitcherButtonGrid->set_tooltip_text("Switch between board selector and board viewer modes");

    m_stackSwitcherButton1->set_image((Gtk::Widget&)*m_stackSwitcherButton1Image);
    m_stackSwitcherButton1->set_size_request(50, 40);
    m_stackSwitcherButton2->set_image((Gtk::Widget&)*m_stackSwitcherButton2Image);
    m_stackSwitcherButton2->set_size_request(50, 40);

    m_aboutButton->set_image((Gtk::Widget&)*m_aboutButtonImage);
    m_aboutButton->set_label("About...");
    m_aboutButton->set_tooltip_text("Show the about dialog");

    add(*m_grid);
    m_grid->attach(*m_stackSwitcherButtonGrid, 0, 0);
    m_grid->attach(*m_stack, 0, 1);
    m_grid->child_property_width(*m_stack) = 2;
    m_grid->attach(*m_aboutButton, 1, 0);
    m_stack->add(*m_boardSelectorWidget, "BoardSelectorWidget");
    m_stack->add(*m_boardViewerWidget, "BoardViewerWidget");
    m_stackSwitcherButtonGrid->attach(*m_stackSwitcherButton1, 0, 0);
    m_stackSwitcherButtonGrid->attach(*m_stackSwitcherButton2, 1, 0);

    m_stack->set_transition_type(Gtk::StackTransitionType::STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);

    m_stackSwitcherButton1->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onStackSwitcherButton1Clicked));
    m_stackSwitcherButton2->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onStackSwitcherButton2Clicked));
    m_aboutButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onAboutButtonClicked));

    show_all_children();
}

void MainWindow::rebuildBoardViewerWidgets(int boardIndex)
{
    m_boardViewerWidget->rebuildWidgets(boardIndex);

    Glib::ustring currentBoardName{m_boardViewerWidget->getCurrentBoardName()};

    if (currentBoardName.size())
        set_title("List Maker - "+currentBoardName);
    else
        set_title("List Maker");
}

void MainWindow::onAboutButtonClicked()
{
    auto aboutDialog{std::make_unique<AboutDialog>()};
    aboutDialog->run();
}

void MainWindow::onStackSwitcherButton1Clicked()
{
    m_stack->set_visible_child(*m_boardSelectorWidget);
}

void MainWindow::onStackSwitcherButton2Clicked()
{
    m_stack->set_visible_child(*m_boardViewerWidget);
}
