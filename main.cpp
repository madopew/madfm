#include <Windows.h>
#include "./headers/ConsoleGuiHandler.h"
#include "./headers/KeypressHandler.h"

int main() {
    Sleep(100);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleGuiHandler cgh(hConsole);
    KeypressHandler kh(cgh);
    return kh.start();
}
