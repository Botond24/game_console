#pragma once

#include <Adafruit_SSD1306.h>

class Oled {
private:
    Adafruit_SSD1306 display;
public:
    Oled();
    bool init();
    void show_image(bool show);
    void show_button(const char* button_name);
};
