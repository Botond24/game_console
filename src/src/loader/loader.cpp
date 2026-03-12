#include "loader.h"
#include "pins.h"
#include <SPI.h>
#include <SD.h>

bool Loader::init() {
    SPI.setRX(PIN_SD_MISO);
    SPI.setTX(PIN_SD_MOSI);
    SPI.setSCK(PIN_SD_SCLK);
    SPI.setCS(PIN_SD_CS);

    pinMode(PIN_SD_CS, OUTPUT);
    digitalWrite(PIN_SD_CS, HIGH);
    SPI.begin();

    if (!SD.begin(PIN_SD_CS)) {
        Serial.println("[SD] FAIL - card not detected or init failed");
        return false;
    }

    return true;
}

int Loader::list_root() {
    File root = SD.open("/");

    Serial.println("[SD] Root listing:");
    int count = 0;
    File entry = root.openNextFile();
    while (entry) {
        Serial.print(" - ");
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
        } else {
            Serial.print(" (");
            Serial.print(entry.size());
            Serial.println(" bytes)");
        }
        entry.close();
        count++;
        entry = root.openNextFile();
    }

    root.close();
    return count;
}
