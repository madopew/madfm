//
// Created by Madi on 25.11.2020.
//

#ifndef MADFM_FILEPREVIEWER_H
#define MADFM_FILEPREVIEWER_H

#include <string>
#include "../ConsoleGuiHandler.h"

class FilePreviewer {
    friend class ConsoleGuiHandler;
private:
    ConsoleGuiHandler *cgh;

    int rows;
    int columns;

    inline static const std::string PRE_NOTEND = "\n-->Preview doesn't contain the whole file! Consider opening it in 'o' mode<--";
    void showTextPreview(void);
    void showRawPreview(void);

    void waitForClose(void);

    FilePreviewer(ConsoleGuiHandler*);
};

#endif //MADFM_FILEPREVIEWER_H
