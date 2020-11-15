#include "ConsoleLine.h"
#include "ConsoleGuiHandler.h"
#include <vector>
#include <windows.h>

#ifndef MADFM_CONSOLEGUIUTILS_H
#define MADFM_CONSOLEGUIUTILS_H

#define INPUT_MAX_LENGTH 64

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
    void outputChar(const char, WORD);
    static void protectKeypress(void);
    std::string inputLine(WORD);
};
#endif //MADFM_CONSOLEGUIUTILS_H
