#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "video/vga.h"
#include "keyboard/keyboard.h"

static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void display_files() {
    int start_x = 7;
    int start_y = 9;
    
    write_string(start_x, start_y, "BDoomOS.bin", WHITE_ON_BLACK);
    write_string(start_x, start_y + 1, "builder.sh", WHITE_ON_BLACK);
    write_string(start_x, start_y + 2, "BDoomOS.iso", WHITE_ON_BLACK);
    write_string(start_x, start_y + 3, "gccbuilder.sh", WHITE_ON_BLACK);
    write_string(start_x, start_y + 4, "boot.o", WHITE_ON_BLACK);
    write_string(start_x, start_y + 5, "kernel.o", WHITE_ON_BLACK);
    write_string(start_x, start_y + 6, "boot.s", WHITE_ON_BLACK);
    write_string(start_x, start_y + 7, "kernel.s", WHITE_ON_BLACK);
    write_string(start_x, start_y + 8, "kernel.c", WHITE_ON_BLACK);
}

void display_system_info_window() {
    draw_window(35, 2, 30, 10, "System Info", WHITE_ON_BLACK);
    write_string(37, 4, "OS Version: BDoomOS 0.2", WHITE_ON_BLACK);
    write_string(37, 6, "Processor: x86", WHITE_ON_BLACK);
    write_string(37, 8, "Bootloader: GRUB", WHITE_ON_BLACK);
}
void display_info() {
    int start_x = 7;
    int start_y = 9;
    
    write_string(start_x, start_y + 1, "System Info:", WHITE_ON_BLACK);
    write_string(start_x, start_y + 2, "OS Version: BDoomOS 0.2", WHITE_ON_BLACK);
    write_string(start_x, start_y + 3, "Processor: x86", WHITE_ON_BLACK);
    write_string(start_x, start_y + 4, "Bootloader: GRUB'", WHITE_ON_BLACK);
}

void display_help() {
    int start_x = 7;
    int start_y = 9;
    
    write_string(start_x, start_y, "Help Menu", WHITE_ON_BLACK);
    write_string(start_x, start_y + 1, "clear - Clear the screen.", WHITE_ON_BLACK);
    write_string(start_x, start_y + 2, "reboot - Reboot the system.", WHITE_ON_BLACK);
    write_string(start_x, start_y + 3, "dir - List the files of system.", WHITE_ON_BLACK);
    write_string(start_x, start_y + 4, "drucker - Print something in the screen 'drucker BDoomOS'", WHITE_ON_BLACK);
    write_string(start_x, start_y + 5, "dmesg - Display kernel messages.", WHITE_ON_BLACK);
    write_string(start_x, start_y + 6, "info - Display system information.", WHITE_ON_BLACK);
}

void draw_shell_window() {
    clear_screen_with_color(WHITE_ON_BLUE);
    draw_window(5, 3, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 6, "BDoomOS Shell", WHITE_ON_BLACK);
    write_string(7, 5, "Type 'help' to see commands", WHITE_ON_BLACK);
    write_string(7, 7, "> ", WHITE_ON_BLACK);
}

void clear_screen(char color) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            write_char(x, y, ' ', color);
        }
    }
}


void display_dmesg() {
    int start_x = 7;
    int start_y = 9;
    
    write_string(start_x, start_y + 1, "GRUB Loaded BDoomOS.iso", WHITE_ON_BLACK);
    write_string(start_x, start_y + 2, "Booting Kernel...", WHITE_ON_BLACK);
    write_string(start_x, start_y + 3, "Initing Kernel...", WHITE_ON_BLACK);
    write_string(start_x, start_y + 4, "Loaded!!'", WHITE_ON_BLACK);
    write_string(start_x, start_y + 5, "BDoomOS 0.2 ready.'", WHITE_ON_BLACK);
}

#endif 
