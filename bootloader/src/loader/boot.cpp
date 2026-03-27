#include <Arduino.h>
#include <vector>
#include <Adafruit_TinyUSB.h>
#include "loader/boot.h"

constexpr uint32_t SRAM_END = 0x20040000;
constexpr size_t MAX_SIZE = SRAM_END - LD_ADDR;
constexpr uint16_t BLOCK_SIZE = 512;

Adafruit_USBD_MSC usbMsc;
Sd2Card usbMscCard;
SdVolume usbMscVolume;
bool usbMscStarted = false;

// necessary functions for the USB MSC
int32_t MSCRead(uint32_t lba, void* buffer, uint32_t bufsize) {
    (void)bufsize;
    return usbMscCard.readBlock(lba, static_cast<uint8_t*>(buffer)) ? BLOCK_SIZE : -1;
}

int32_t MSCWrite(uint32_t lba, uint8_t* buffer, uint32_t bufsize) {
    (void)bufsize;
    return usbMscCard.writeBlock(lba, buffer) ? BLOCK_SIZE : -1;
}

void mscFlushCb(void) {
}

void massStorage() {
    if (usbMscStarted) {
        return;
    }

    usbMsc.setID("ChipKid:tm:", "SD Upload", "1.0");
    usbMsc.setReadWriteCallback(MSCRead, MSCWrite, mscFlushCb);
    usbMsc.setUnitReady(false);
    usbMsc.begin();

    if (TinyUSBDevice.mounted()) {
        TinyUSBDevice.detach();
        delay(10);
        TinyUSBDevice.attach();
    }

    usbMscStarted = true;
}

bool isGameFile(const char* name) {
    if (!name) {
        return false;
    }

    String filename(name);
    filename.toLowerCase();
    return filename.endsWith(".bin");
}

String gamePath(const char* name) {
    if (!name || name[0] == '\0') {
        return String();
    }
    if (name[0] == '/') {
        return String(name);
    }
    return String("/") + name;
}

std::vector<String> findGames(SDLoader& loader) {
    std::vector<String> games;
    loader.listRoot([&games](const char* name) {
        if (isGameFile(name)) {
            games.push_back(String(name));
        }
    });
    return games;
}

void drawMenu(Oled& oled, const std::vector<String>& items, size_t selected) {
    oled.clear();
    oled.println("Menu");
    oled.println("A=Select B=Rescan");

    if (items.empty()) {
        oled.println("No .bin files found");
        oled.render();
        return;
    }

    size_t start = 0;
    if (selected >= 3) {
        start = selected - 2;
    }
    size_t end = start + 4;
    if (end > items.size()) {
        end = items.size();
    }

    for (size_t i = start; i < end; ++i) {
        oled.print(i == selected ? "> " : "  ");
        oled.println(items[i]);
    }

    oled.render();
}

bool Bootloader::pressed(int pin) {
    return digitalRead(pin) == LOW;
}

void Bootloader::DO_NOT_TOUCH() {
  rp2040.begin(0);
  initVariant();
}

void Bootloader::launch(const char* path) {
    String fullPath = gamePath(path);

    File f = SD.open(fullPath.c_str(), FILE_READ);
    if (!f) {
        exception("[BOOT] Failed to open game file", oled);
    }

    size_t gameSize = f.size();
    if (gameSize == 0) {
        exception("[BOOT] Game file is empty", oled);
    }

    if (gameSize > MAX_SIZE) {
        exception("[BOOT] Game too large for RAM", oled);
    }

    uint8_t* dest = (uint8_t*)LD_ADDR;
    memset(dest, 0, MAX_SIZE);
    while (f.available()) *dest++ = f.read();
    f.close();

    // barrier
    __dmb();

    oled.clear();
    oled.render();

    gameRunning = true;

    typedef void (*game_entry_t)(const HAL*);
    ((game_entry_t)(LD_ADDR | 1u))(&hal);

    gameRunning = false;
}

void Bootloader::uploadMode() {
    massStorage();

    SD.end();

    bool cardOk = usbMscCard.init(SPI_HALF_SPEED, PIN_SD_CS);
    bool volumeOk = cardOk && usbMscVolume.init(usbMscCard);

    oled.clear();
    oled.println("Upload Mode");

    if (volumeOk) {
        uint32_t blockCount = usbMscVolume.blocksPerCluster() * usbMscVolume.clusterCount();
        usbMsc.setCapacity(blockCount, BLOCK_SIZE);
        usbMsc.setUnitReady(true);

        oled.println("SD is USB drive.");
        oled.println("Copy .bin files.");
        oled.println("Press B to exit.");
    } else {
        usbMsc.setUnitReady(false);
        exception("[BOOT] Failed to initialize SD card for USB mass storage", oled);
    }

    oled.render();

    bool prevB = false;
    while (true) {
        bool b = pressed(PIN_BTN_B);
        if (b && !prevB) {
            usbMsc.setUnitReady(false);
            SD.begin(PIN_SD_CS);
            return;
        }
        prevB = b;
        delay(30);
    }
}

void Bootloader::menu() {
    std::vector<String> entries;
    entries.push_back("!UPLOAD MODE!");

    std::vector<String> games = findGames(loader);
    for (const String& game : games) {
        entries.push_back(game);
    }

    size_t selected = 0;
    bool prevUp = false;
    bool prevDown = false;
    bool prevA = false;
    bool prevB = false;

    drawMenu(oled, entries, selected);

    while (true) {
        bool up = pressed(PIN_UP);
        bool down = pressed(PIN_DOWN);
        bool a = pressed(PIN_BTN_A);
        bool b = pressed(PIN_BTN_B);

        if (up && !prevUp && selected > 0) {
            --selected;
            drawMenu(oled, entries, selected);
        }

        if (down && !prevDown && (selected + 1) < entries.size()) {
            ++selected;
            drawMenu(oled, entries, selected);
        }

        if (a && !prevA) {
            if (selected == 0) {
                uploadMode();
                drawMenu(oled, entries, selected);
            } else {
                launch(entries[selected].c_str());
                drawMenu(oled, entries, selected);
            }
        }

        if (b && !prevB) {
            entries.clear();
            entries.push_back("[Upload mode USB]");
            games = findGames(loader);
            for (const String& game : games) {
                entries.push_back(game);
            }
            if (selected >= entries.size()) {
                selected = entries.size() - 1;
            }
            drawMenu(oled, entries, selected);
        }

        prevUp = up;
        prevDown = down;
        prevA = a;
        prevB = b;
        delay(35);
    }
}

void Bootloader::init() {

    DO_NOT_TOUCH();
    hal = createHal();
    // boot sequence
    bool oledOk = oled.init();
    oled.showImage(true);
    delay(1000);
    oled.clear();

    Print& stream = oled;
    #ifdef LOGGING
    Serial.begin(115200);
    delay(5000);
    stream = Serial;
    stream.println("[BOOT] Initializing bootloader...");
    #endif


    bool sdOk = loader.init();
    bool buttonsOk = buttons.init();

    if (!buttonsOk) {
        exception("[BOOT] Buttons init failed!", stream);
    }

    // test fat32
    if (!loader.testFat32()) {
      exception("[BOOT] SD card test failed! Is the card FAT32 formatted?", stream);
      sdOk = false;
    }
    
    if (!sdOk) {
        stream.println("[BOOT] SD card init failed!");
    }

    menu();
}
