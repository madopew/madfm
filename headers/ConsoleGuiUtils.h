#include "ConsoleLine.h"
#include "ConsoleGuiHandler.h"
#include <vector>
#include <windows.h>

#ifndef MADFM_CONSOLEGUIUTILS_H
#define MADFM_CONSOLEGUIUTILS_H

class ConsoleGuiUtils {
private:
    HANDLE h_console;
public:
    ConsoleGuiUtils(HANDLE);
    void clearScreen(void);
    void clearCurrentLine(void);
    void clearLine(COORD);
    void outputLine(ConsoleLine);
    void outputLine(const std::string &, WORD);
};
#endif //MADFM_CONSOLEGUIUTILS_H
