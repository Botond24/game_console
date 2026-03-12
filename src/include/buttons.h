#pragma once

#include <stdint.h>

class Buttons {
public:
    void init();
    const char* read_clicked();

private:
    uint8_t prev_state_mask = 0;
};
