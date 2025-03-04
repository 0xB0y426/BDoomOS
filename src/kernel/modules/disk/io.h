#ifndef ATA_H
#define ATA_H

#include <stdint.h>
#include "../func.h"

#define ATA_PRIMARY_PORT_BASE 0x1F0
#define ATA_STATUS_REGISTER (ATA_PRIMARY_PORT_BASE + 0x07)
#define ATA_COMMAND_REGISTER (ATA_PRIMARY_PORT_BASE + 0x07)
#define ATA_DATA_REGISTER (ATA_PRIMARY_PORT_BASE)
#define ATA_SECTOR_COUNT_REGISTER (ATA_PRIMARY_PORT_BASE + 2)
#define ATA_LBA_LOW_REGISTER (ATA_PRIMARY_PORT_BASE + 3)
#define ATA_LBA_MID_REGISTER (ATA_PRIMARY_PORT_BASE + 4)
#define ATA_LBA_HIGH_REGISTER (ATA_PRIMARY_PORT_BASE + 5)
#define ATA_DRIVE_REGISTER (ATA_PRIMARY_PORT_BASE + 6)
#define ATA_DRIVE_COMMAND 0xE0
#define ATA_READ_COMMAND 0x20
#define ATA_WRITE_COMMAND 0x30
#define SECTOR_SIZE 512

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t ret;
    __asm__ volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outw(uint16_t port, uint16_t value) {
    __asm__ volatile ("outw %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void wait_for_ata() {
    while (inb(ATA_STATUS_REGISTER) & 0x80);
}

// Função de leitura de dados de um setor
void ata_read(uint32_t lba, uint16_t sector_count, char *buffer) {
    wait_for_ata();
    outb(ATA_DRIVE_REGISTER, ATA_DRIVE_COMMAND);
    outb(ATA_SECTOR_COUNT_REGISTER, sector_count);
    outb(ATA_LBA_LOW_REGISTER, (lba & 0xFF));
    outb(ATA_LBA_MID_REGISTER, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH_REGISTER, (lba >> 16) & 0xFF);
    outb(ATA_COMMAND_REGISTER, ATA_READ_COMMAND);

    wait_for_ata();

    for (int i = 0; i < sector_count; i++) {
        wait_for_ata();
        for (int j = 0; j < 256; j++) {
            uint16_t data = inw(ATA_DATA_REGISTER);
            buffer[j * 2] = (char)(data & 0xFF);
            if (j * 2 + 1 < SECTOR_SIZE) {
                buffer[j * 2 + 1] = (char)((data >> 8) & 0xFF);
            }
        }
    }
    buffer[sector_count * SECTOR_SIZE - 1] = '\0';
}

int ata_write(uint32_t sector_number, uint32_t sector_count, char *buffer) {
    wait_for_ata();
    outb(ATA_DRIVE_REGISTER, ATA_DRIVE_COMMAND);
    outb(ATA_SECTOR_COUNT_REGISTER, sector_count);
    outb(ATA_LBA_LOW_REGISTER, (sector_number & 0xFF));
    outb(ATA_LBA_MID_REGISTER, (sector_number >> 8) & 0xFF);
    outb(ATA_LBA_HIGH_REGISTER, (sector_number >> 16) & 0xFF);
    outb(ATA_COMMAND_REGISTER, ATA_WRITE_COMMAND);

    int bytes_written = 0;
    for (int i = 0; i < sector_count; i++) {
        wait_for_ata();
        for (int j = 0; j < 256; j++) {
            uint16_t data = buffer[j * 2] | (buffer[j * 2 + 1] << 8);
            outw(ATA_DATA_REGISTER, data);
            bytes_written += 2;
        }
    }
    return bytes_written; 
}


#endif // ATA_H
