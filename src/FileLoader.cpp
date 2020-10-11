#include "FileLoader.h"
#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <chrono>
#include <gtkmm-3.0/gtkmm/messagedialog.h>
#include <glibmm-2.4/glibmm/fileutils.h>


#define FILELOADER_DEBUG 1

static constexpr int fileFormatVersion{3};

int FileLoader::load(const Glib::ustring &filename, boardList_t &boards, Glib::ustring &errorMessageOut)
{
    auto &errMsg{errorMessageOut};

    std::string fileContent{};
    try
    {
        fileContent = Glib::file_get_contents(filename);
    }
    catch (const Glib::FileError &e)
    {
        errMsg = e.what();
        return 1;
    }
    catch (...)
    {
        errMsg = "Unknown error";
        return 1;
    }

    std::string currentLine;
    std::stringstream fileContentSs{fileContent};
    int currentLineI{};

    int numOfWarnings{};

    bool isInsideBoard{};
    bool isInsideList{};
    bool isInsideItem{};
    bool wasPrevTagBoard{};
    bool wasPrevTagItem{};
    bool wasPrevLineBoardColor{};

    if (std::getline(fileContentSs, currentLine) && currentLine.length() > 8 && currentLine.find("version ") != std::string::npos)
    {
        if (currentLine[8] != std::to_string(fileFormatVersion)[0])
        {
            std::cerr << "ERROR: Incompatible version" << std::endl;
            errMsg = "Incompatible verson";
            return 1;
        }

#ifdef FILELOADER_DEBUG
        std::cout << "Version: OK" << std::endl;
#endif

        ++currentLineI;
    }
    else
    {
        std::cerr << "ERROR: Version information not found" << std::endl;
        errMsg = "Version information not found";
        return 1;
    }

    while (std::getline(fileContentSs, currentLine))
    {
        if (currentLine.length() == 0)
        {
#ifdef FILELOADER_DEBUG
            std::cout << "Current line is empty, skipping" << std::endl;
#endif
            ++currentLineI;
            continue;
        }

        currentLine = currentLine.substr(currentLine.find_first_not_of(" \t"));

#ifdef FILELOADER_DEBUG
        std::cout << currentLineI << ": " << currentLine << std::endl;
#endif

        if (currentLine.length() > 6 && currentLine.substr(0, 6).compare("board ") == 0)
        {
            if (isInsideBoard)
            {
                std::cerr << "ERROR: Board inside an another board" << std::endl;
                errMsg = "Board inside an another board";
                return 1;
            }
            if (isInsideList)
            {
                std::cerr << "ERROR: Board inside a list" << std::endl;
                errMsg = "Board inside a list";
                return 1;
            }
            if (isInsideItem)
            {
                std::cerr << "ERROR: Board inside an item" << std::endl;
                errMsg = "Board inside an item";
                return 1;
            }

#ifdef FILELOADER_DEBUG
            std::cout << "Board found" << std::endl;
#endif
            isInsideBoard = true;
            wasPrevTagBoard = true;

            boards.push_back(std::make_unique<Board>(currentLine.substr(6), Gdk::RGBA("white")));
        }
        else if (currentLine.length() > 5 && currentLine.substr(0, 5).compare("list ") == 0)
        {
            if (isInsideList)
            {
                std::cerr << "ERROR: List inside an another list" << std::endl;
                errMsg = "List inside an another list";
                return 1;
            }
            if (isInsideItem)
            {
                std::cerr << "ERROR: List inside an item" << std::endl;
                errMsg = "List inside an item";
                return 1;
            }

            if (!isInsideBoard)
            {
                std::cerr << "ERROR: List outside a board" << std::endl;
                errMsg = "List outside a board";
                return 1;
            }
#ifdef FILELOADER_DEBUG
            std::cout << "List found" << std::endl;
#endif
            isInsideList = true;

            boards[boards.size()-1]->addNewList(currentLine.substr(5));
        }
        else if (currentLine.length() > 5 && currentLine.substr(0, 5).compare("item ") == 0)
        {
            if (isInsideItem)
            {
                std::cerr << "ERROR: Item inside an another item" << std::endl;
                errMsg = "Item inside an another item";
                return 1;
            }

            if (!isInsideBoard)
            {
                std::cerr << "ERROR: Item outside a board" << std::endl;
                errMsg = "Item outside a board";
                return 1;
            }
            else if (!isInsideList)
            {
                std::cerr << "ERROR: Item outside a list" << std::endl;
                errMsg = "Item outside a list";
                return 1;
            }
#ifdef FILELOADER_DEBUG
            std::cout << "Item found" << std::endl;
#endif
            isInsideItem = true;
            wasPrevTagItem = true;

            boards[boards.size()-1]->getList(boards[boards.size()-1]->getNumOfLists()-1).addNewItem(currentLine.substr(5));
        }
        else if (currentLine.length() >= 3 && currentLine.substr(0, 3).compare("end") == 0)
        {
            if (isInsideItem)
            {
                if (wasPrevTagItem)
                {
                    std::cerr << "ERROR: End of item, but no isDone value specified" << std::endl;
                    errMsg = "End of item, but no isDone value specified";
                    return 1;
                }
#ifdef FILELOADER_DEBUG
                std::cout << "End of item" << std::endl;
#endif
                isInsideItem = false;
            }
            else if (isInsideList)
            {
#ifdef FILELOADER_DEBUG
                std::cout << "End of list" << std::endl;
#endif
                isInsideList = false;
                isInsideItem = false; // for sure
            }
            else if (isInsideBoard)
            {
                if (wasPrevTagBoard)
                {
                    std::cerr << "ERROR: End of board, but no color specified" << std::endl;
                    errMsg = "End of board, but no color specified";
                    return 1;
                }
#ifdef FILELOADER_DEBUG
                std::cout << "End of board" << std::endl;
#endif
                isInsideBoard = false;
                isInsideList = false; // for sure
                isInsideItem = false; // for sure
            }
            else
            {
                std::cerr << "ERROR: 'end' outside a board" << std::endl;
                errMsg = "'end outside a board'";
                return 1;
            }
        }
        else if (wasPrevTagBoard)
        {
            Gdk::RGBA color{Gdk::RGBA(currentLine)};
            // If the color is black (default value on error) and it shouldn't be
            if (color == Gdk::RGBA("black") &&
                // TODO: make this better
                (currentLine.compare("black") ||
                currentLine.compare("#0000") ||
                currentLine.compare("#000000") ||
                currentLine.compare("rgba(0,0,0,0)") ||
                currentLine.compare("rgb(0,0,0)")))
            {
                std::cerr << "ERROR: Invalid board color" << std::endl;
                errMsg = "Invalid board color";
                return 1;
            }
#ifdef FILELOADER_DEBUG
            std::cout << "Found a board color: " << color.to_string() << std::endl;
#endif
            color.set_alpha(1.0);
            boards[boards.size()-1]->getColor() = color;
            wasPrevTagBoard = false;
            wasPrevLineBoardColor = true;
        }
        else if (wasPrevLineBoardColor)
        {
            time_t modificationDate{};

            try
            {
                modificationDate = std::stoull(currentLine);
            }
            catch (std::exception&)
            {
                std::cerr << "Invalid board modification date" << std::endl;
                errMsg = "Invalid board modification date";
                return 1;
            }

#ifdef FILELOADER_DEBUG
            std::cout << "Found a board modification date: " << currentLine << std::endl;
#endif
            boards[boards.size()-1]->getModificationDate() = std::chrono::time_point<Board::clock_t>(std::chrono::duration<time_t>(modificationDate));
            wasPrevLineBoardColor = false;
        }
        else if (wasPrevTagItem)
        {
            auto &currentBoard{boards[boards.size()-1]};
            auto &currentList{currentBoard->getList(currentBoard->getNumOfLists()-1)};
            auto &currentItem{currentList.getItem(currentList.getNumOfItems()-1)};

            if (currentLine.compare("true") == 0)
            {
#ifdef FILELOADER_DEBUG
                std::cout << "Found an item isDone value: true" << std::endl;
#endif
                currentItem.isDone() = true;
            }
            else if (currentLine.compare("false") == 0)
            {
#ifdef FILELOADER_DEBUG
                std::cout << "Found an item isDone value: false" << std::endl;
#endif
                currentItem.isDone() = false;
            }
            else
            {
                std::cerr << "ERROR: Invalid value for item isDone" << std::endl;
                errMsg = "Invalid value for item isDone";
                return 1;
            }
            wasPrevTagItem = false;
        }
        else
        {
            std::cerr << "WARNING: Unexpected token, skipping" << std::endl;
            ++numOfWarnings;
            //return 1;
        }

        ++currentLineI;
    }

    if (isInsideBoard)
    {
        std::cerr << "WARNING: No end of board" << std::endl;
        ++numOfWarnings;
    }
    if (isInsideList)
    {
        std::cerr << "WARNING: No end of list" << std::endl;
        ++numOfWarnings;
    }
    if (isInsideItem)
    {
        std::cerr << "WARNING: No end of item" << std::endl;
        ++numOfWarnings;
    }

    std::cout <<
        "Parsing of " << filename << " finished with no errors, " <<
        (numOfWarnings ? std::to_string(numOfWarnings) : std::string("no")) <<
        " warning(s)" << std::endl;

    return 0;
}

