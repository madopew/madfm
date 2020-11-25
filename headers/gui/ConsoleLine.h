#include <string>
#include <Windows.h>

#ifndef MADFM_CONSOLELINE_H
#define MADFM_CONSOLELINE_H
class ConsoleLine {
private:
    std::string text;
    WORD attribute;
public:
    ConsoleLine(void);
    ConsoleLine(std::string, WORD);
    void setText(std::string);
    void setAttribute(WORD);
    std::string getText(void);
    WORD getAttribute(void) const;
    bool operator ==(const ConsoleLine&);
    bool operator !=(const ConsoleLine&);
};
#endif //MADFM_CONSOLELINE_H
