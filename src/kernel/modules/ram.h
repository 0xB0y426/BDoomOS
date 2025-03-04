#ifndef RAM_H
#define RAM_H 

#include <stddef.h>
#include <stdint.h>
#include "video/vga.h"

#define INPUT_BUFFER_SIZE 256
#define RAM_SIZE (1024 * 1024)
#define PAGE_SIZE 4096

extern size_t input_index;

char input_buffer[INPUT_BUFFER_SIZE];
size_t input_index = 0;

typedef struct {
    void *start;
    size_t size;
} MemoryBlock;

uint8_t ram[RAM_SIZE];
MemoryBlock allocated_blocks[100];
size_t allocated_count = 0;

void init_ram() {
    for (size_t i = 0; i < RAM_SIZE; ++i) {
        ram[i] = 0;
    }
    allocated_count = 0;
}

void* allocate_memory(size_t size) {
    if (allocated_count < 100 && size > 0) {
        void* ptr = NULL;
        for (size_t i = 0; i < RAM_SIZE; i += PAGE_SIZE) {
            if (!ram[i]) {
                ptr = &ram[i];
                allocated_blocks[allocated_count++] = (MemoryBlock){ptr, size};
                for (size_t j = 0; j < size; j++) {
                    ram[i + j] = 1;
                }
                return ptr;
            }
        }
    }
    return NULL;
}

void free_memory(void* ptr) {
    for (size_t i = 0; i < allocated_count; ++i) {
        if (allocated_blocks[i].start == ptr) {
            for (size_t j = 0; j < allocated_blocks[i].size; j++) {
                ram[(uintptr_t)ptr - (uintptr_t)ram + j] = 0;
            }
            for (size_t j = i; j < allocated_count - 1; ++j) {
                allocated_blocks[j] = allocated_blocks[j + 1];
            }
            allocated_count--;
            return;
        }
    }
}

size_t get_total_memory() {
    return RAM_SIZE;
}

size_t get_free_memory() {
    size_t used_memory = 0;
    for (size_t i = 0; i < allocated_count; ++i) {
        used_memory += allocated_blocks[i].size;
    }
    return RAM_SIZE - used_memory;
}

void display_memory_info() {
    size_t total_memory = get_total_memory();
    size_t free_memory = get_free_memory();
    
    write_string(7, 9, "Memory Info:", WHITE_ON_BLACK);
    write_string(7, 10, "Total RAM: ", WHITE_ON_BLACK);
    write_number(7, 11, total_memory / 1024, WHITE_ON_BLACK);
    write_string(7, 12, " KB", WHITE_ON_BLACK);

    write_string(7, 13, "Free RAM: ", WHITE_ON_BLACK);
    write_number(7, 14, free_memory / 1024, WHITE_ON_BLACK);
    write_string(7, 15, " KB", WHITE_ON_BLACK);
}

#endif // RAM_H
