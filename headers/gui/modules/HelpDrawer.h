//
// Created by Madi on 25.11.2020.
//

#ifndef MADFM_HELPDRAWER_H
#define MADFM_HELPDRAWER_H

#include "../ConsoleGuiHandler.h"
#include <string>

class HelpDrawer {
    friend class ConsoleGuiHandler;
private:
    ConsoleGuiHandler *cgh;
    inline static const std::string HELP_LINES[] = {
            "    Key                                    Function",
            "=============================================================",
            "                        NAVIGATION",
            "j k ; UP DOWN                          up/down the list",
            "h   ; LEFT                             previous directory",
            "l   ; RIGHT                            next directory (open)",
            "=============================================================",
            "",
            "=============================================================",
            "                        FILES/DIRS",
            "   o/O                                 open file by default",
            "   r/R                                 rename file/dir",
            "   d/D                                 delete file/dir",
            "   n/N                                 create file/dir",
            "   v                                   show text preview",
            "(shift)V                               show hexdump",
            "=============================================================",
            "",
            "=============================================================",
            "                          OTHERS",
            "   F2                                  show this window",
            "   q/Q                                 close this/program",
            "============================================================="
    };
    HelpDrawer(ConsoleGuiHandler*);
    void showHelp(void);
};

#endif //MADFM_HELPDRAWER_H
