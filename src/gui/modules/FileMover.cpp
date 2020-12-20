#include "../../../headers/gui/modules/FileMover.h"

FileMover::FileMover(ConsoleGuiHandler *cgh): cgh(cgh), saved_location(""), saved_name("") {

}

void FileMover::saveLocation() {
    saved_name = cgh->list_files[cgh->current_selected_index].getName();
    saved_location = cgh->fd.getCurrentDirectory() + "/" + saved_name;
    cgh->redrawConsoleGui();
    cgh->utils.outputLine(MSG_LOC_SAVED, cgh->saved_attributes);
}

void FileMover::moveOrCopyHandle(FiledirectoryException (*func)(const std::string&, const std::string&), const std::string &success_msg) {
    if(saved_location.empty()) {
        return;
    }
    std::string old_p = saved_location;
    std::string new_p = cgh->fd.getCurrentDirectory() + "/" + saved_name;
    if(!cgh->checkFile(new_p)) return;
    saved_location.clear();
    saved_name.clear();
    FiledirectoryException e = func(old_p, new_p);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        cgh->redrawConsoleGui();
        cgh->reInit(".");
        cgh->utils.outputLine(success_msg, cgh->saved_attributes);
    } else {
        cgh->outputCorrespondingException(e);
    }
}

void FileMover::moveFile() {
    moveOrCopyHandle(Filedirectory::move, SUCCESS_MOVE);
}

void FileMover::copyFile() {
    moveOrCopyHandle(Filedirectory::copy, SUCCESS_COPY);
}

