#ifndef MADFM_CONSOLEGUIHANDLER_H
#define MADFM_CONSOLEGUIHANDLER_H

#include <windows.h>
#include "Filedirectory.h"
#include "ConsoleLine.h"
#include "ConsoleGuiUtils.h"
#include <vector>
#include <string>

class ConsoleGuiHandler {
private:
    enum Prefixes {NOT_SELECTED_PREFIX, TAB_PREFIX, SELECTED_PREFIX, DIR_PREFIX, TOO_MANY_PREFIX};
    inline static const std::string CONST_PREFIXES[] = {"   ", "    ", " > ", "DIR: ", "..."};
    const WORD SELECTED_ATTR = (BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    const WORD DIR_ATTR = (FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    const WORD EXE_ATTR = (FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    const WORD TOO_MANY_ATTR = (FOREGROUND_RED | FOREGROUND_INTENSITY);
    const int MAX_FILES = 15;
    const int MAX_ADDITIONAL_LINES = 5;
    const UINT RUSSIAN_CP = 1251;

    ConsoleGuiUtils utils;
    std::vector <ConsoleLine> current_lines;
    std::vector <ConsoleLine> next_lines;
    int current_selected_index = 0;
    int starting_index = 0;
    HANDLE h_console;
    WORD saved_attributes;
    Filedirectory fd;
    std::vector<File> list_files;
    void addDirLine();
    void addTopBottomLine(bool);
    void addFilesLines(void);
    void addSelected(int);
    void addNotSelected(int);
    void reInit(std::string);
    void drawDifferentGui(void);
    void reduceCurrentLines(void);
    void appendFileInfo(ConsoleLine&, int);
    void saveAttributes(void);
    void reserveLines(void);
public:
    ConsoleGuiHandler(HANDLE);
    ~ConsoleGuiHandler(void);
    void redrawConsoleGui(void);
    void moveDown(void);
    void moveUp(void);
    void open(void);
    void openDir(void);
    void goUp(void);
    void rename(void);
};
#endif //MADFM_CONSOLEGUIHANDLER_H