int FileLoader::save(const Glib::ustring &filename, const boardList_t &boards, Glib::ustring &errorMessageOut)
{
    auto &errMsg{errorMessageOut};
    
    std::stringstream buffer{};

    buffer << "version " << std::to_string(fileFormatVersion) << std::endl << std::endl;

    for (size_t boardI{}; boardI < boards.size(); ++boardI)
    {
        auto &board{boards[boardI]};

        buffer << "board " << board->getName() << std::endl;
        buffer << std::string(4, ' ') << board->getColor().to_string() << std::endl;
        buffer << std::string(4, ' ') << board->getModificationDateTimeT() << std::endl;

        for (size_t listI{}; listI < board->getNumOfLists(); ++listI)
        {
            auto &list{board->getList(listI)};

            buffer << std::string(4, ' ') << "list " << list.getTitle() << std::endl;

            for (size_t itemI{}; itemI < list.getNumOfItems(); ++itemI)
            {
                auto &item{list.getItem(itemI)};

                buffer << std::string(8, ' ') << "item " << item.getText() << std::endl;
                buffer << std::string(12, ' ') << (item.isDone() ? "true" : "false") << std::endl;

                buffer << std::string(8, ' ') << "end" << std::endl;
            }
            buffer << std::string(4, ' ') << "end" << std::endl;
        }
        buffer << "end" << std::endl << std::endl;
    }

    try
    {
        Glib::file_set_contents(filename, buffer.str());
    }
    catch (Glib::FileError &e)
    {
        errMsg = e.what();
        return 1;
    }
    catch (...)
    {
        errMsg = "Unknown error";
        return 1;
    }

    std::cout << "Boards saved to " << filename << std::endl;

    return 0;
}
