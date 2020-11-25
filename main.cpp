#include <Windows.h>
#include "headers/gui/ConsoleGuiHandler.h"
#include "./headers/KeypressHandler.h"

int main() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleGuiHandler cgh(hConsole);
    KeypressHandler kh(&cgh);
    return kh.start();
}
