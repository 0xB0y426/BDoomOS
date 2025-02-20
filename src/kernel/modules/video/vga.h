#ifndef VIDEO_CONFIG_H
#define VIDEO_CONFIG_H

#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define WHITE_ON_BLUE 0x1F
#define SELECTED_COLOR 0x70
#define HIGHLIGHT_COLOR 0x1F
#define INFO_COLOR 0x0F
#define WHITE_ON_BLACK 0x0F
#define WHITE_ON_WHITE 0x1F
#define YELLOW_ON_BLACK 0x0E
#define GREEN_ON_BLACK 0x0A
#define BROWN_ON_BLACK 0x0C
#define LIGHT_BLUE_ON_BLACK 0x1B
#define LIGHT_GREEN_ON_BLACK 0x0A

volatile char *video = (volatile char *)VIDEO_MEMORY;

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

void clear_screen_with_color(char color) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            write_char(x, y, ' ', color);
        }
    }
}

void clear_area(int start_x, int start_y, int width, int height, char color) {
    for (int y = start_y; y < start_y + height; y++) {
        for (int x = start_x; x < start_x + width; x++) {
            write_char(x, y, ' ', color);
        }
    }
}

void draw_window(int start_x, int start_y, int width, int height, const char* title, char color) {
    clear_area(start_x, start_y, width, height, color);
    
    for (int x = start_x; x < start_x + width; x++) {
        write_char(x, start_y, '-', HIGHLIGHT_COLOR); 
        write_char(x, start_y + height - 1, '-', HIGHLIGHT_COLOR); 
    }

    for (int y = start_y; y < start_y + height; y++) {
        write_char(start_x, y, '|', HIGHLIGHT_COLOR); 
        write_char(start_x + width - 1, y, '|', HIGHLIGHT_COLOR);  
    }

    write_string(start_x + 1, start_y, title, HIGHLIGHT_COLOR);
}

#endif
