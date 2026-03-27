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
    void (*drawText)(int x, int y, const char* text, uint16_t color, uint8_t font[96][6]);
    void (*drawChar)(int x, int y, char c, uint16_t color, uint8_t font[96][6]);
    void (*clearScreen)(void);
    void (*render)(void);
    KeyState (*readInput)(void);
    void (*delayMs)(uint32_t ms);
    int (*sdRead)(const char* path, uint8_t* buf, uint32_t offset, uint32_t len);
    int (*sdWrite)(const char* path, const uint8_t* buf, uint32_t len);
    uint64_t (*getTime)(void);
    int screenWidth;
    int screenHeight;
};

#endif
