#include "loader/boot.h"

Bootloader bootloader;

void setup() {
    bootloader.init();
}

void loop() {}

void setup1() {
    uint32_t addr = rp2040.fifo.pop();
    typedef void (*game_entry_t)(const HAL*);
    ((game_entry_t)addr)(game_hal);
}

void loop1() {}
