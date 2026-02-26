#ifndef _KERNEL_SERIAL_H
#define _KERNEL_SERIAL_H

#include <stddef.h>

int serial_initialize(void);
void serial_putchar(char c);
void serial_write(const char* data, size_t size);

#endif
