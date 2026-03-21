#include "game.h"

// do not touch this function
// it does some funky linker shit
extern "C" __attribute__((section(".text.game_entry")))
void game_entry(const HAL* hal) {
    // example gmae that moves a pixel
    int x = hal->screenWidth / 2;
    int y = hal->screenHeight / 2;

    while (true) {
        KeyState k = hal->readInput();
        if (k.left)  x--;
        if (k.right) x++;
        if (k.up)    y--;
        if (k.down)  y++;

        hal->clearScreen();
        hal->drawPixel(x, y, 1);
        hal->render();
        hal->delayMs(16);
    }
}
