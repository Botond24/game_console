#include "game.h"

// fully claude opus 4.6'd
namespace {

struct Glyph { uint8_t col[3]; };

// clang-format off
constexpr Glyph font[] = {
    // A        B        C        D        E        F
    {{0x1E,0x05,0x1E}}, {{0x1F,0x15,0x0A}}, {{0x0E,0x11,0x11}},
    {{0x1F,0x11,0x0E}}, {{0x1F,0x15,0x11}}, {{0x1F,0x05,0x01}},
    // G        H        I        J        K        L
    {{0x0E,0x11,0x1D}}, {{0x1F,0x04,0x1F}}, {{0x11,0x1F,0x11}},
    {{0x08,0x10,0x0F}}, {{0x1F,0x04,0x1B}}, {{0x1F,0x10,0x10}},
    // M        N        O        P        Q        R
    {{0x1F,0x02,0x1F}}, {{0x1F,0x01,0x1F}}, {{0x0E,0x11,0x0E}},
    {{0x1F,0x05,0x02}}, {{0x0E,0x11,0x2E}}, {{0x1F,0x05,0x1A}},
    // S        T        U        V        W        X
    {{0x12,0x15,0x09}}, {{0x01,0x1F,0x01}}, {{0x0F,0x10,0x0F}},
    {{0x07,0x18,0x07}}, {{0x1F,0x08,0x1F}}, {{0x1B,0x04,0x1B}},
    // Y        Z
    {{0x03,0x1C,0x03}}, {{0x19,0x15,0x13}},
    // 0        1        2        3        4        5
    {{0x0E,0x11,0x0E}}, {{0x12,0x1F,0x10}}, {{0x1A,0x15,0x12}},
    {{0x11,0x15,0x0A}}, {{0x07,0x04,0x1F}}, {{0x17,0x15,0x09}},
    // 6        7        8        9
    {{0x0E,0x15,0x08}}, {{0x01,0x19,0x07}}, {{0x0A,0x15,0x0A}},
    {{0x02,0x15,0x0E}},
    // space    colon
    {{0x00,0x00,0x00}}, {{0x00,0x0A,0x00}},
};
// clang-format on

int glyph_index(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a';
    if (c >= '0' && c <= '9') return 26 + (c - '0');
    if (c == ' ') return 36;
    if (c == ':') return 37;
    return 36; // default: space
}

void draw_char(const HAL* h, int ox, int oy, char c, uint16_t color, int scale = 3) {
    const Glyph& g = font[glyph_index(c)];
    for (int col = 0; col < 3; ++col)
        for (int row = 0; row < 5; ++row)
            if (g.col[col] & (1 << row))
                for (int sy = 0; sy < scale; ++sy)
                    for (int sx = 0; sx < scale; ++sx)
                        h->draw_pixel(ox + col * scale + sx, oy + row * scale + sy, color);
}

void draw_text(const HAL* h, int x, int y, const char* s, uint16_t color, int scale = 3) {
    while (*s) {
        draw_char(h, x, y, *s, color, scale);
        x += (3 + 1) * scale;
        ++s;
    }
}

// --- Drawing helpers ---

void draw_hline(const HAL* h, int x, int y, int len, uint16_t c) {
    for (int i = 0; i < len; ++i) h->draw_pixel(x + i, y, c);
}

void draw_vline(const HAL* h, int x, int y, int len, uint16_t c) {
    for (int i = 0; i < len; ++i) h->draw_pixel(x, y + i, c);
}

void draw_rect(const HAL* h, int x, int y, int w, int ht, uint16_t c) {
    draw_hline(h, x, y, w, c);
    draw_hline(h, x, y + ht - 1, w, c);
    draw_vline(h, x, y, ht, c);
    draw_vline(h, x + w - 1, y, ht, c);
}

void fill_rect(const HAL* h, int x, int y, int w, int ht, uint16_t c) {
    for (int row = y; row < y + ht; ++row)
        draw_hline(h, x, row, w, c);
}

// Arrow pointing to bottom-left
void draw_arrow_bottom_left(const HAL* h, int cx, int cy, uint16_t c) {
    // Shaft: diagonal from top-right to bottom-left
    for (int i = 0; i < 20; ++i)
        h->draw_pixel(cx + 19 - i, cy - 19 + i, c);
    // Arrowhead at bottom-left
    draw_vline(h, cx, cy - 6, 7, c);   // vertical bar
    draw_hline(h, cx, cy, 7, c);       // horizontal bar
}

// Arrow pointing to bottom-right
void draw_arrow_bottom_right(const HAL* h, int cx, int cy, uint16_t c) {
    // Shaft: diagonal from top-left to bottom-right
    for (int i = 0; i < 20; ++i)
        h->draw_pixel(cx + i, cy - 19 + i, c);
    // Arrowhead at bottom-right
    draw_vline(h, cx + 19, cy - 6, 7, c);
    draw_hline(h, cx + 13, cy, 7, c);
}

// D-pad graphic (cross shape)
void draw_dpad(const HAL* h, int cx, int cy, uint16_t c) {
    // Center of cross at (cx, cy). Each arm is 7px wide, 9px long.
    fill_rect(h, cx - 3, cy - 12, 7, 25, c);  // vertical bar
    fill_rect(h, cx - 12, cy - 3, 25, 7, c);  // horizontal bar
    // Outline
    draw_rect(h, cx - 3, cy - 12, 7, 25, c);
    draw_rect(h, cx - 12, cy - 3, 25, 7, c);
    // Arrow triangles on each arm
    // Up
    h->draw_pixel(cx, cy - 10, 0);
    h->draw_pixel(cx - 1, cy - 9, 0);
    h->draw_pixel(cx + 1, cy - 9, 0);
    // Down
    h->draw_pixel(cx, cy + 10, 0);
    h->draw_pixel(cx - 1, cy + 9, 0);
    h->draw_pixel(cx + 1, cy + 9, 0);
    // Left
    h->draw_pixel(cx - 10, cy, 0);
    h->draw_pixel(cx - 9, cy - 1, 0);
    h->draw_pixel(cx - 9, cy + 1, 0);
    // Right
    h->draw_pixel(cx + 10, cy, 0);
    h->draw_pixel(cx + 9, cy - 1, 0);
    h->draw_pixel(cx + 9, cy + 1, 0);
}

// Draw A and B buttons: A is left-lower, B is right-higher
void draw_ab_buttons(const HAL* h, int base_x, int base_y, uint16_t c) {
    int r = 7;
    // Button A: left and lower
    int ax = base_x - 10;
    int ay = base_y + 6;
    // Circle outline for A
    for (int dy = -r; dy <= r; ++dy)
        for (int dx = -r; dx <= r; ++dx)
            if (dx*dx + dy*dy >= (r-1)*(r-1) && dx*dx + dy*dy <= r*r)
                h->draw_pixel(ax + dx, ay + dy, c);
    draw_char(h, ax - 1, ay - 2, 'A', c, 1);

    // Button B: right and higher
    int bx = base_x + 10;
    int by = base_y - 6;
    for (int dy = -r; dy <= r; ++dy)
        for (int dx = -r; dx <= r; ++dx)
            if (dx*dx + dy*dy >= (r-1)*(r-1) && dx*dx + dy*dy <= r*r)
                h->draw_pixel(bx + dx, by + dy, c);
    draw_char(h, bx - 1, by - 2, 'B', c, 1);
}

// --- Screens ---

void screen_dpad(const HAL* h) {
    h->clear_screen();
    draw_text(h, 2, 2, "D PAD", 1);
    draw_dpad(h, 90, 38, 1);
    draw_arrow_bottom_left(h, 22, 54, 1);
    h->render();
    h->delay_ms(2000);
}

void screen_buttons(const HAL* h) {
    h->clear_screen();
    draw_text(h, 2, 2, "BUTTONS", 1);
    draw_ab_buttons(h, 38, 40, 1);
    draw_arrow_bottom_right(h, 70, 54, 1);
    h->render();
    h->delay_ms(2000);
}

const char* button_name(const KeyState& k) {
    if (k.up)    return "UP";
    if (k.down)  return "DOWN";
    if (k.left)  return "LEFT";
    if (k.right) return "RIGHT";
    if (k.a)     return "A";
    if (k.b)     return "B";
    return 0;
}

void screen_button_test(const HAL* h) {
    const char* last = "NONE";

    while (true) {
        KeyState k = h->read_input();
        const char* name = button_name(k);
        if (name) last = name;

        h->clear_screen();
        draw_text(h, 2, 2, "PRESSED:", 1);
        draw_text(h, 16, 34, last, 1);
        h->render();
        h->delay_ms(33);
    }
}

} // namespace

extern "C" __attribute__((section(".text.game_entry")))
void game_entry(const HAL* hal) {
    screen_dpad(hal);
    screen_buttons(hal);
    screen_button_test(hal);
}
