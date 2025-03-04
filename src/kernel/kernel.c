#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "modules/video/vga.h"
#include "modules/keyboard/keyboard.h"
#include "modules/func.h"
#include "modules/keyboard/keymap.h"
#include "modules/ram.h"
#include "modules/cpuinfo.h"
#include "modules/disk/io.h"

bool key_pressed[128] = {false};
bool displaying_files = false;
bool helping_files = false;
bool dmesg = false;
bool info = false;
bool mem = false;
bool cpuinfo = false;

#define SECTOR_SIZE 512
#define INPUT_BUFFER_SIZE 256

bool starts_with(const char *input, const char *cmd);

void clear_line(int line) {
    for (int i = 0; i < SCREEN_WIDTH; i++) {
        write_char(i, line, ' ', WHITE_ON_BLUE);
    }
}

bool starts_with(const char *input, const char *cmd) {
    while (*cmd) {
        if (*input++ != *cmd++) {
            return false;
        }
    }
    return (*input == '\0' || *input == ' ');
}

void reboot_system() {
    __asm__ volatile ("int $0x19");
}

uint32_t extract_sector_number(const char *input) {
    uint32_t num = 0;
    while (*input >= '0' && *input <= '9') {
        num = num * 10 + (*input - '0');
        input++;
    }
    return num;
}

void itoa(int num, char *str, int base) {
    int i = 0;
    bool is_negative = false;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0 && base == 10) {
        is_negative = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
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
        } else if (starts_with(input_buffer, "meminfo")) {
            display_memory_info();
            mem = true;
        } else if (starts_with(input_buffer, "cpuinfo")) {
            display_cpu_info();
            cpuinfo = true;
        } else if (starts_with(input_buffer, "read")) {
            char buffer[SECTOR_SIZE] = {0};
            uint32_t sector_number = extract_sector_number(input_buffer + 5);
            ata_read(sector_number, 1, (char *)buffer); 
            buffer[SECTOR_SIZE - 1] = '\0';

            char sector_str[20];
            itoa(sector_number, sector_str, 10);
            char message[100] = "Read 512 Bytes ";
            for (int i = 0; sector_str[i] != '\0'; i++) {
                message[26 + i] = sector_str[i];
            }
            write_string(7, 9, message, WHITE_ON_BLUE);
            write_string(7, 10, buffer, WHITE_ON_BLUE);
        } else if (starts_with(input_buffer, "write")) {
            char buffer[SECTOR_SIZE] = {0};
            char *data = input_buffer + 6;
            uint32_t sector_number = extract_sector_number(data);
            while (*data >= '0' && *data <= '9') {
                data++;
            }
            while (*data == ' ') {
                data++;
            }
            for (int i = 0; data[i] && i < SECTOR_SIZE - 1; i++) {
                buffer[i] = data[i];
            }
            int bytes_written = ata_write(sector_number, 1, (char *)buffer);

            char message[100];
            char byte_str[20], sector_str[20];

            itoa(bytes_written, byte_str, 10);
            itoa(sector_number, sector_str, 10);

            int j = 0;
            for (int i = 0; "Wrote "[i] != '\0'; i++) {
                message[j++] = "Wrote "[i];
            }

            for (int i = 0; byte_str[i] != '\0'; i++) {
                message[j++] = byte_str[i];
            }

            for (int i = 0; " bytes to sector "[i] != '\0'; i++) {
                message[j++] = " bytes to sector "[i];
            }

            for (int i = 0; sector_str[i] != '\0'; i++) {
                message[j++] = sector_str[i];
            }

            message[j] = '\0';

            write_string(7, 9, message, WHITE_ON_BLUE);
        } else {
            write_string(7, 9, "Unknown command.", WHITE_ON_BLUE);
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
                else if (mem) {
                    draw_shell_window();
                    mem = false;
                }
                else if (cpuinfo) {
                    draw_shell_window();
                    cpuinfo = false;
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
    init_ram();
    keyboard_listener();
}
