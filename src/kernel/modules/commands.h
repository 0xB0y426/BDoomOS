#ifndef COMMANDS_H
#define COMMANDS_H

#include <stddef.h>
#include <stdint.h>
#include "video/vga.h"
#include "keyboard/keyboard.h"
#include "ram.h"
#include "disk/io.h"
#include "cpuinfo.h"

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
            ata_read(sector_number, 1, buffer);

            bool is_empty = true;
            for (int i = 0; i < SECTOR_SIZE; i++) {
                if (buffer[i] != 0) {
                    is_empty = false;
                    break;
                }
            }

            char sector_str[20];
            itoa(sector_number, sector_str, 10);

            if (is_empty) {
                write_string(7, 9, "Sector Successfully Readed", WHITE_ON_BLUE);
            } else {
                char message[100] = "Read 512 Bytes";
                int offset = 14;

                int sector_str_len = 0;
                while (sector_str[sector_str_len] != '\0') {
                    sector_str_len++;
                }

                for (int i = 0; i < sector_str_len; i++) {
                    message[offset + i] = sector_str[i];
                }

                message[offset + sector_str_len] = '\0';

                write_string(7, 9, message, WHITE_ON_BLUE);
                write_string(7, 10, buffer, WHITE_ON_BLUE);
            }
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

#endif // COMMANDS_H