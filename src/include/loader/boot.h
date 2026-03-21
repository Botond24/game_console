#pragma once
#include "shared.h"
#include "loader/hal.h"
#include "buttons.h"
#include "sd.h"
#include <USB.h>

#define GAME_LOAD_ADDR 0x20010000


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