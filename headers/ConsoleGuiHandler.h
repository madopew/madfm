#ifndef MADFM_CONSOLEGUIHANDLER_H
#define MADFM_CONSOLEGUIHANDLER_H

#include <windows.h>
#include "Filedirectory.h"
#include <vector>
#include <string>

const char NOT_SELECTED_PREFIX[] = "   ";
const char TAB_PREFIX[] = "    ";
const char SELECTED_PREFIX[] =  " > ";
const char DIR_PREFIX[]  = "DIR: ";
const char TOO_MANY_PREFIX[] =  "...";
const WORD SELECTED_ATTR = (BACKGROUND_BLUE | BACKGROUND_INTENSITY);
const WORD DIR_ATTR = (FOREGROUND_BLUE | FOREGROUND_INTENSITY);
const WORD EXE_ATTR = (FOREGROUND_GREEN | FOREGROUND_INTENSITY);
const WORD TOO_MANY_ATTR = (FOREGROUND_RED | FOREGROUND_INTENSITY);
const int MAX_FILES = 15;
const int MAX_ADDITIONAL_LINES = 5;
const UINT RUSSIAN_CP = 1251;

typedef struct _CONSOLE_LINE {
    std::string text = "";
    WORD attr = 0;
} CONSOLE_LINE;

class ConsoleGuiHandler {
private:
    const CONSOLE_LINE NEW_LINE = { "", 0 };
    std::vector <CONSOLE_LINE> current_lines;
    std::vector <CONSOLE_LINE> next_lines;
    int current_selected_index = 0;
    int starting_index = 0;
    HANDLE h_console;
    WORD saved_attributes;
    Filedirectory fd;
    std::vector<file> list_files;
    void addDirLine();
    void addSelected(int);
    void addNotSelected(int);
    void reInit(std::string);
    void drawDifferentGui(void);
    void addTopBottomLine(bool);
    void addFilesLines(void);
    static inline bool areLinesDifferent(const CONSOLE_LINE&, const CONSOLE_LINE&);
    void fillEmptyLines(std::vector<CONSOLE_LINE>&);
    void reduceCurrentLines(void);
    inline void appendFileInfo(CONSOLE_LINE&, int);
    void clearScreen(void);
    void clearCurrentLine(void);
    void clearLine(COORD);
    void outputLine(const CONSOLE_LINE&);
    void outputLine(std::string);
public:
    ConsoleGuiHandler(HANDLE);
    ~ConsoleGuiHandler(void);
    void redrawConsoleGui(void);
    void moveDown(void);
    void moveUp(void);
    void open(void);
    void goUp(void);
    void rename(void);
};
#endif //MADFM_CONSOLEGUIHANDLER_H
