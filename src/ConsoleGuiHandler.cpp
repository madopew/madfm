#include "../headers/ConsoleGuiHandler.h"
#include <iostream>
#include <utility>

ConsoleGuiHandler::ConsoleGuiHandler(HANDLE h_console) : h_console(h_console), fd(".") {
    CONSOLE_SCREEN_BUFFER_INFO console_info;
    GetConsoleScreenBufferInfo(h_console, &console_info);
    saved_attributes = console_info.wAttributes;
    SetConsoleOutputCP(RUSSIAN_CP);
    clearScreen();

    current_lines.reserve(MAX_FILES + MAX_ADDITIONAL_LINES);
    fillEmptyLines(current_lines);
    next_lines.reserve(MAX_FILES + MAX_ADDITIONAL_LINES);

    list_files = fd.getFilesList();
    redrawConsoleGui();
}

ConsoleGuiHandler::~ConsoleGuiHandler() {
    SetConsoleTextAttribute(h_console, saved_attributes);
    std::cin.clear();
    fflush(stdin);
    clearScreen();
}

void ConsoleGuiHandler::fillEmptyLines(std::vector<CONSOLE_LINE>& lines) {
    for (int i = 0; i < MAX_FILES + MAX_ADDITIONAL_LINES; i++)
        lines.push_back(NEW_LINE);
}

void ConsoleGuiHandler::clearScreen() {
    COORD tl = { 0,0 };
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(h_console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(h_console, ' ', cells, tl, &written);
    FillConsoleOutputAttribute(h_console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(h_console, tl);
}

void ConsoleGuiHandler::clearCurrentLine() {
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(h_console, &s);
    COORD current_line_coord = { 0, s.dwCursorPosition.Y };
    clearLine(current_line_coord);
}

void ConsoleGuiHandler::clearLine(COORD current_line_coord) {
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(h_console, &s);
    DWORD written, cells = s.dwSize.X;
    FillConsoleOutputCharacter(h_console, ' ', cells, current_line_coord, &written);
    FillConsoleOutputAttribute(h_console, saved_attributes, cells, current_line_coord, &written);
}

void ConsoleGuiHandler::outputLine(const CONSOLE_LINE& line) {
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(h_console, &s);
    COORD current_cursor_coord = s.dwCursorPosition;
    COORD next_line_coord = { 0, (short)(s.dwCursorPosition.Y + 1) };
    DWORD written;
    WriteConsoleA(h_console, line.text.c_str(), line.text.size(), &written, nullptr);
    FillConsoleOutputAttribute(h_console, line.attr, line.text.size(), current_cursor_coord, &written);
    SetConsoleCursorPosition(h_console, next_line_coord);
}

void ConsoleGuiHandler::outputLine(std::string str) {
    CONSOLE_LINE line;
    line.text = std::move(str);
    line.attr = saved_attributes;
    outputLine(line);
}

void ConsoleGuiHandler::addDirLine() {
    CONSOLE_LINE dir_line;
    dir_line.attr = saved_attributes;
    dir_line.text.append(DIR_PREFIX).append(Filedirectory::getCurrentDirectory());
    next_lines.push_back(dir_line);
    next_lines.push_back(NEW_LINE);
}

void ConsoleGuiHandler::addTopBottomLine(bool expression) {
    CONSOLE_LINE line;
    if (expression) {
        int amount_of_pages = ((int) list_files.size() / MAX_FILES) + ((list_files.size() % MAX_FILES) != 0);
        int current_page = starting_index / MAX_FILES + 1;
        line.attr = TOO_MANY_ATTR;
        line.text.append(NOT_SELECTED_PREFIX).append(TOO_MANY_PREFIX)
                .append(" (").append(std::to_string(current_page))
                .append("/").append(std::to_string(amount_of_pages))
                .append(")");
    } else {
        line.attr = saved_attributes;
        line.text.append(NOT_SELECTED_PREFIX);
    }
    next_lines.push_back(line);
}

void ConsoleGuiHandler::addSelected(int index) {
    CONSOLE_LINE selected_line;
    selected_line.attr = SELECTED_ATTR;
    selected_line.text.append(SELECTED_PREFIX);
    appendFileInfo(selected_line, index);
    next_lines.push_back(selected_line);
}

void ConsoleGuiHandler::addNotSelected(int index) {
    CONSOLE_LINE not_selected_line;
    switch(list_files[index].type) {
        case FileType::DIR:
            not_selected_line.attr = DIR_ATTR;
            break;
        case FileType::EXE:
            not_selected_line.attr = EXE_ATTR;
            break;
        case FileType::ORD:
            not_selected_line.attr = saved_attributes;
            break;
    }
    not_selected_line.text.append(NOT_SELECTED_PREFIX);
    appendFileInfo(not_selected_line, index);
    next_lines.push_back(not_selected_line);
}

inline void ConsoleGuiHandler::appendFileInfo(CONSOLE_LINE& line, int index) {
    file f = list_files[index];
    line.text.append(f.time).append(TAB_PREFIX)
            .append(f.size).append(TAB_PREFIX).append(f.name_beautified);
}

void ConsoleGuiHandler::addFilesLines() {
    for (int i = starting_index; i < MAX_FILES + starting_index &&
                                 i < list_files.size(); i++) {
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

inline bool ConsoleGuiHandler::areLinesDifferent(const CONSOLE_LINE& line1, const CONSOLE_LINE& line2) {
    return line1.attr != line2.attr || line1.text != line2.text;
}

void ConsoleGuiHandler::reduceCurrentLines() {
    COORD line_coord;
    for (int i = next_lines.size(); i < current_lines.size(); i++) {
        line_coord.X = 0;
        line_coord.Y = i;
        clearLine(line_coord);
        current_lines[i] = NEW_LINE;
    }
    line_coord.X = 0;
    line_coord.Y = next_lines.size();
    SetConsoleCursorPosition(h_console, line_coord);
}

void ConsoleGuiHandler::drawDifferentGui() {
    for (int i = 0; i < next_lines.size(); i++) {
        if (areLinesDifferent(current_lines[i], next_lines[i])) {
            COORD current_line_coord = { 0, (short) i };
            SetConsoleCursorPosition(h_console, current_line_coord);
            clearCurrentLine();
            outputLine(next_lines[i]);
            current_lines[i] = next_lines[i];
        }
    }

    if (next_lines.size() < current_lines.size()) {
        reduceCurrentLines();
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
    std::string file_name = list_files[current_selected_index].name_orig;
    switch(list_files[current_selected_index].type) {
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
    Exception ex = fd.reInit(file_name);
    switch(ex) {
        case Exception::NO_EXCEPTION:
            list_files = fd.getFilesList();
            current_selected_index = 0;
            starting_index = 0;
            redrawConsoleGui();
            break;
        case Exception::ACCESS_DENIED:
            outputLine("Access is denied!");
            break;
        case Exception::FILE_NOT_FOUND:
            outputLine("File not found!");
            break;
        case Exception::UNHANDLED:
            outputLine("An error has occurred!");
            break;
    }
}

void ConsoleGuiHandler::goUp() {
    reInit("..");
}