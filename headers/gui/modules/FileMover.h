//
// Created by Madi on 26.11.2020.
//

#ifndef MADFM_FILEMOVER_H
#define MADFM_FILEMOVER_H

#include "../ConsoleGuiHandler.h"

class FileMover {
    friend class ConsoleGuiHandler;
private:
    ConsoleGuiHandler *cgh;
    std::string saved_name;
    std::string saved_location;

    inline static const std::string MSG_LOC_SAVED = "File location saved.";
    inline static const std::string SUCCESS_MOVE = "File moved successfully.";
    inline static const std::string SUCCESS_COPY = "File copied successfully.";
    void moveOrCopyHandle( FiledirectoryException (*)(const std::string&, const std::string&), const std::string&);

    void saveLocation(void);
    void moveFile(void);
    void copyFile(void);

    FileMover(ConsoleGuiHandler *);
};

#endif //MADFM_FILEMOVER_H
