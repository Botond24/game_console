#include <Arduino.h>
#include "loader/boot.h"

bool Bootloader::pressed(int pin) {
    return digitalRead(pin) == LOW;
}

void Bootloader::DO_NOT_TOUCH() {
  rp2040.begin(0);
  initVariant();
  USB.begin();
}

// TODO: Linker script for games
void Bootloader::launch(const char* path) {
    File f = SD.open(path);
    uint8_t* dest = (uint8_t*)GAME_LOAD_ADDR;
    while (f.available()) *dest++ = f.read();
    f.close();

    // barrier
    __dmb();

    game_hal = &hal;

    rp2040.fifo.push(GAME_LOAD_ADDR);

    overlay_loop();
}

void Bootloader::overlay_loop() {
    while (true) {
        delay(50);
        if (pressed(PIN_BTN_A) && pressed(PIN_BTN_B) && pressed(PIN_UP)) {
            kill_game();
            menu();
            return;
        }
    }
}

void Bootloader::kill_game() {
    rp2040.restartCore1();
}

// TODO: USB mass storage mode to uplaod games to the SD card?
void Bootloader::upload_mode() {
    exception("Upload mode not implemented yet", oled);
}

// TODO: menu
void Bootloader::menu() {
  exception("No games and no bacon", oled);
}

void Bootloader::init() {

    DO_NOT_TOUCH();
    hal = create_hal();
    // boot sequence
    bool oled_ok = oled.init();
    oled.show_image(true);
    delay(1000);
    oled.clear();

    Print& stream = oled;
    #ifdef LOGGING
    Serial.begin(115200);
    delay(5000);
    stream = Serial;
    stream.println("[BOOT] Initializing bootloader...");
    #endif


    bool sd_ok = loader.init();
    bool buttons_ok = buttons.init();

    if (!buttons_ok) {
        exception("[BOOT] Buttons init failed!", stream);
    }

    // test fat32
    if (!loader.testFat32()) {
      exception("[BOOT] SD card test failed! Is the card FAT32 formatted?", stream);
      sd_ok = false;
    }
    
    if (!sd_ok) {
        stream.println("[BOOT] SD card init failed!");
    }

    menu();
}
