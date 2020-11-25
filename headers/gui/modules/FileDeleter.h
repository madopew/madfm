//
// Created by Madi on 25.11.2020.
//

#ifndef MADFM_FILEDELETER_H
#define MADFM_FILEDELETER_H

#include "../ConsoleGuiHandler.h"
#include <string>

class FileDeleter {
    friend class ConsoleGuiHandler;
private:
    ConsoleGuiHandler *cgh;

    inline static const std::string SUCCESS_DELETE = "File or directory successfully deleted.";
    inline static const std::string DEL_QUESTION = "Are you sure you want to delete this? (y/N)";
    void deleteFile(const std::string&);
    void deleteFile(void);

    FileDeleter(ConsoleGuiHandler*);
};
#endif //MADFM_FILEDELETER_H
