#include "../../headers/gui/ConsoleGuiHandler.h"
#include <iostream>
#include <utility>
#include <conio.h>

#include <cstdio>

ConsoleGuiHandler::ConsoleGuiHandler(HANDLE h_console) : utils(h_console), h_console(h_console), fd(".") {
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    GetConsoleScreenBufferInfo(h_console, &console_info);
    saved_attributes = console_info.wAttributes;

    menuDrawer = new MenuDrawer(this);
    helpDrawer = new HelpDrawer(this);
    fileCreator = new FileCreator(this);
    fileRenamer = new FileRenamer(this);
    fileDeleter = new FileDeleter(this);
    filePreviewer = new FilePreviewer(this);
    fileMover = new FileMover(this);

    SetConsoleCP(RUSSIAN_CP);
    SetConsoleOutputCP(RUSSIAN_CP);
    utils.clearScreen();

    list_files = fd.getFilesList();
    redrawConsoleGui();
}

ConsoleGuiHandler::~ConsoleGuiHandler() {
    SetConsoleTextAttribute(h_console, saved_attributes);
    std::cin.clear();
    fflush(stdin);
    utils.clearScreen();
    CloseHandle(h_console);
}

void ConsoleGuiHandler::redrawConsoleGui() {
    menuDrawer->redrawConsoleGui();
}

void ConsoleGuiHandler::cleanRedrawConsoleGui() {
    menuDrawer->cleanRedrawConsoleGui();
}

void ConsoleGuiHandler::moveDown() {
    int relative_index = current_selected_index + 1 - starting_index;
    if (relative_index < menuDrawer->MAX_FILES && current_selected_index < list_files.size() - 1) {
        current_selected_index++;
    } else if (relative_index == menuDrawer->MAX_FILES && current_selected_index < list_files.size() - 1) {
        current_selected_index++;
        starting_index += menuDrawer->MAX_FILES;
    } else if (current_selected_index == list_files.size() - 1) {
        starting_index = 0;
        current_selected_index = 0;
    }
    redrawConsoleGui();
}

void ConsoleGuiHandler::moveUp() {
    int relative_index = current_selected_index + 1 - starting_index;
    if (relative_index > 1) {
        current_selected_index--;
    }
    else if (relative_index == 1 && starting_index > 0) {
        current_selected_index--;
        starting_index -= menuDrawer->MAX_FILES;
    }
    else if (relative_index == 1 && starting_index == 0) {
        current_selected_index = list_files.size() - 1;
        starting_index = current_selected_index / menuDrawer->MAX_FILES * menuDrawer->MAX_FILES;
    }
    redrawConsoleGui();
}

void ConsoleGuiHandler::goUp() {
    reInitSafe("..");
}

void ConsoleGuiHandler::open() {
    std::string file_name = list_files[current_selected_index].getName();
    switch(list_files[current_selected_index].getType()) {
        case FileType::DIR:
            reInitSafe(file_name);
            break;
        case FileType::EXE:
        case FileType::ORD:
            std::string command = R"(start "" ")" + file_name + "\"";
            system(command.c_str());
            break;
    }
}

void ConsoleGuiHandler::openDir() {
    std::string file_name = list_files[current_selected_index].getName();
    if(list_files[current_selected_index].getType() == FileType::DIR)
        reInitSafe(file_name);
}

void ConsoleGuiHandler::reInit(std::string file_name) {
    redrawConsoleGui();
    //file_name = fd.getCurrentDirectory() + "/" + file_name;
    FiledirectoryException e = fd.reInit(file_name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        list_files = fd.getFilesList();
        if(current_selected_index >= list_files.size()) {
            current_selected_index = list_files.size() - 1;
        }
        if(starting_index >= list_files.size() && list_files.size() != 0) {
            starting_index -= menuDrawer->MAX_FILES;
        }
        redrawConsoleGui();
    } else {
        outputCorrespondingException(e);
    }
}

void ConsoleGuiHandler::reInitSafe(std::string file_name) {
    redrawConsoleGui();
    //file_name = fd.getCurrentDirectory() + "/" + file_name;
    FiledirectoryException e = fd.reInit(file_name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        list_files = fd.getFilesList();
        current_selected_index = 0;
        starting_index = 0;
        redrawConsoleGui();
    } else {
        outputCorrespondingException(e);
    }
}

void ConsoleGuiHandler::outputCorrespondingException(FiledirectoryException e) {
    redrawConsoleGui();
    switch(e) {
        case FiledirectoryException::ACCESS_DENIED:
            utils.outputLine("Access is denied!", saved_attributes);
            break;
        case FiledirectoryException::FILE_NOT_FOUND:
            utils.outputLine("File not found!", saved_attributes);
            break;
        case FiledirectoryException::INCORRECT_NAME:
            utils.outputLine("Preserved name!", saved_attributes);
            break;
        case FiledirectoryException::UNHANDLED:
            utils.outputLine("An error has occurred!", saved_attributes);
            break;
    }
}

bool ConsoleGuiHandler::checkFile(const std::string &name) {
    if(name.empty()) return false;
    if(fd.containsCurrent(name)) {
        redrawConsoleGui();
        utils.outputLine(FD_EXISTS, saved_attributes);
        return false;
    }
    return true;
}

void ConsoleGuiHandler::rename() {
    fileRenamer->rename();
}

void ConsoleGuiHandler::createFileOrDir() {
    fileCreator->createFileOrDir();
}

void ConsoleGuiHandler::deleteFile() {
    fileDeleter->deleteFile();
}

void ConsoleGuiHandler::showHelp() {
    helpDrawer->showHelp();
}

void ConsoleGuiHandler::showTextPreview() {
    filePreviewer->showTextPreview();
}

void ConsoleGuiHandler::showRawPreview() {
    filePreviewer->showRawPreview();
}

void ConsoleGuiHandler::saveLocation() {
    fileMover->saveLocation();
}

void ConsoleGuiHandler::moveFile() {
    fileMover->moveFile();
}

void ConsoleGuiHandler::copyFile() {
    fileMover->copyFile();
}