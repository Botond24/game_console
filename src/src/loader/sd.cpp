#include "loader/sd.h"
bool SDLoader::init() {
    SPI.setRX(PIN_SD_MISO);
    SPI.setTX(PIN_SD_MOSI);
    SPI.setSCK(PIN_SD_SCLK);
    SPI.setCS(PIN_SD_CS);

    pinMode(PIN_SD_CS, OUTPUT);
    digitalWrite(PIN_SD_CS, HIGH);
    SPI.begin();

    return true;
}

bool SDLoader::testFat32() {
    // try making and removing a file
    if (!SD.begin(PIN_SD_CS)) {
        return false;
    }
    
    File testFile = SD.open("/test.txt", FILE_WRITE);
    if (!testFile) {
        return false;
    }

    testFile.println("This is a test.");
    testFile.close();

    if (!SD.remove("/test.txt")) {
        return false;
    }

    return true;
}