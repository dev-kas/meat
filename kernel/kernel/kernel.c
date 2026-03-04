#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

/*
#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/serial.h>
#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/multiboot.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/kheap.h>
#include <kernel/fs.h>
#include <kernel/process.h>
#include <kernel/kex.h>

#include <string.h>
*/

// base revision = 3
__attribute__((used, section(".requests")))
static volatile LIMINE_BASE_REVISION(3);

// request framebuffer
__attribute__((used, section(".requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
	.id = LIMINE_FRAMEBUFFER_REQUEST,
	.revision = 0
};

__attribute__((section(".text")))
void _start(void) {
	if (LIMINE_BASE_REVISION_SUPPORTED == false) {
		for (;;) asm("hlt");
	}

	if (framebuffer_request.response == NULL ||
		framebuffer_request.response->framebuffer_count < 1) {
		for (;;) asm("hlt");
	}

	// get first framebuffer
	struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

	// get properties of the screen
	uint32_t width = framebuffer->width;
	uint32_t height = framebuffer->height;
	uint32_t pitch = framebuffer->pitch; // bytes per row
	uint8_t* fb_ptr = framebuffer->address;

	uint32_t x_center = width / 2;
	uint32_t y_center = height / 2;

	for (uint32_t y = y_center - 50; y < y_center + 50; y++) {
		for (uint32_t x = x_center - 50; x < x_center + 50; x++) {
			// calculate memory address of pixel (x, y)
			// each pixel is 4 bytes (32bit color: blue, green, red, reserved)
			uint32_t pixel_offset = (y * pitch) + (x * 4);

			fb_ptr[pixel_offset + 0] = 255;
			fb_ptr[pixel_offset + 1] = 0;
			fb_ptr[pixel_offset + 2] = 0;
		}
	}

	for (;;) asm volatile ("hlt");
}

/*
extern uint32_t _kernel_end;

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

	// check if grub loaded the module
	if (mboot_ptr->mods_count > 0) {
		uint32_t mod_addr = mboot_ptr->mods_addr;
		multiboot_module_t* module = (multiboot_module_t*)mod_addr;

		uint32_t start = module->mod_start;
		uint32_t end = module->mod_end;
		uint32_t len = end - start;

		printf("Module found at 0x%x (size = %d bytes)\n", start, len);

		fs_init(start);
		fs_cat("message.txt");
	}

	// calculate total RAM size from multiboot
	uint32_t mem_size = (mboot_ptr->mem_upper + 1024) * 1024;

	pmm_initialize(mem_size, (uint32_t)&_kernel_end);

	// parse multiboot to unlock valid RAM
	multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mboot_ptr->mmap_addr;
	while ((uint32_t)mmap < mboot_ptr->mmap_addr + mboot_ptr->mmap_length) {
		if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
			pmm_init_region(mmap->addr_low, mmap->len_low);
		}
		mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(unsigned int));
	}

	// lock the kernel
	uint32_t bitmap_size = (mem_size / 4096) / 8;
	pmm_deinit_region(0x100000, ((uint32_t)&_kernel_end - 0x100000) + bitmap_size);

	printf("PMM initialized.\n");

	vmm_initialize();

	// for 1MB heap (256 pages)
	uint32_t heap_start = 0xD0000000;
	uint32_t heap_end = heap_start + (1024 * 1024);

	for (uint32_t i = heap_start; i < heap_end; i += 4096) {
		uint32_t phys = pmm_alloc_block();
		vmm_map_page(phys, i, 3);
	}

	kheap_initialize((void*)heap_start, 256 * 4096);
	multitasking_initialize();

	asm volatile("sti");

	uint32_t file_size;
	void* hello_kex = fs_get_file("hello.kex", &file_size);

	if (hello_kex) {
		terminal_initialize(); // clear the screen
		load_kex_and_run(hello_kex);
	} else {
		printf("Could not find hello.kex in ramdisk");
	}

	while(1);
}
*/
