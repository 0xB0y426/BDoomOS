#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "video/vga.h"
#include "keyboard/keyboard.h"

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

void display_info() {
    int start_x = 7;
    int start_y = 9;
    
    write_string(start_x, start_y + 1, "System Info:", WHITE_ON_BLACK);
    write_string(start_x, start_y + 2, "OS Version: BDoomOS 0.5", WHITE_ON_BLACK);
    write_string(start_x, start_y + 3, "Architecture: x86", WHITE_ON_BLACK);
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
    write_string(start_x, start_y + 7, "meminfo - Memory RAM Info", WHITE_ON_BLACK);
    write_string(start_x, start_y + 8, "cpuinfo - Display CPU informations.", WHITE_ON_BLACK);
    write_string(start_x, start_y + 9, "write [sector] - Write 512 bytes in a sector, Example: 'write 1'", WHITE_ON_BLACK);
    write_string(start_x, start_y + 10, "read [sector] - Read a sector, Example: 'read 1'", WHITE_ON_BLACK);
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
    write_string(start_x, start_y + 5, "BDoomOS 0.5 ready.'", WHITE_ON_BLACK);
}

#endif 
