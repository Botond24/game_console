#include "loader/boot.h"

volatile bool gameRunning = false;

namespace {
Oled* gOled = nullptr;
}

HAL Bootloader::createHal() {
  gOled = &oled;

  HAL h = {};

  h.drawPixel = [](int x, int y, uint16_t color) {
    gOled->drawPixel(x, y, color);
  };

  h.clearScreen = []() {
    gOled->clear();
  };

  h.render = []() {
    gOled->render();
  };

  h.readInput = []() -> KeyState {
    return {
        digitalRead(PIN_UP) == LOW,
        digitalRead(PIN_DOWN) == LOW,
        digitalRead(PIN_LEFT) == LOW,
        digitalRead(PIN_RIGHT) == LOW,
        digitalRead(PIN_BTN_A) == LOW,
        digitalRead(PIN_BTN_B) == LOW
    };
  };

  h.delayMs = [](uint32_t ms) {
    delay(ms);
  };

  h.screenWidth = 128;
  h.screenHeight = 64;

  return h;
}
