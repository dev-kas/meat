#ifndef _KERNEL_PMM_H
#define _KERNEL_PMM_H

#include <stdint.h>
#include <stddef.h>

void pmm_initialize(uint64_t mem_upper_limit, uint64_t kernel_end);
uint64_t pmm_alloc_block();
void pmm_free_block(uint64_t block_addr);
void pmm_deinit_region(uint64_t base, uint64_t size);
void pmm_init_region(uint64_t base, uint64_t size);

#endif
