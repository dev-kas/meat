#ifndef _KERNEL_GDT_H
#define _KERNEL_GDT_H

#include <stdint.h>

struct gdt_entry {
	uint16_t limit_low; // lower 16 bits of the limit
	uint16_t base_low; // lower 16 bits of the base
	uint8_t base_middle; // next 8 bits of the base
	uint8_t access; // access flag used to determine what ring this segment can be used in
	uint8_t granularity; // granularity and the remaining 4 bits of the limit
	uint8_t base_high; // last 8 bits of the base
} __attribute__((packed));

struct gdt_ptr {
	uint16_t limit; // upper 16 bits of all selector limits
	uint32_t base; // address of first gdt_entry_t struct
} __attribute__((packed));

void gdt_initialize(void);

#endif
