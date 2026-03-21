#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>

struct KeyState {
    bool up;
    bool down;
    bool left;
    bool right;
    bool a;
    bool b;
};

struct HAL {
    void (*drawPixel)(int x, int y, uint16_t color);
    void (*clearScreen)(void);
    void (*render)(void);
    KeyState (*readInput)(void);
    void (*delayMs)(uint32_t ms);
    int screenWidth;
    int screenHeight;
};

#endif
