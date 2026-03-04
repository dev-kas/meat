#ifndef _KERNEL_VMM_H
#define _KERNEL_VMM_H

#include <stdint.h>

uint64_t* get_active_page_dir();

void vmm_initialize();
void vmm_map_page(uint64_t phys, uint64_t virt, uint64_t flags);

#endif
