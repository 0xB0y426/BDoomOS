#ifndef KERNEL_LOG_H
#define KERNEL_LOG_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void log_message(const char *msg);

void system_init();

#define SYSTEM_INIT_MESSAGES() { \
    log_message("GRUB Loaded BDoomOS.iso"); \
    log_message("Booting Kernel..."); \
    log_message("Initing kernel..."); \
    log_message("Loaded!!"); \
    log_message("BDoomOS 0.1 ready."); \
}

#endif 
