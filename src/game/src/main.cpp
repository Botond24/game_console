#include "game.h"

// do not touch this function
// it does some funky linker shit
extern "C" __attribute__((section(".text.game_entry")))
void game_entry(const HAL* hal) {
    // example gmae that moves a pixel
    int x = hal->screen_width / 2;
    int y = hal->screen_height / 2;

    while (true) {
        KeyState k = hal->read_input();
        if (k.left)  x--;
        if (k.right) x++;
        if (k.up)    y--;
        if (k.down)  y++;

        hal->clear_screen();
        hal->draw_pixel(x, y, 1);
        hal->render();
        hal->delay_ms(16);
    }
}
