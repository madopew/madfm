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
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    int rows;
    int columns;

    inline static const std::string PRE_NOTEND = "\n-->Preview doesn't contain the whole file! Consider opening it in 'o' mode<--";
    inline static const std::string HEX_OFFSET = "Offset:  00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ------TEXT------";
    void showTextPreview(void);
    void showRawPreview(void);

    void waitForClose(void);

    FilePreviewer(ConsoleGuiHandler*);
};

#endif //MADFM_FILEPREVIEWER_H
