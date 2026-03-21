#pragma once
#include "shared.h"
#include "pins.h"

#include <vector>
#include <SPI.h>
#include <SD.h>


class SDLoader {
public:
    bool init();

    template<typename F>
    void listRoot(F callback) {
        File root = SD.open("/");
        File entry = root.openNextFile();
        while (entry) {
            if (!entry.isDirectory()) callback(entry.name());
            entry.close();
            entry = root.openNextFile();
        }
    }
    
    bool testFat32();
};
