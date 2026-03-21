#include "loader/boot.h"

const HAL *volatile game_hal = nullptr;

HAL Bootloader::create_hal() {
  hal.ctx = this;
  // TODO: have core 0 own all rendering and core 1 just submits draw commands
  hal.draw_pixel = [](void *ctx, int x, int y, uint16_t whiteness) {
    Bootloader *bootloader = (Bootloader *)ctx;
    bootloader->oled.drawPixel(x, y, whiteness);
  };

  hal.read_input = [](void *ctx) -> KeyState {
    Bootloader *bootloader = (Bootloader *)ctx;
    KeyState state = {
        .up = bootloader->pressed(PIN_UP),
        .down = bootloader->pressed(PIN_DOWN),
        .left = bootloader->pressed(PIN_LEFT),
        .right = bootloader->pressed(PIN_RIGHT),
        .a = bootloader->pressed(PIN_BTN_A),
        .b = bootloader->pressed(PIN_BTN_B)
        };
    return state;
  };

  return hal;
}
