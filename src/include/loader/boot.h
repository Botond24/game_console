#pragma once
#include "shared.h"
#include "buttons.h"
#include "sd.h"
#include <USB.h>

#define GAME_LOAD_ADDR 0x20010000

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


class Bootloader {
    void DO_NOT_TOUCH();
    HAL hal;

    Oled oled;
    SDLoader loader;
    Buttons buttons;

    HAL create_hal();
    void launch(const char* path);

    void overlay_loop();
    void kill_game();
    void menu();
    void upload_mode();
public:
    bool pressed(int pin);
    void init();
    void run_game(void (*main_func)(const HAL&));
    HAL get_hal() const {
        return hal;
    }
};