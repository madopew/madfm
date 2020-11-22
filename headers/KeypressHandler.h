#ifndef MADFM_KEYPRESSHANDLER_H
#define MADFM_KEYPRESSHANDLER_H

#include "ConsoleGuiHandler.h"

class KeypressHandler {
private:
    ConsoleGuiHandler cgh;
public:
    KeypressHandler(ConsoleGuiHandler&);
    int start(void);
};
#endif //MADFM_KEYPRESSHANDLER_H
