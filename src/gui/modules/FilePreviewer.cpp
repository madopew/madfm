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
    std::string output;
    for(int i = 0; i < rows - 1; i++) {
        for(int j = 0; j < columns; j++) {
            if(j == 0 && !skip_start) {
                output.push_back('~');
                output.push_back(' ');
                j++;
                continue;
            }
            c = fgetc(f);
            if(c == '\n' || c == EOF) {
                skip_start = 0;
                if(i != rows - 2)
                    output.push_back('\n');
                break;
            } else if(c == '\t') {
                for(int k = 0; k < 4; k++) {
                    output.push_back(' ');
                }
                j+=3;
            } else {
                output.push_back(c);
                if(j == columns - 1)
                    skip_start = 1;
            }
        }
    }
    if(c != EOF)
        output.append(PRE_NOTEND);

    cgh->utils.outputLineNoNew(output, cgh->saved_attributes);
    waitForClose();
    fclose(f);
    cgh->cleanRedrawConsoleGui();
}

void FilePreviewer::showRawPreview() {
    FILE *f = fopen(cgh->list_files[cgh->current_selected_index].getName().c_str(), "rb");
    if(!f) {
        return;
    }
    if(PRE_NOTEND.size() > columns) {
        return;
    }

    cgh->utils.clearScreen();
    std::string output;
    std::string ascii_line = " ";
    output.append(HEX_OFFSET);
    output.append("\n\n");

    int c;
    int off = 0;
    char off_str[9] = {0};
    char curr_byte[4] = {0};
    for(int i = 0; i < rows - 4; i++) {
        for(int j = 0; j < 16; j++) {
            c = fgetc(f);
            if(c == EOF) {
                goto end;
            } else {
                if(j == 0) {
                    sprintf(off_str, "  %04X: ", off);
                    off += 16;
                    output.append(off_str);
                }
                sprintf(curr_byte, " %02X", c);
                output.append(curr_byte);
            }
            if(c > 32 && c < 127)
                ascii_line.push_back(c);
            else
                ascii_line.push_back('.');
        }
        output.append(ascii_line);
        ascii_line = " ";
        output.push_back('\n');
    }
    if(c != EOF)
        output.append(PRE_NOTEND);
end:
    cgh->utils.outputLineNoNew(output, cgh->saved_attributes);
    waitForClose();
    fclose(f);
    cgh->cleanRedrawConsoleGui();
}

