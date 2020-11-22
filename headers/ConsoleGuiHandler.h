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
    const int MAX_ADDITIONAL_LINES = 7;
    const UINT RUSSIAN_CP = 1251;
    inline static const std::string FD_EXISTS = "File or directory with such name already exists!";
    inline static const std::string SUCCESS_RENAME = "File or directory successfully renamed.";
    inline static const std::string SUCCESS_CREATE_DIR = "Directory successfully created.";
    inline static const std::string SUCCESS_CREATE_FILE = "File successfully created.";
    inline static const std::string SUCCESS_DELETE = "Successfully deleted.";
    inline static const std::string FD_QUESTION = "Is it a (f)ile or a (d)irectory?";
    inline static const std::string NAME_QUESTION = "Input name:";
    inline static const std::string DEL_QUESTION = "Are you sure you want to delete this? (y/N)";

    inline static const std::string HELP_LINES[] = {
            "    Key                                    Function",
            "=============================================================",
            "                        NAVIGATION",
            "j k ; UP DOWN                          up/down the list",
            "h   ; LEFT                             previous directory",
            "l   ; RIGHT                            next directory (open)",
            "",
            "=============================================================",
            "",
            "=============================================================",
            "                        FILES/DIRS",
            "   o/O                                 open file by default",
            "   r/R                                 rename file/dir",
            "   d/D                                 delete file/dir",
            "   n/N                                 create file/dir",
            "=============================================================",
            "",
            "=============================================================",
            "                          OTHERS",
            "    H                                  show this window",
            "   q/Q                                 close this/program",
            "============================================================="
    };

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
    void reInitSafe(std::string);
    void drawDifferentGui(void);
    void reduceCurrentLines(void);
    void appendFileInfo(ConsoleLine&, int);
    void saveAttributes(void);
    void reserveLines(void);
    void rename(const std::string&, const std::string&);
    void createFile(const std::string&);
    void createDir(const std::string&);
    void outputCorrespondingException(FiledirectoryException);
    bool checkFile(const std::string&);
    void deleteFile(const std::string&);
public:
    ConsoleGuiHandler(HANDLE);
    ~ConsoleGuiHandler(void);
    void redrawConsoleGui(void);
    void moveDown(void);
    void moveUp(void);
    void open(void);
    void openDir(void);
    void createFileOrDir(void);
    void goUp(void);
    void rename(void);
    void deleteFile(void);
    void showHelp(void);
};
#endif //MADFM_CONSOLEGUIHANDLER_H
