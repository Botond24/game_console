Unfortunately, you'll have to learn a little bit of PlatformIO to be able to program your game console. Fortunately, it's not that hard.

The project is structured in a way that should be familiar to anyone who has done C programming before. The `src` directory contains the source files, while the `include` directory contains the header files. The `lib` directory contains any libraries that the project depends on (you won't need to worry about this, unless you can't add a library using PlatformIO's library manager).

Obviously, `main.cpp` is the entry point of the program. 

# The Bootloader

The bootloader runs on **core 0** and handles all hardware init, the menu, and game lifecycle. On startup it:

1. Initializes the OLED display and shows the splash screen
2. Initializes the SD card (must be FAT32 formatted)
3. Initializes the buttons
4. Shows the game menu

## Launching a Game

Games are `.bin` files on the SD card. The bootloader loads the binary into RAM at a fixed address and runs it on **core 1**. Core 0 continues running the overlay loop.

Hold **A + B + UP** at any time to kill the game and return to the menu.

# Writing a Game

Your game is a separate PlatformIO project compiled to a raw `.bin` with a custom linker script (TODO). It gets a single entry point:
```cpp
extern "C" void game_entry(const HAL* hal) {
    while (true) {
        KeyState input = hal->read_input(hal->ctx);
        if (input.right) {
            hal->draw_pixel(hal->ctx, x, y, 255);
        }
    }
}
```

## HAL Reference
```cpp
struct HAL {
    void* ctx; // pass this back to every function

    // draw a pixel. whiteness: 0 = black, 255 = white
    void (*draw_pixel)(void* ctx, int x, int y, uint16_t whiteness);

    // read current button state
    KeyState (*read_input)(void* ctx);
};

struct KeyState {
    bool up, down, left, right;
    bool a, b;
};
```
