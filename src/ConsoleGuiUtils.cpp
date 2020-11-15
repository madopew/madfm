#include "../headers/ConsoleGuiUtils.h"
#include <conio.h>

ConsoleGuiUtils::ConsoleGuiUtils(HANDLE h_console) : h_console(h_console) {}

void ConsoleGuiUtils::clearScreen() {
    COORD tl = { 0,0 };
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(h_console, &s);
    DWORD written, cells = s.dwSize.X * s.dwSize.Y;
    FillConsoleOutputCharacter(h_console, ' ', cells, tl, &written);
    FillConsoleOutputAttribute(h_console, s.wAttributes, cells, tl, &written);
    SetConsoleCursorPosition(h_console, tl);
}

void ConsoleGuiUtils::clearCurrentLine() {
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(h_console, &s);
    COORD current_line_coord = { 0, s.dwCursorPosition.Y };
    clearLine(current_line_coord);
}

void ConsoleGuiUtils::clearLine(COORD current_line_coord) {
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(h_console, &s);
    DWORD written, cells = s.dwSize.X;
    FillConsoleOutputCharacter(h_console, ' ', cells, current_line_coord, &written);
    FillConsoleOutputAttribute(h_console, 0, cells, current_line_coord, &written);
}

void ConsoleGuiUtils::outputLine(ConsoleLine line) {
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(h_console, &s);
    COORD current_cursor_coord = s.dwCursorPosition;
    COORD next_line_coord = { 0, (short)(s.dwCursorPosition.Y + 1) };
    DWORD written;
    WriteConsoleA(h_console, line.getText().c_str(), line.getText().size(), &written, nullptr);
    FillConsoleOutputAttribute(h_console, line.getAttribute(), line.getText().size(), current_cursor_coord, &written);
    SetConsoleCursorPosition(h_console, next_line_coord);
}

void ConsoleGuiUtils::outputLine(const std::string& str, WORD attr) {
    outputLine(ConsoleLine(str, attr));
}

void ConsoleGuiUtils::outputChar(const char c, WORD attr) {
    CONSOLE_SCREEN_BUFFER_INFO s;
    GetConsoleScreenBufferInfo(h_console, &s);
    COORD current_cursor_coord = s.dwCursorPosition;
    DWORD written;
    WriteConsole(h_console, &c, 1, &written, nullptr);
    FillConsoleOutputAttribute(h_console, attr, 1, current_cursor_coord, &written);
}

void ConsoleGuiUtils::protectKeypress() {
    _getch();
    while(_kbhit()) _getch();
}

std::string ConsoleGuiUtils::inputLine(WORD attr) {
    protectKeypress();
    char buff[INPUT_MAX_LENGTH] = {0};
    int index = 0;
    int c;
iloop:
    c = _getch();
    switch(c) {
        case 27: //esc
            return "";
        case 0: //extended
            _getch();
            printf(" %s ", "extended");
            goto iloop;
        case '\n':
        case '\r':
            return buff;
        case '\b':
            outputChar(c, attr);
            outputChar(' ', attr);
            outputChar(c, attr);
            index -= index != 0;
            buff[index] = 0;
            goto iloop;
        default:
            if(index < INPUT_MAX_LENGTH) {
                outputChar(c, attr);
                buff[index] = c;
                index++;
            }
            goto iloop;
    }
}
