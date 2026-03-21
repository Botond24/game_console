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
    void (*draw_pixel)(int x, int y, uint16_t color);
    void (*clear_screen)(void);
    void (*render)(void);
    KeyState (*read_input)(void);
    void (*delay_ms)(uint32_t ms);
    int screen_width;
    int screen_height;
};

extern volatile bool game_running;
