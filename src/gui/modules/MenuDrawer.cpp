//
// Created by Madi on 25.11.2020.
//

#include "../../../headers/gui/modules/MenuDrawer.h"

MenuDrawer::MenuDrawer(ConsoleGuiHandler *cgh) : cgh(cgh) {
    reserveLines();
}

void MenuDrawer::reserveLines() {
    current_lines.reserve(MAX_FILES + MAX_ADDITIONAL_LINES);
    for (int i = 0; i < MAX_FILES + MAX_ADDITIONAL_LINES; i++)
        current_lines.emplace_back();
    next_lines.reserve(MAX_FILES + MAX_ADDITIONAL_LINES);
}

void MenuDrawer::redrawConsoleGui() {
    next_lines.clear();
    addDirLine();
    addTopBottomLine(cgh->starting_index);
    addFilesLines();
    addTopBottomLine(cgh->starting_index + MAX_FILES < cgh->list_files.size());
    drawDifferentGui();
}

void MenuDrawer::addDirLine() {
    std::string dir = std::string(CONST_PREFIXES[DIR_PREFIX]).append(Filedirectory::getCurrentDirectory());
    ConsoleLine dir_line(dir, cgh->saved_attributes);
    next_lines.push_back(dir_line);
    next_lines.emplace_back();
}

void MenuDrawer::addTopBottomLine(bool expression) {
    ConsoleLine line;
    if (expression) {
        int amount_of_pages = ((int) cgh->list_files.size() / MAX_FILES) + ((cgh->list_files.size() % MAX_FILES) != 0);
        int current_page = cgh->starting_index / MAX_FILES + 1;
        char buff[256] = {0};
        sprintf(buff, "%s%s (%d/%d)", CONST_PREFIXES[NOT_SELECTED_PREFIX].c_str(), CONST_PREFIXES[TOO_MANY_PREFIX].c_str(),
                current_page, amount_of_pages);
        line.setAttribute(TOO_MANY_ATTR);
        line.setText(buff);
    }
    next_lines.push_back(line);
}

void MenuDrawer::addSelected(int index) {
    ConsoleLine selected_line(CONST_PREFIXES[SELECTED_PREFIX], SELECTED_ATTR);
    appendFileInfo(selected_line, index);
    next_lines.push_back(selected_line);
}

void MenuDrawer::addNotSelected(int index) {
    ConsoleLine not_selected_line;
    switch(cgh->list_files[index].getType()) {
        case FileType::DIR:
            not_selected_line.setAttribute(DIR_ATTR);
            break;
        case FileType::EXE:
            not_selected_line.setAttribute(EXE_ATTR);
            break;
        case FileType::ORD:
            not_selected_line.setAttribute(cgh->saved_attributes);
            break;
    }
    not_selected_line.setText(CONST_PREFIXES[NOT_SELECTED_PREFIX]);
    appendFileInfo(not_selected_line, index);
    next_lines.push_back(not_selected_line);
}

inline void MenuDrawer::appendFileInfo(ConsoleLine& line, int index) {
    File f = cgh->list_files[index];
    std::string line_text = line.getText().append(f.getTime()).append(CONST_PREFIXES[TAB_PREFIX])
            .append(f.getSize()).append(CONST_PREFIXES[TAB_PREFIX]).append(f.getParsedName());
    line.setText(line_text);
}

void MenuDrawer::addFilesLines() {
    for (int i = cgh->starting_index; i < MAX_FILES + cgh->starting_index && i < cgh->list_files.size(); i++) {
        if (i == cgh->current_selected_index)
            addSelected(i);
        else
            addNotSelected(i);
    }
}

void MenuDrawer::cleanRedrawConsoleGui() {
    cgh->utils.clearScreen();
    current_lines.clear();
    reserveLines();
    redrawConsoleGui();
}

void MenuDrawer::drawDifferentGui() {
    for (int i = 0; i < next_lines.size(); i++) {
        if (current_lines[i] != next_lines[i]) {
            COORD current_line_coord = { 0, (short) i };
            SetConsoleCursorPosition(cgh->h_console, current_line_coord);
            cgh->utils.clearCurrentLine();
            cgh->utils.outputLine(next_lines[i]);
            current_lines[i] = next_lines[i];
        }
    }
    reduceCurrentLines();
}

void MenuDrawer::reduceCurrentLines() {
    if (next_lines.size() < current_lines.size()) {
        COORD line_coord;
        for (int i = next_lines.size(); i < current_lines.size(); i++) {
            line_coord.X = 0;
            line_coord.Y = i;
            cgh->utils.clearLine(line_coord);
            current_lines[i] = ConsoleLine();
        }
        line_coord.X = 0;
        line_coord.Y = next_lines.size();
        SetConsoleCursorPosition(cgh->h_console, line_coord);
    }
}