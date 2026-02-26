#ifndef _KERNEL_VMM_H
#define _KERNEL_VMM_H

#include <stdint.h>

void vmm_initialize();
void vmm_map_page(uint32_t phys, uint32_t virt);

#endif
