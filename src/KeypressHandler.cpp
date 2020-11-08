#include "../headers/KeypressHandler.h"

KeypressHandler::KeypressHandler(ConsoleGuiHandler &cgh) : cgh(cgh) {
    counter = 0;
    down_pressed = false;
    up_pressed = false;
    right_pressed = false;
    left_pressed = false;
    r_pressed = false;
}

inline bool KeypressHandler::callOnce(bool &exp, const short KEY, void (ConsoleGuiHandler:: *func_call)()) {
    bool is_pressed = GetAsyncKeyState(KEY) < 0;
    if (!exp && !is_pressed)
        return false;

    if (!exp && is_pressed) {
        (cgh.*func_call)();
        exp = true;
        return true;
    }

    if (exp && !is_pressed) {
        exp = false;
    }
    return false;
}

inline void KeypressHandler::fastForward(bool &exp, const short KEY, void (ConsoleGuiHandler:: *func_call)()) {
    bool is_pressed = GetAsyncKeyState(KEY) < 0;
    if (!exp && !is_pressed)
        return;

    if (!exp && is_pressed) {
        (cgh.*func_call)();
        exp = true;
    }

    if (exp && is_pressed) {
        counter++;
    }

    if (counter > THRESHOLD && is_pressed) {
        (cgh.*func_call)();
        counter -= THRESHOLD_DRAWBACK;
    }

    if (exp && !is_pressed) {
        exp = false;
        counter = 0;
    }
}

int KeypressHandler::start() {
    HWND w_console = GetConsoleWindow();
    while(true) {
        if (GetForegroundWindow() == w_console) {
            if (GetAsyncKeyState('Q') < 0)
                return 0;

            fastForward(down_pressed, VK_DOWN, &ConsoleGuiHandler::moveDown);
            fastForward(up_pressed, VK_UP, &ConsoleGuiHandler::moveUp);

            callOnce(right_pressed, VK_RIGHT, &ConsoleGuiHandler::open);
            callOnce(left_pressed, VK_LEFT, &ConsoleGuiHandler::goUp);

            callOnce(r_pressed, 'R', &ConsoleGuiHandler::rename);
        }
    }
}

