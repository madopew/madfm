#ifndef MADFM_CONSOLEGUIHANDLER_H
#define MADFM_CONSOLEGUIHANDLER_H

#include <Windows.h>
#include "../files/Filedirectory.h"
#include "ConsoleLine.h"
#include "ConsoleGuiUtils.h"
#include <vector>
#include <string>
#include "modules/MenuDrawer.h"
#include "modules/HelpDrawer.h"
#include "modules/FileCreator.h"
#include "modules/FileDeleter.h"
#include "modules/FileRenamer.h"
#include "modules/FilePreviewer.h"

class ConsoleGuiHandler {
    friend class MenuDrawer;
    friend class HelpDrawer;
    friend class FileCreator;
    friend class FileRenamer;
    friend class FileDeleter;
    friend class FilePreviewer;
private:
    MenuDrawer *menuDrawer;
    HelpDrawer *helpDrawer;
    FileCreator *fileCreator;
    FileRenamer *fileRenamer;
    FileDeleter *fileDeleter;
    FilePreviewer *filePreviewer;

    int current_selected_index = 0;
    int starting_index = 0;
    const UINT RUSSIAN_CP = 1251;
    ConsoleGuiUtils utils;
    HANDLE h_console;
    WORD saved_attributes;
    Filedirectory fd;
    std::vector<File> list_files;
    void cleanRedrawConsoleGui(void);
    void reInit(std::string);
    void reInitSave(std::string);
    void outputCorrespondingException(FiledirectoryException);
    bool checkFile(const std::string&);
    inline static const std::string FD_EXISTS = "File or directory with such name already exists!";
    inline static const std::string NAME_QUESTION = "Input name:";
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
    void showTextPreview(void);
    void showRawPreview(void);
};
#endif //MADFM_CONSOLEGUIHANDLER_H
