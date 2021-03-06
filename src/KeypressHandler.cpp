#include "../headers/KeypressHandler.h"
#include <conio.h>

KeypressHandler::KeypressHandler(ConsoleGuiHandler *cgh) : cgh(cgh) {
}

int KeypressHandler::start() {
    int c;
    while(kbhit()) getch();

mloop:
    c = getch();
    switch(c) {
        case 0: //func keys
        case 224:
            c = getch();
            switch(c) {
                case 72:  //up arrow
                    cgh->moveUp();
                    break;
                case 80:  //down arrow
                    cgh->moveDown();
                    break;
                case 75:  //left arrow
                    cgh->goUp();
                    break;
                case 77:  //right arrow
                    cgh->openDir();
                    break;
                case 60:  //f2
                    cgh->showHelp();
                    break;
            }
            goto mloop;
        case 'h':
            cgh->goUp();
            goto mloop;
        case 'j':
            cgh->moveDown();
            goto mloop;
        case 'k':
            cgh->moveUp();
            goto mloop;
        case 'l':
            cgh->openDir();
            goto mloop;
        case 'v':
            cgh->showTextPreview();
            goto mloop;
        case 'V':
            cgh->showRawPreview();
            goto mloop;
        case 'q':
        case 'Q':
            return 0;
        case 'o':
        case 'O':
            cgh->open();
            goto mloop;
        case 'r':
        case 'R':
            cgh->rename();
            goto mloop;
        case 'n':
        case 'N':
            cgh->createFileOrDir();
            goto mloop;
        case 'd':
        case 'D':
            cgh->deleteFile();
            goto mloop;
        case 'c':
        case 'C':
            cgh->saveLocation();
            goto mloop;
        case 'm':
        case 'M':
            cgh->moveFile();
            goto mloop;
        case 'p':
        case 'P':
            cgh->copyFile();
            goto mloop;
        default:
            goto mloop;
    }
}

