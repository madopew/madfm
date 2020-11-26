//
// Created by Madi on 25.11.2020.
//
#include "../../../headers/gui/modules/FileRenamer.h"

FileRenamer::FileRenamer(ConsoleGuiHandler *cgh) : cgh(cgh) {}

void FileRenamer::rename() {
    cgh->redrawConsoleGui();
    cgh->utils.outputLine(cgh->NAME_QUESTION, cgh->saved_attributes);
    std::string new_name = cgh->utils.inputLine(cgh->saved_attributes);
    cgh->redrawConsoleGui();
    if(!cgh->checkFile(new_name)) return;
    rename(cgh->list_files[cgh->current_selected_index].getName(), new_name);
}

void FileRenamer::rename(const std::string &old_name, const std::string &new_name) {
    FiledirectoryException e = Filedirectory::changeName(old_name, new_name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        cgh->redrawConsoleGui();
        cgh->reInit(".");
        cgh->utils.outputLine(SUCCESS_RENAME, cgh->saved_attributes);
    } else {
        cgh->outputCorrespondingException(e);
    }
}
