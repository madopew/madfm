#include <conio.h>
#include "../../../headers/gui/modules/HelpDrawer.h"

HelpDrawer::HelpDrawer(ConsoleGuiHandler *cgh):cgh(cgh) {}

void HelpDrawer::showHelp() {
    cgh->utils.clearScreen();
    for(const auto& line : HELP_LINES) {
        cgh->utils.outputLine(line, cgh->saved_attributes);
    }
    int c;
    do {
        c = getch();
    } while (c != 'q' && c != 'Q');
    cgh->cleanRedrawConsoleGui();
}

