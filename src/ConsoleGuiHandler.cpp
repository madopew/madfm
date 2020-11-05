#include "../headers/ConsoleGuiHandler.h"
#include <iostream>
#include <utility>
#include <conio.h>

ConsoleGuiHandler::ConsoleGuiHandler(HANDLE h_console) : utils(h_console), h_console(h_console), fd(".") {
    saveAttributes();
    SetConsoleOutputCP(RUSSIAN_CP);
    utils.clearScreen();
    reserveLines();
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

void ConsoleGuiHandler::saveAttributes() {
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    GetConsoleScreenBufferInfo(h_console, &console_info);
    saved_attributes = console_info.wAttributes;
}

void ConsoleGuiHandler::reserveLines() {
    current_lines.reserve(MAX_FILES + MAX_ADDITIONAL_LINES);
    for (int i = 0; i < MAX_FILES + MAX_ADDITIONAL_LINES; i++)
        current_lines.emplace_back();
    next_lines.reserve(MAX_FILES + MAX_ADDITIONAL_LINES);
}

void ConsoleGuiHandler::addDirLine() {
    std::string dir = std::string(CONST_PREFIXES[DIR_PREFIX]).append(Filedirectory::getCurrentDirectory());
    ConsoleLine dir_line(dir, saved_attributes);
    next_lines.push_back(dir_line);
    next_lines.emplace_back();
}

void ConsoleGuiHandler::addTopBottomLine(bool expression) {
    ConsoleLine line;
    if (expression) {
        int amount_of_pages = ((int) list_files.size() / MAX_FILES) + ((list_files.size() % MAX_FILES) != 0);
        int current_page = starting_index / MAX_FILES + 1;
        char buff[256] = {0};
        sprintf(buff, "%s%s (%d/%d)", CONST_PREFIXES[NOT_SELECTED_PREFIX].c_str(), CONST_PREFIXES[TOO_MANY_PREFIX].c_str(),
                current_page, amount_of_pages);
        line.setAttribute(TOO_MANY_ATTR);
        line.setText(buff);
    }
    next_lines.push_back(line);
}

void ConsoleGuiHandler::addSelected(int index) {
    ConsoleLine selected_line(CONST_PREFIXES[SELECTED_PREFIX], SELECTED_ATTR);
    appendFileInfo(selected_line, index);
    next_lines.push_back(selected_line);
}

void ConsoleGuiHandler::addNotSelected(int index) {
    ConsoleLine not_selected_line;
    switch(list_files[index].getType()) {
        case FileType::DIR:
            not_selected_line.setAttribute(DIR_ATTR);
            break;
        case FileType::EXE:
            not_selected_line.setAttribute(EXE_ATTR);
            break;
        case FileType::ORD:
            not_selected_line.setAttribute(saved_attributes);
            break;
    }
    not_selected_line.setText(CONST_PREFIXES[NOT_SELECTED_PREFIX]);
    appendFileInfo(not_selected_line, index);
    next_lines.push_back(not_selected_line);
}

inline void ConsoleGuiHandler::appendFileInfo(ConsoleLine& line, int index) {
    File f = list_files[index];
    std::string line_text = line.getText().append(f.getTime()).append(CONST_PREFIXES[TAB_PREFIX])
            .append(f.getSize()).append(CONST_PREFIXES[TAB_PREFIX]).append(f.getParsedName());
    line.setText(line_text);
}

void ConsoleGuiHandler::addFilesLines() {
    for (int i = starting_index; i < MAX_FILES + starting_index && i < list_files.size(); i++) {
        if (i == current_selected_index)
            addSelected(i);
        else
            addNotSelected(i);
    }
}

void ConsoleGuiHandler::redrawConsoleGui() {
    next_lines.clear();
    addDirLine();
    addTopBottomLine(starting_index);
    addFilesLines();
    addTopBottomLine(starting_index + MAX_FILES < list_files.size());
    drawDifferentGui();
}

void ConsoleGuiHandler::drawDifferentGui() {
    for (int i = 0; i < next_lines.size(); i++) {
        if (current_lines[i] != next_lines[i]) {
            COORD current_line_coord = { 0, (short) i };
            SetConsoleCursorPosition(h_console, current_line_coord);
            utils.clearCurrentLine();
            utils.outputLine(next_lines[i]);
            current_lines[i] = next_lines[i];
        }
    }
    reduceCurrentLines();
}

void ConsoleGuiHandler::reduceCurrentLines() {
    if (next_lines.size() < current_lines.size()) {
        COORD line_coord;
        for (int i = next_lines.size(); i < current_lines.size(); i++) {
            line_coord.X = 0;
            line_coord.Y = i;
            utils.clearLine(line_coord);
            current_lines[i] = ConsoleLine();
        }
        line_coord.X = 0;
        line_coord.Y = next_lines.size();
        SetConsoleCursorPosition(h_console, line_coord);
    }
}

void ConsoleGuiHandler::moveDown() {
    int absolute_index = current_selected_index + 1 - starting_index;
    if (absolute_index < MAX_FILES && current_selected_index < list_files.size() - 1) {
        current_selected_index++;
    } else if (absolute_index == MAX_FILES && current_selected_index < list_files.size() - 1) {
        current_selected_index++;
        starting_index += MAX_FILES;
    } else if (absolute_index == MAX_FILES && current_selected_index == list_files.size() - 1) {
        return;
    }
    redrawConsoleGui();
}

void ConsoleGuiHandler::moveUp() {
    int absolute_index = current_selected_index + 1 - starting_index;
    if (absolute_index > 1) {
        current_selected_index--;
    }
    else if (absolute_index == 1 && starting_index > 0) {
        current_selected_index--;
        starting_index -= MAX_FILES;
    }
    else if (absolute_index == 1 && starting_index == 0) {
        return;
    }
    redrawConsoleGui();
}

void ConsoleGuiHandler::open() {
    std::string file_name = list_files[current_selected_index].getName();
    switch(list_files[current_selected_index].getType()) {
        case FileType::DIR:
            reInit(file_name);
            break;
        case FileType::EXE:
        case FileType::ORD:
            std::string command = "start " + file_name;
            system(command.c_str());
            break;
    }
}

void ConsoleGuiHandler::reInit(std::string file_name) {
    file_name = Filedirectory::getCurrentDirectory() + "/" + file_name;
    FiledirectoryException ex = fd.reInit(file_name);
    switch(ex) {
        case FiledirectoryException::NO_EXCEPTION:
            list_files = fd.getFilesList();
            current_selected_index = 0;
            starting_index = 0;
            redrawConsoleGui();
            break;
        case FiledirectoryException::ACCESS_DENIED:
            utils.outputLine("Access is denied!", saved_attributes);
            break;
        case FiledirectoryException::FILE_NOT_FOUND:
            utils.outputLine("File not found!", saved_attributes);
            break;
        case FiledirectoryException::UNHANDLED:
            utils.outputLine("An error has occurred!", saved_attributes);
            break;
    }
}

void ConsoleGuiHandler::goUp() {
    reInit("..");
}

void ConsoleGuiHandler::rename() {
    while(_kbhit()) _getch();
}