#pragma once
#include <Adafruit_SSD1306.h>
#include <pico/mutex.h>

class Oled : public Print {
private:
    Adafruit_SSD1306 display;
    mutex_t mutex;

    void lock()   { mutex_enter_blocking(&mutex); }
    void unlock() { mutex_exit(&mutex); }
public:
    Oled();
    bool init();
    void show_image(bool show);
    void clear();
    void flush();
    void render();
    void drawPixel(int x, int y, uint16_t whiteness);
    size_t write(uint8_t) override;
    size_t print(const String& msg);
    size_t println(const String& msg);
};