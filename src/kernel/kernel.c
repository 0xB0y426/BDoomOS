#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "modules/video/vga.h"
#include "modules/keyboard/keyboard.h"
#include "modules/func.h"
#include "modules/keyboard/keymap.h"

#define INPUT_BUFFER_SIZE 256

char input_buffer[INPUT_BUFFER_SIZE];
size_t input_index = 0;
bool key_pressed[128] = {false};
bool displaying_files = false;
bool helping_files = false; 
bool dmesg = false;
bool info = false;

bool starts_with(const char *input, const char *cmd);

void clear_line(int line) {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        write_char(i, line, ' ', WHITE_ON_BLUE);
    }
}

bool starts_with(const char *input, const char *cmd) {
    while (*cmd) {
        if (*input++ != *cmd++) return false;
    }
    return *input == '\0' || *input == ' ';
}

void reboot_system() {
    __asm__ volatile (
        "int $0x19"
    );
}

void process_command() {
    if (input_index > 0) {
        clear_line(9);

        if (starts_with(input_buffer, "drucker")) {
            write_string(7, 9, &input_buffer[8], WHITE_ON_BLACK);
        } else if (starts_with(input_buffer, "dir")) {
            display_files();
            displaying_files = true;
        } else if (starts_with(input_buffer, "clear")) {
            draw_shell_window();
        } else if (starts_with(input_buffer, "reboot")) {
            reboot_system();
        } else if (starts_with(input_buffer, "help")) {
            display_help();
            helping_files = true;
        } else if (starts_with(input_buffer, "dmesg")) {
            display_dmesg(); 
            dmesg = true;
        } else if (starts_with(input_buffer, "info")) {
            display_info();
            info = true;
        } else {
            write_string(7, 9, "Unknown Command", WHITE_ON_BLACK);
        }

        write_string(7, 7, "> ", WHITE_ON_BLACK);
        write_string(9, 7, input_buffer, WHITE_ON_BLACK);
    } else {
        write_string(7, 7, "> ", WHITE_ON_BLACK);
    }
}

void keyboard_listener() {
    draw_shell_window();

    while (true) {
        unsigned char key = inb(KEYBOARD_DATA_PORT);

        if (key < 128 && !key_pressed[key]) {
            key_pressed[key] = true;
            char pressed_key = keymap[key];

            if (pressed_key == '\n') {
                process_command();
                input_index = 0;
                input_buffer[0] = '\0';
            } else if (pressed_key == '\b' && input_index > 0) {
                input_buffer[--input_index] = '\0';
                write_char(9 + input_index, 7, ' ', WHITE_ON_BLACK);
            } else if (pressed_key != 0 && input_index < INPUT_BUFFER_SIZE - 1) {
                if (displaying_files) {
                    draw_shell_window();
                    displaying_files = false;
                } else if (helping_files) { 
                    draw_shell_window();
                    helping_files = false; 
                } else if (dmesg) {
                    draw_shell_window();
                    dmesg = false;
                } else if (info) {
                    draw_shell_window();
                    info = false;
                }

                
                input_buffer[input_index++] = pressed_key;
                input_buffer[input_index] = '\0';
                write_char(9 + input_index - 1, 7, pressed_key, WHITE_ON_BLACK);
            }
        } else if (key >= 128) {
            key_pressed[key - 128] = false;
        }
    }
}

void kernel_main() {
    keyboard_listener();
}
