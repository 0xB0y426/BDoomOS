#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "modules/kernel_log.h"

#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLUE 0x1F
#define SELECTED_COLOR 0x70
#define HIGHLIGHT_COLOR 0x1F
#define INFO_COLOR 0x0F
#define WHITE_ON_BLACK 0x0F
#define WHITE_ON_WHITE 0x0F

#define KEYBOARD_DATA_PORT 0x60

#define MAX_MESSAGES 10

volatile char *video = (volatile char *)VIDEO_MEMORY;

typedef struct {
    char message[80];
} KernelMessage;

KernelMessage dmesg[MAX_MESSAGES];
int dmesg_index = 0;

static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void write_char(int x, int y, char c, char color) {
    int offset = (y * SCREEN_WIDTH + x) * 2;
    video[offset] = c;
    video[offset + 1] = color;
}

void write_string(int x, int y, const char *str, char color) {
    size_t i = 0;
    while (str[i] != '\0') {
        write_char(x + i, y, str[i], color);
        i++;
    }
}

void clear_screen() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            write_char(x, y, ' ', WHITE_ON_BLUE);  
        }
    }
}

void log_message(const char *msg) {
    if (dmesg_index < MAX_MESSAGES) {
        for (int i = 0; i < 80 && msg[i] != '\0'; i++) {
            dmesg[dmesg_index].message[i] = msg[i];
        }
        dmesg_index++;
    }
}

void display_dmesg() {
    clear_screen();
    write_string(5, 5, "Kernel Messages:", HIGHLIGHT_COLOR);
    
    for (int i = 0; i < dmesg_index; i++) {
        write_string(5, 7 + i, dmesg[i].message, WHITE_ON_BLUE);
    }

    write_string(5, 14, "Reboot the system to return.", WHITE_ON_BLUE);
    while (inb(KEYBOARD_DATA_PORT) != 0x1C);  
}

void reboot() {
    asm volatile ("int $0x19");
}

void system_init() {
    SYSTEM_INIT_MESSAGES();
}

void list_files() {
    const char *files[] = {
        "BDoomOS.bin",
        "BDoomOS.iso",
        "boot.o",
        "boot.s",
        "builder.sh",
        "gccbuilder.sh",
        "kernel.o",
        "kernel.s",
        "linker.ld"
    };

    clear_screen();
    write_string(5, 5, "File List:", HIGHLIGHT_COLOR);

    for (int i = 0; i < 6; i++) {
        write_string(5, 7 + i, files[i], WHITE_ON_BLUE);  
    }

    write_string(5, 14, "Reboot the system to return", WHITE_ON_BLUE);  
    while (inb(KEYBOARD_DATA_PORT) != 0x1C);  
}

void draw_menu_box() {
    int start_x = (SCREEN_WIDTH - 30) / 2;
    int start_y = 7;
    
    for (int y = start_y; y < start_y + 4; y++) {
        for (int x = start_x; x < start_x + 30; x++) {
            write_char(x, y, ' ', WHITE_ON_WHITE);  
        }
    }
    
    for (int x = start_x; x < start_x + 30; x++) {
        write_char(x, start_y - 1, '-', WHITE_ON_WHITE);  
        write_char(x, start_y + 3, '-', WHITE_ON_WHITE);  
    }
    for (int y = start_y; y < start_y + 4; y++) {
        write_char(start_x - 1, y, '|', WHITE_ON_WHITE);  
        write_char(start_x + 29, y, '|', WHITE_ON_WHITE);  
    }
}

void wait_for_1_second() {
    for (volatile int i = 0; i < 100000000; i++) {

    }
}

void gui_main() {
    int selected_option = 0;
    const char *options[] = {
        "1. List Files",
        "2. Kernel Messages"
    };

    bool screen_updated = false;

    clear_screen();
    write_string((SCREEN_WIDTH - 17) / 2, 2, "Welcome to BDoomOS 0.1", HIGHLIGHT_COLOR);
    write_string((SCREEN_WIDTH - 33) / 2, 4, "Please wait, loading system...", WHITE_ON_BLUE);
    wait_for_1_second();  

    while (true) {
        unsigned char key = inb(KEYBOARD_DATA_PORT);

        if (key == 0x1C) {
            if (selected_option == 0) {
                list_files();
            } else if (selected_option == 1) {
                display_dmesg();
            }
            break;
        }

        if (!screen_updated) {
            clear_screen();
            write_string((SCREEN_WIDTH - 10) / 2, 2, "BDoomOS 0.1", HIGHLIGHT_COLOR);
            draw_menu_box();

            for (int i = 0; i < 2; i++) {
                char color = (i == selected_option) ? SELECTED_COLOR : WHITE_ON_BLACK;
                write_string((SCREEN_WIDTH - 30) / 2 + 2, 7 + i, options[i], color);
            }

            write_string(5, 15, "                        Press Enter to select.", WHITE_ON_BLUE);
            
            screen_updated = true;
        }

        if (key == 0x48 && selected_option > 0) {  
            selected_option--;
            screen_updated = false;  
        } else if (key == 0x50 && selected_option < 1) {  
            selected_option++;
            screen_updated = false;  
        }
    }
}

void kernel_main() {
    system_init();  
    gui_main();
}
