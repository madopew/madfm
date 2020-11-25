//
// Created by Madi on 25.11.2020.
//

#ifndef MADFM_FILECREATOR_H
#define MADFM_FILECREATOR_H

#include <string>
#include "../ConsoleGuiHandler.h"

class FileCreator {
    friend class ConsoleGuiHandler;
private:
    ConsoleGuiHandler *cgh;

    inline static const std::string FD_QUESTION = "Is it a (f)ile or a (d)irectory?";
    inline static const std::string SUCCESS_CREATE_DIR = "Directory successfully created.";
    inline static const std::string SUCCESS_CREATE_FILE = "File successfully created.";
    void createFile(const std::string&);
    void createDir(const std::string&);
    void createFileOrDir(void);

    FileCreator(ConsoleGuiHandler*);
};

#endif //MADFM_FILECREATOR_H
