#include "buttons.h"
#include <Arduino.h>

bool Buttons::init() {
    for (auto& btn : BUTTONS) {
        pinMode(btn.pin, INPUT_PULLUP);
    }

    return true;
}

