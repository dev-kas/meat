#ifndef _KERNEL_IDT_H
#define _KERNEL_IDT_H

#include <stdint.h>

struct idt_entry {
	uint16_t base_low; // lower 16 bits of the addr to jump to
	uint16_t sel; // kernel segment selector (0x08, gdt kernel code)
	uint8_t  always0; // this must always be zero
	uint8_t  flags; // flags that tell the CPU this is an interrupt handler
	uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void idt_initialize(void);

#endif
