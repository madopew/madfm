#ifndef MADFM_KEYPRESSHANDLER_H
#define MADFM_KEYPRESSHANDLER_H

#include "ConsoleGuiHandler.h"

const int THRESHOLD = 75000;
const int THRESHOLD_DRAWBACK = (THRESHOLD / 10);

class KeypressHandler {
private:
    ConsoleGuiHandler cgh;
    int counter;
    bool down_pressed;
    bool up_pressed;
    bool right_pressed;
    bool left_pressed;
    bool r_pressed;
    bool o_pressed;
    bool n_pressed;
    inline bool callOnce(bool&, const short, void(ConsoleGuiHandler::*)(void));
    inline void fastForward(bool&, const short, void(ConsoleGuiHandler::*)(void));
public:
    KeypressHandler(ConsoleGuiHandler&);
    int start(void);
};
#endif //MADFM_KEYPRESSHANDLER_H
