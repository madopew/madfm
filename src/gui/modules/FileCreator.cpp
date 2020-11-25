//
// Created by Madi on 25.11.2020.
//

#include <conio.h>
#include "../../../headers/gui/modules/FileCreator.h"

FileCreator::FileCreator(ConsoleGuiHandler *cgh) : cgh(cgh) {}

void FileCreator::createFileOrDir() {
    cgh->utils.outputLine(ConsoleGuiHandler::NAME_QUESTION, cgh->saved_attributes);
    std::string name = cgh->utils.inputLine(cgh->saved_attributes);
    cgh->redrawConsoleGui();
    if(!cgh->checkFile(name)) return;
    cgh->utils.outputLine(FD_QUESTION, cgh->saved_attributes);
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
            cgh->redrawConsoleGui();
            break;
        default:
            goto iloop;
    }
}

void FileCreator::createDir(const std::string &name) {
    FiledirectoryException e = Filedirectory::createDir(name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        cgh->redrawConsoleGui();
        cgh->reInit(".");
        cgh->utils.outputLine(SUCCESS_CREATE_DIR, cgh->saved_attributes);
    } else {
        cgh->outputCorrespondingException(e);
    }
}

void FileCreator::createFile(const std::string &name) {
    FiledirectoryException e = Filedirectory::createFile(name);
    if(e == FiledirectoryException::NO_EXCEPTION) {
        cgh->redrawConsoleGui();
        cgh->reInit(".");
        cgh->utils.outputLine(SUCCESS_CREATE_FILE, cgh->saved_attributes);
    } else {
        cgh->outputCorrespondingException(e);
    }
}



