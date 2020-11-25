#include "../../headers/gui/ConsoleLine.h"

#include <utility>

ConsoleLine::ConsoleLine() : attribute(0) { }

ConsoleLine::ConsoleLine(std::string text, WORD attribute) : text(std::move(text)), attribute(attribute) { }

void ConsoleLine::setText(std::string _text) {
    text = std::move(_text);
}

void ConsoleLine::setAttribute(WORD _attribute) {
    attribute = _attribute;
}

std::string ConsoleLine::getText() {
    return text;
}

WORD ConsoleLine::getAttribute() const {
    return attribute;
}

bool ConsoleLine::operator==(const ConsoleLine &other) {
    if(this->text == other.text && this->attribute == other.attribute)
        return true;
    return false;
}

bool ConsoleLine::operator!=(const ConsoleLine &other) {
    return !(*this == other);
}
