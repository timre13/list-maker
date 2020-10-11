#include "MainWindow.h"
#include "Board.h"
#include "FileLoader.h"
#include <memory>
#include <gtkmm-3.0/gtkmm/application.h>
#include <gtkmm-3.0/gtkmm/messagedialog.h>


int main(int argc, char* argv[])
{
    auto app = Gtk::Application::create(argc, argv, "org.timre13.listmaker");
    
    // -------------------------------------------------------------------------

    boardList_t boards;

    Glib::ustring loadErrorMessage;
    if (FileLoader::load("boards.lmf", boards, loadErrorMessage))
    {
        auto dialog{std::make_unique<Gtk::MessageDialog>(
                "Failed to load file: "+loadErrorMessage,
                false,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK,
                true
        )};
        dialog->run();
        // Remove the partially loaded boards
        boards.clear();
    }

    // -------------------------------------------------------------------------

    auto mainWindow{std::make_shared<MainWindow>(&boards)};
    mainWindow->set_default_size(500, 400);

    // -------------------------------------------------------------------------

    auto status{app->run(*mainWindow)};

    Glib::ustring saveErrorMessage;
    if (FileLoader::save("boards.lmf", boards, saveErrorMessage))
    {
        auto dialog{std::make_unique<Gtk::MessageDialog>(
                "Failed to save file: "+saveErrorMessage,
                false,
                Gtk::MESSAGE_ERROR,
                Gtk::BUTTONS_OK,
                true
        )};
        dialog->run();
    }

    return status;}
