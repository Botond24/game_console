#include "loader/boot.h"
#include <hardware/watchdog.h>

Bootloader bootloader;

void setup() {
    bootloader.init();
}

void loop() {}

void setup1() {}

void loop1() {
    if (!game_running) {
        delay(100);
        return;
    }
    if (digitalRead(PIN_BTN_A) == LOW &&
        digitalRead(PIN_BTN_B) == LOW &&
        digitalRead(PIN_UP) == LOW) {
        watchdog_reboot(0, 0, 10);
    }
    delay(50);
}
