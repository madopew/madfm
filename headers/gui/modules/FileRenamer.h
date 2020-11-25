//
// Created by Madi on 25.11.2020.
//

#ifndef MADFM_FILERENAMER_H
#define MADFM_FILERENAMER_H

#include "../ConsoleGuiHandler.h"

#include <string>

class FileRenamer {
    friend class ConsoleGuiHandler;
private:
    ConsoleGuiHandler *cgh;

    inline static const std::string SUCCESS_RENAME = "File or directory successfully renamed.";
    void rename(const std::string&, const std::string&);
    void rename(void);

    FileRenamer(ConsoleGuiHandler*);
};
#endif //MADFM_FILERENAMER_H
