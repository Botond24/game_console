Unfortunately, you'll have to learn a little bit of PlatformIO to be able to program your game console. Fortunately, it's not that hard.

The project is structured in a way that should be familiar to anyone who has done C programming before. The `src` directory contains the source files, while the `include` directory contains the header files. The `lib` directory contains any libraries that the project depends on (you won't need to worry about this, unless you can't add a library using PlatformIO's library manager).

Obviously, `main.cpp` is the entry point of the program. 

## Project Structure

- `src/` — Bootloader firmware (PlatformIO, Arduino framework)
- `game/` — **Game template**: this is what you should edit/copy to make your own game.
- `test-games/` — Example games

## The Bootloader

1. Initializes the OLED display and shows the splash screen
2. Initializes the SD card (must be FAT32 formatted)
3. Initializes the buttons
4. Shows the game menu

From the menu:
- **A** = select a `.bin` game to launch, or enter USB upload mode
- **B** = rescan the SD card
- **A + B + UP** (held briefly) = kill a running game (reboots the console, lol)

