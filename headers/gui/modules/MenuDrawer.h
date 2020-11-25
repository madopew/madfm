//
// Created by Madi on 25.11.2020.
//

#include "../ConsoleGuiHandler.h"

#ifndef MADFM_MENUDRAWER_H
#define MADFM_MENUDRAWER_H

class MenuDrawer {
    friend class ConsoleGuiHandler;
private:
    ConsoleGuiHandler *cgh;

    enum Prefixes {NOT_SELECTED_PREFIX, TAB_PREFIX, SELECTED_PREFIX, DIR_PREFIX, TOO_MANY_PREFIX};
    inline static const std::string CONST_PREFIXES[] = {"   ", "    ", " > ", "DIR: ", "..."};
    const WORD SELECTED_ATTR = (BACKGROUND_BLUE | BACKGROUND_INTENSITY);
    const WORD DIR_ATTR = (FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    const WORD EXE_ATTR = (FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    const WORD TOO_MANY_ATTR = (FOREGROUND_RED | FOREGROUND_INTENSITY);
    const int MAX_FILES = 15;
    const int MAX_ADDITIONAL_LINES = 7;
    std::vector <ConsoleLine> current_lines;
    std::vector <ConsoleLine> next_lines;
    void addDirLine();
    void addTopBottomLine(bool);
    void addFilesLines(void);
    void addSelected(int);
    void addNotSelected(int);
    void drawDifferentGui(void);
    void cleanRedrawConsoleGui(void);
    void reduceCurrentLines(void);
    void appendFileInfo(ConsoleLine&, int);
    void reserveLines(void);
    void redrawConsoleGui(void);
    MenuDrawer(ConsoleGuiHandler*);
};
#endif //MADFM_MENUDRAWER_H
