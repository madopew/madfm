//
// Created by Madi on 25.11.2020.
//
#include <conio.h>
#include "../../../headers/gui/modules/FileDeleter.h"

FileDeleter::FileDeleter(ConsoleGuiHandler *cgh): cgh(cgh) {}

void FileDeleter::deleteFile() {
    cgh->utils.outputLine(DEL_QUESTION, cgh->saved_attributes);
    int c;
    c = getch();
    if(c == 'y' || c == 'Y') {
        deleteFile(cgh->list_files[cgh->current_selected_index].getName());
    } else {
        cgh->redrawConsoleGui();
    }
}

void FileDeleter::deleteFile(const std::string &name) {
    FiledirectoryException e = Filedirectory::deleteFile(name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        cgh->redrawConsoleGui();
        cgh->reInit(".");
        cgh->utils.outputLine(SUCCESS_DELETE, cgh->saved_attributes);
    } else {
        cgh->outputCorrespondingException(e);
    }
}
