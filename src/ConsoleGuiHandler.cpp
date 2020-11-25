#include "../headers/ConsoleGuiHandler.h"
#include <iostream>
#include <utility>
#include <conio.h>

#include <cstdio>

ConsoleGuiHandler::ConsoleGuiHandler(HANDLE h_console) : utils(h_console), h_console(h_console), fd(".") {
    saveAttributes();
    SetConsoleCP(RUSSIAN_CP);
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

void ConsoleGuiHandler::cleanRedrawConsoleGui() {
    utils.clearScreen();
    current_lines.clear();
    reserveLines();
    redrawConsoleGui();
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
    int relative_index = current_selected_index + 1 - starting_index;
    if (relative_index < MAX_FILES && current_selected_index < list_files.size() - 1) {
        current_selected_index++;
    } else if (relative_index == MAX_FILES && current_selected_index < list_files.size() - 1) {
        current_selected_index++;
        starting_index += MAX_FILES;
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
        starting_index -= MAX_FILES;
    }
    else if (relative_index == 1 && starting_index == 0) {
        current_selected_index = list_files.size() - 1;
        starting_index = current_selected_index / MAX_FILES * MAX_FILES;
    }
    redrawConsoleGui();
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

void ConsoleGuiHandler::reInit(std::string file_name) {
    file_name = Filedirectory::getCurrentDirectory() + "/" + file_name;
    FiledirectoryException e = fd.reInit(file_name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        list_files = fd.getFilesList();
        if(current_selected_index >= list_files.size()) {
            current_selected_index = list_files.size() - 1;
        }
        if(starting_index >= list_files.size() && list_files.size() != 0) {
            starting_index -= MAX_FILES;
        }
        redrawConsoleGui();
    } else {
        outputCorrespondingException(e);
    }
}

void ConsoleGuiHandler::reInitSafe(std::string file_name) {
    file_name = Filedirectory::getCurrentDirectory() + "/" + file_name;
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

void ConsoleGuiHandler::goUp() {
    reInitSafe("..");
}

bool ConsoleGuiHandler::checkFile(const std::string &name) {
    if(name.empty()) return false;
    if(Filedirectory::containsCurrent(name)) {
        redrawConsoleGui();
        utils.outputLine(FD_EXISTS, saved_attributes);
        return false;
    }
    return true;
}

void ConsoleGuiHandler::rename() {
    utils.outputLine(NAME_QUESTION, saved_attributes);
    std::string new_name = utils.inputLine(saved_attributes);
    redrawConsoleGui();
    if(!checkFile(new_name)) return;
    rename(list_files[current_selected_index].getName(), new_name);
}

void ConsoleGuiHandler::rename(const std::string &old_name, const std::string &new_name) {
    FiledirectoryException e = Filedirectory::changeName(old_name, new_name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        redrawConsoleGui();
        reInit(".");
        utils.outputLine(SUCCESS_RENAME, saved_attributes);
    } else {
        outputCorrespondingException(e);
    }
}

void ConsoleGuiHandler::createFileOrDir() {
    utils.outputLine(NAME_QUESTION, saved_attributes);
    std::string name = utils.inputLine(saved_attributes);
    redrawConsoleGui();
    if(!checkFile(name)) return;
    utils.outputLine(FD_QUESTION, saved_attributes);
    int c;
iloop:
    c = getch();
    switch(c) {
        case 'd':
        case 'D':
            createDir(name);
            break;
        case 'f':
        case 'F':
            createFile(name);
            break;
        case 27:
            redrawConsoleGui();
            break;
        default:
            goto iloop;
    }
}

void ConsoleGuiHandler::createDir(const std::string &name) {
    FiledirectoryException e = Filedirectory::createDir(name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        redrawConsoleGui();
        reInit(".");
        utils.outputLine(SUCCESS_CREATE_DIR, saved_attributes);
    } else {
        outputCorrespondingException(e);
    }
}

void ConsoleGuiHandler::createFile(const std::string &name) {
    FiledirectoryException e = Filedirectory::createFile(name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        redrawConsoleGui();
        reInit(".");
        utils.outputLine(SUCCESS_CREATE_FILE, saved_attributes);
    } else {
        outputCorrespondingException(e);
    }
}

void ConsoleGuiHandler::deleteFile() {
    utils.outputLine(DEL_QUESTION, saved_attributes);
    int c;
    c = _getch();
    if(c == 'y' || c == 'Y') {
        deleteFile(list_files[current_selected_index].getName());
    } else {
        redrawConsoleGui();
    }
}

void ConsoleGuiHandler::deleteFile(const std::string &name) {
    FiledirectoryException e = Filedirectory::deleteFile(name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        redrawConsoleGui();
        reInit(".");
        utils.outputLine(SUCCESS_DELETE, saved_attributes);
    } else {
        outputCorrespondingException(e);
    }
}

void ConsoleGuiHandler::showHelp() {
    utils.clearScreen();
    for(const auto& line : HELP_LINES) {
        utils.outputLine(line, saved_attributes);
    }
    int c;
    do {
        c = getch();
    } while (c != 'q' && c != 'Q');
    cleanRedrawConsoleGui();
}

void ConsoleGuiHandler::showPreview() {
    FILE *f = fopen(list_files[current_selected_index].getName().c_str(), "r");
    if(!f) {
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    GetConsoleScreenBufferInfo(h_console, &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    utils.outputLine("Show (r)aw or (t)ext view?", saved_attributes);
    int c;
iloop:
    c = getch();
    c = toupper(c);
    switch(c) {
        case 'T':
            utils.clearScreen();
            showTextPreview(rows, columns, f);
            break;
        case 'R':
            utils.clearScreen();
            showRawPreview(rows, columns, f);
            break;
        case 27:
            redrawConsoleGui();
            return;
        default: goto iloop;
    }

eloop:
    c = getch();
    c = toupper(c);
    if(c != 'Q') goto eloop;

    fclose(f);
    cleanRedrawConsoleGui();
}

void ConsoleGuiHandler::showTextPreview(int rows, int columns, FILE *f) {
    int c;
    int skip_start = 0;
    for(int i = 0; i < rows - 1; i++) {
        for(int j = 0; j < columns; j++) {
            if(j == 0 && !skip_start) {
                utils.outputChar('~', saved_attributes);
                utils.outputChar(' ', saved_attributes);
                j++;
                continue;
            }
            c = fgetc(f);
            if(c == '\n' || c == EOF) {
                skip_start = 0;
                if(i != rows - 2)
                    utils.outputChar('\n', saved_attributes);
                break;
            } else if(c == '\t') {
                for(int k = 0; k < 4; k++) {
                    utils.outputChar(' ', saved_attributes);
                }
                j+=3;
            } else {
                utils.outputChar(c, saved_attributes);
                if(j == columns - 1) {
                    skip_start = 1;
                }
            }
        }
    }
    if(c != EOF)
        utils.outputLine("\n-->Preview doesn't contain the whole file! Consider opening it in 'o' mode<--", saved_attributes);
}

void ConsoleGuiHandler::showRawPreview(int rows, int columns, FILE *f) {

}