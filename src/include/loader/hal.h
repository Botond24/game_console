#pragma once

#include "shared.h"

struct KeyState {
    bool up;
    bool down;
    bool left;
    bool right;
    bool a;
    bool b;
};

struct HAL {
    void *ctx;
    void (*draw_pixel)(void* ctx, int x, int y, uint16_t whiteness);
    KeyState (*read_input)(void* ctx);
};

extern const HAL* volatile game_hal;
