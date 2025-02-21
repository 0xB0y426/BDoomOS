#ifndef CPUINFO_H
#define CPUINFO_H

#include <stdint.h>
#include "video/vga.h"

static inline void cpuid(uint32_t code, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
    __asm__ volatile("cpuid"
                     : "=a"(*a), "=b"(*b), "=c"(*c), "=d"(*d)
                     : "a"(code));
}

void display_cpu_info() {
    uint32_t eax, ebx, ecx, edx;
    char vendor[13];

    // Obter o vendedor da CPU
    cpuid(0, &eax, &ebx, &ecx, &edx);
    ((uint32_t *)vendor)[0] = ebx;
    ((uint32_t *)vendor)[1] = edx;
    ((uint32_t *)vendor)[2] = ecx;
    vendor[12] = '\0';

    write_string(7, 9, "CPU Vendor: ", WHITE_ON_BLACK);
    write_string(20, 9, vendor, WHITE_ON_BLACK);

    cpuid(1, &eax, &ebx, &ecx, &edx);
    int family = ((eax >> 8) & 0xF) + ((eax >> 20) & 0xFF);
    int model = ((eax >> 4) & 0xF) + ((eax >> 12) & 0xF0);
    int stepping = eax & 0xF;

    write_string(7, 10, "Family: ", WHITE_ON_BLACK);
    write_number(15, 10, family, WHITE_ON_BLACK);
    write_string(7, 11, "Model: ", WHITE_ON_BLACK);
    write_number(15, 11, model, WHITE_ON_BLACK);
    write_string(7, 12, "Stepping: ", WHITE_ON_BLACK);
    write_number(15, 12, stepping, WHITE_ON_BLACK);

    cpuid(4, &eax, &ebx, &ecx, &edx);
    int cores = ((eax >> 26) & 0x3F) + 1;

    write_string(7, 13, "Cores: ", WHITE_ON_BLACK);
    write_number(15, 13, cores, WHITE_ON_BLACK);

    cpuid(2, &eax, &ebx, &ecx, &edx);
    int cacheL1 = (eax >> 16) & 0xFF;
    int cacheL2 = (eax >> 24) & 0xFF;
    int cacheL3 = (ebx >> 18) & 0x3FFF;

    write_string(7, 14, "Cache L1: ", WHITE_ON_BLACK);
    write_number(17, 14, cacheL1, WHITE_ON_BLACK);
    write_string(7, 15, "Cache L2: ", WHITE_ON_BLACK);
    write_number(17, 15, cacheL2, WHITE_ON_BLACK);
    write_string(7, 16, "Cache L3: ", WHITE_ON_BLACK);
    write_number(17, 16, cacheL3, WHITE_ON_BLACK);

    cpuid(1, &eax, &ebx, &ecx, &edx);
    
    write_string(7, 17, "SSE Support: ", WHITE_ON_BLACK);
    write_string(20, 17, (edx & (1 << 25)) ? "Yes" : "No", WHITE_ON_BLACK);
    
    write_string(7, 18, "SSE2 Support: ", WHITE_ON_BLACK);
    write_string(20, 18, (edx & (1 << 26)) ? "Yes" : "No", WHITE_ON_BLACK);
    
    write_string(7, 19, "SSE3 Support: ", WHITE_ON_BLACK);
    write_string(20, 19, (ecx & 1) ? "Yes" : "No", WHITE_ON_BLACK);
    
    write_string(7, 20, "AVX Support: ", WHITE_ON_BLACK);
    write_string(20, 20, (ecx & (1 << 28)) ? "Yes" : "No", WHITE_ON_BLACK);
}

#endif
