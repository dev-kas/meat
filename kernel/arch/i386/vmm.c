#include <stdint.h>
#include <stdio.h>

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

extern void load_page_directory(uint32_t*);
extern void enable_paging();

void vmm_initialize() {
	// clear the directory (mark everything as not present)
	// bit 1 = read/write, 0 = present. 0x2 means rw but not present
	for (int i = 0; i < 1024; i++) {
		page_directory[i] = 0x00000002;
	}

	// fill the first page table (identity map 0MB to 4MB)
	for (unsigned int i = 0; i < 1024; i++) {
		first_page_table[i] = (i * 4096) | 3;
	}

	// put the page table into the directory
	page_directory[0] = ((unsigned int)first_page_table) | 3;

	load_page_directory(page_directory);
	enable_paging();

	printf("VMM: Paging enabled!\n");
}
