#include <Arduino.h>
#include <USB.h>
#include "buttons.h"
#include "loader.h"
#include "oled.h"

void DO_NOT_TOUCH() {
  rp2040.begin(0);
  initVariant();
  USB.begin();
}

int main() {
    DO_NOT_TOUCH();

    Serial.begin(115200);
    delay(5000);
    
    Oled oled;
    Loader loader;
    Buttons buttons;

    bool oled_ok = oled.init();
    bool sd_ok = loader.init();
    buttons.init();
    
    Serial.print("OLED: ");
    Serial.println(oled_ok ? "OK" : "FAIL");
    Serial.print("SD Card: ");
    Serial.println(sd_ok ? "OK" : "FAIL");
    Serial.println("Button test ready");

    if (sd_ok) {
      loader.list_root();
    }

    while (true) {
      const char* clicked = buttons.read_clicked();
      if (clicked != nullptr) {
        Serial.print("Clicked: ");
        Serial.println(clicked);
        if (oled_ok) {
          oled.show_button(clicked);
        }
      }

      delay(20);
    }
    
    return 0;
}
