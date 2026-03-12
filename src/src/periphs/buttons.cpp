#include "buttons.h"
#include "pins.h"
#include <Arduino.h>

struct Button {
    const char* name;
    uint8_t pin;
};

static const Button BUTTONS[] = {
    {"A",     PIN_BTN_A},
    {"B",     PIN_BTN_B},
    {"UP",    PIN_UP},
    {"DOWN",  PIN_DOWN},
    {"LEFT",  PIN_LEFT},
    {"RIGHT", PIN_RIGHT},
};

static uint8_t read_state_mask() {
    uint8_t mask = 0;
    for (uint8_t i = 0; i < (sizeof(BUTTONS) / sizeof(BUTTONS[0])); ++i) {
        const bool pressed = digitalRead(BUTTONS[i].pin) == LOW;
        if (pressed) {
            mask |= (1u << i);
        }
    }
    return mask;
}

void Buttons::init() {
    for (auto& btn : BUTTONS) {
        pinMode(btn.pin, INPUT_PULLUP);
    }

    prev_state_mask = read_state_mask();
    Serial.println("[BUTTONS] Initialized");
}

const char* Buttons::read_clicked() {
    const uint8_t current_mask = read_state_mask();

    for (uint8_t i = 0; i < (sizeof(BUTTONS) / sizeof(BUTTONS[0])); ++i) {
        const bool was_pressed = (prev_state_mask & (1u << i)) != 0;
        const bool is_pressed = (current_mask & (1u << i)) != 0;
        if (!was_pressed && is_pressed) {
            prev_state_mask = current_mask;
            return BUTTONS[i].name;
        }
    }

    prev_state_mask = current_mask;
    return nullptr;
}
