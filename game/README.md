
## Writing a Game

1. Go into/Copy the `game/` folder
2. Edit `src/main.cpp` — that's your game
3. Build with `pio run`
4. Copy `.pio/build/pico/firmware.bin` to the SD card (rename to `whatverthefuck.bin`)

### Game Entry Point
As you might have already noticed, your game is a separate PlatformIO project compiled to a raw `.bin` with a custom linker script.


```cpp
#include "game.h" // <- the HAL is already declared for u

extern "C" __attribute__((section(".text.game_entry")))
void game_entry(const HAL* hal) {
    while (true) {
        KeyState k = hal->read_input();
        hal->clear_screen();
        hal->draw_pixel(64, 32, 1);
        hal->render();
        hal->delay_ms(16);
    }
}
```

### HAL Reference

```cpp
struct HAL {
    void (*draw_pixel)(int x, int y, uint16_t color); // 0=black, 1=white
    void (*clear_screen)(void); // clear the framebuffer
    void (*render)(void); // push framebuffer to display
    KeyState (*read_input)(void); // read buttons
    void (*delay_ms)(uint32_t ms); // sleep
    int screen_width; // 128
    int screen_height; // 64
};

struct KeyState {
    bool up, down, left, right;
    bool a, b;
};
```
