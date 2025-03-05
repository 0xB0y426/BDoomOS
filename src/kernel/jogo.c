#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "modules/video/vga.h"
#include "modules/keyboard/keyboard.h"
#include "modules/func.h"
#include "modules/keyboard/keymap.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define CURSOR_CHAR 'X'

int cursor_x = SCREEN_WIDTH / 2;
int cursor_y = SCREEN_HEIGHT / 2;

void draw_village() {
    write_char(5, 5, 'H', WHITE_ON_BLACK);
    write_char(6, 5, 'H', WHITE_ON_BLACK);
    write_char(5, 6, 'H', WHITE_ON_BLACK);
    write_char(5, 4, 'H', WHITE_ON_BLACK);
    write_char(6, 4, 'H', WHITE_ON_BLACK);

    write_char(10, 5, 'H', WHITE_ON_BLACK);
    write_char(11, 5, 'H', WHITE_ON_BLACK);
    write_char(10, 6, 'H', WHITE_ON_BLACK);
    write_char(10, 4, 'H', WHITE_ON_BLACK);
    write_char(11, 4, 'H', WHITE_ON_BLACK);

    write_char(15, 5, 'H', WHITE_ON_BLACK);
    write_char(16, 5, 'H', WHITE_ON_BLACK);
    write_char(15, 6, 'H', WHITE_ON_BLACK);
    write_char(15, 4, 'H', WHITE_ON_BLACK);
    write_char(16, 4, 'H', WHITE_ON_BLACK);

    write_char(18, 4, 'T', WHITE_ON_BLACK);
    write_char(18, 5, 'T', WHITE_ON_BLACK);
    write_char(18, 6, 'T', WHITE_ON_BLACK);
    write_char(18, 3, 'T', WHITE_ON_BLACK);
    
    write_char(22, 4, 'T', WHITE_ON_BLACK);
    write_char(22, 5, 'T', WHITE_ON_BLACK);
    write_char(22, 6, 'T', WHITE_ON_BLACK);
    write_char(22, 3, 'T', WHITE_ON_BLACK);

    write_char(20, 7, '*', GREEN_ON_BLACK);
    write_char(21, 7, '*', GREEN_ON_BLACK);
    write_char(19, 7, '*', GREEN_ON_BLACK);

    write_char(1, 1, 'O', YELLOW_ON_BLACK);

    for (int x = 30; x < 35; x++) {
        write_char(x, 15, '~', LIGHT_BLUE_ON_BLACK);
    }

    for (int x = 1; x < SCREEN_WIDTH - 1; x++) {
        write_char(x, 10, '-', WHITE_ON_BLACK);
    }

    write_string(1, 2, "Press arrow keys to move around the village.", WHITE_ON_BLACK);
    write_string(1, 3, "Explore the houses, trees, and flowers.", WHITE_ON_BLACK);
    
}

void draw_cursor() {
    write_char(cursor_x, cursor_y, CURSOR_CHAR, WHITE_ON_BLUE);
}

void clear_cursor() {
    write_char(cursor_x, cursor_y, ' ', WHITE_ON_BLACK);
}

void move_cursor(int dx, int dy) {
    clear_cursor();
    cursor_x += dx;
    cursor_y += dy;

    if (cursor_x < 1) cursor_x = 1;
    if (cursor_x >= SCREEN_WIDTH - 1) cursor_x = SCREEN_WIDTH - 2;
    if (cursor_y < 1) cursor_y = 1;
    if (cursor_y >= SCREEN_HEIGHT - 1) cursor_y = SCREEN_HEIGHT - 2;

    draw_cursor();
}

void keyboard_listener() {
    clear_screen(WHITE_ON_BLACK);
    draw_village();
    draw_cursor();

    while (true) {
        unsigned char key = inb(KEYBOARD_DATA_PORT);

        if (key == 0xE0) {
            key = inb(KEYBOARD_DATA_PORT);
            if (key == 0x48) {
                move_cursor(0, -1);
            } else if (key == 0x50) {
                move_cursor(0, 1);
            } else if (key == 0x4B) {
                move_cursor(-1, 0);
            } else if (key == 0x4D) {
                move_cursor(1, 0);
            }
        }
    }
}

void kernel_main() {
    keyboard_listener();
}
