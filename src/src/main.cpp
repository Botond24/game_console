#include <Arduino.h>
#include <USB.h>
#include "loader.h"
#include "buttons.h"
#include "oled.h"

void DO_NOT_TOUCH() {
  rp2040.begin(0);
  initVariant();
  USB.begin();
}

int main() {
    DO_NOT_TOUCH();

    Serial.begin(115200);
    delay(1000);
    
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
    Serial.println("Buttons: OK");
    
    bool show_image = true;
    
    while (true) {
        oled.show_image(show_image);
        show_image = !show_image;
        
        // buttons.test();
        delay(1000);
    }
    
    return 0;
}
