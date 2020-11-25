//
// Created by Madi on 25.11.2020.
//

#include <conio.h>
#include "../../../headers/gui/modules/FilePreviewer.h"

FilePreviewer::FilePreviewer(ConsoleGuiHandler *cgh) : cgh(cgh) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(cgh->h_console, &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void FilePreviewer::waitForClose() {
    int c;
eloop:
    c = getch();
    c = toupper(c);
    if(c != 'Q') goto eloop;
}

void FilePreviewer::showTextPreview() {
    FILE *f = fopen(cgh->list_files[cgh->current_selected_index].getName().c_str(), "r");
    if(!f) {
        return;
    }

    cgh->utils.clearScreen();
    int c;
    int skip_start = 0;
    for(int i = 0; i < rows - 1; i++) {
        for(int j = 0; j < columns; j++) {
            if(j == 0 && !skip_start) {
                cgh->utils.outputChar('~', cgh->saved_attributes);
                cgh->utils.outputChar(' ', cgh->saved_attributes);
                j++;
                continue;
            }
            c = fgetc(f);
            if(c == '\n' || c == EOF) {
                skip_start = 0;
                if(i != rows - 2)
                    cgh->utils.outputChar('\n', cgh->saved_attributes);
                break;
            } else if(c == '\t') {
                for(int k = 0; k < 4; k++) {
                    cgh->utils.outputChar(' ', cgh->saved_attributes);
                }
                j+=3;
            } else {
                cgh->utils.outputChar(c, cgh->saved_attributes);
                if(j == columns - 1)
                    skip_start = 1;
            }
        }
    }
    if(c != EOF)
        cgh->utils.outputLine(PRE_NOTEND, cgh->saved_attributes);

    waitForClose();
    fclose(f);
    cgh->cleanRedrawConsoleGui();
}

void FilePreviewer::showRawPreview() {

}

