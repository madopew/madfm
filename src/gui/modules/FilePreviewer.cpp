//
// Created by Madi on 25.11.2020.
//

#include <conio.h>
#include "../../../headers/gui/modules/FilePreviewer.h"

FilePreviewer::FilePreviewer(ConsoleGuiHandler *cgh) : cgh(cgh) {
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
    FILE *f = fopen(cgh->list_files[cgh->current_selected_index].getName().c_str(), "rb");
    if(!f) {
        return;
    }
    if(HEX_OFFSET.size() * 1.2 > columns) {
        return;
    }

    cgh->utils.clearScreen();
    cgh->utils.outputLine(HEX_OFFSET, cgh->saved_attributes);
    cgh->utils.outputChar('\n', cgh->saved_attributes);

    int c;
    int off = 0;
    char off_str[9] = {0};
    char curr_byte[4] = {0};
    COORD saved_pos = {8, 2};
    for(int i = 0; i < rows - 4; i++) {
        for(int j = 0; j < 16; j++) {
            if(j == 0) {
                sprintf(off_str, "  %04X: ", off);
                off += 16;
                cgh->utils.outputLineNoNew(off_str, cgh->saved_attributes);
            }
            c = fgetc(f);
            if(c == EOF) {
                goto end;
            } else {
                sprintf(curr_byte, " %02X", c);
                cgh->utils.outputLineNoNew(curr_byte, cgh->saved_attributes);
                saved_pos.X += 3;
                int value_offset =(15 - j) * 3 + 1 + j;
                COORD value_pos = {(short)(saved_pos.X + value_offset), saved_pos.Y};
                SetConsoleCursorPosition(cgh->h_console, value_pos);
                if(c > 32 && c < 127)
                    cgh->utils.outputChar(c, cgh->saved_attributes);
                else
                    cgh->utils.outputChar('.', cgh->saved_attributes);
                SetConsoleCursorPosition(cgh->h_console, saved_pos);
            }
        }
        saved_pos.Y += 1;
        saved_pos.X = 8;
        cgh->utils.outputChar('\n', cgh->saved_attributes);
    }
    if(c != EOF)
        cgh->utils.outputLine(PRE_NOTEND, cgh->saved_attributes);
end:
    waitForClose();
    fclose(f);
    cgh->cleanRedrawConsoleGui();
}

