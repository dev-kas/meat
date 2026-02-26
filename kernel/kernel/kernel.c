#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/multiboot.h>

void kernel_main(uint32_t magic, multiboot_info_t* mboot_ptr) {
	gdt_initialize();
	idt_initialize();
	terminal_initialize();
	serial_initialize();
	pic_remap();

	printf("Boot Magic: 0x%x\n", magic);
	if (magic != 0x2badb002) {
		printf("Error: Invalid Multiboot Magic Number.\n");
		return;
	}

	// check bit 6 to see if we have a valid mmap
	if (!(mboot_ptr->flags & MULTIBOOT_FLAG_MMAP)) {
		printf("Error: No memory map provided by GRUB!\n");
		return;
	}

	printf("Memory Map:\n");

	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mboot_ptr->mmap_addr;
	while((uint32_t)mmap < mboot_ptr->mmap_addr + mboot_ptr->mmap_length) {
		// only print available RAM
		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			printf("  Base: 0x%x | Length: 0x%x (Free RAM)\n", 
				mmap->addr_low, mmap->len_low);
		} else {
			printf("  Base: 0x%x | Length: 0x%x (Reserved)\n", 
				mmap->addr_low, mmap->len_low);
		}

		// advance to next entry
		mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
	}

	asm volatile("sti");

	printf("Hello, Meat world!\n");

	while (1) { asm volatile("hlt"); }
}
