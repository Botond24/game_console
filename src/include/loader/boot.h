#pragma once
#include "shared.h"
#include "loader/hal.h"
#include "buttons.h"
#include "sd.h"

#define GAME_LOAD_ADDR 0x20010000


class Bootloader {
    void DO_NOT_TOUCH();
    HAL hal;

    Oled oled;
    SDLoader loader;
    Buttons buttons;

    HAL create_hal();
    void launch(const char* path);

    void menu();
    void upload_mode();
public:
    bool pressed(int pin);
    void init();
};