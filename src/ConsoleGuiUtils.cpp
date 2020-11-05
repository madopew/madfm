#include "../headers/ConsoleGuiUtils.h"

#include <utility>

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
