#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <kernel/pmm.h>
#include <kernel/vmm.h>

extern uint64_t hhdm_offset;

static inline void vmm_flush_tlb_entry(uint64_t addr) {
    asm volatile("invlpg %0" : : "m"(*(volatile char*)addr) : "memory");
}

uint64_t* get_active_page_dir() {
    uint64_t cr3;
    asm volatile("mov %%cr3, %0" : "=r"(cr3));
    return (uint64_t*)(cr3 & 0x000FFFFFFFFFF000);
}

void vmm_initialize() {
    uint64_t* old_pml4 = (uint64_t*)((uint64_t)get_active_page_dir() + hhdm_offset);
    uint64_t new_pml4_phys = pmm_alloc_block();
    uint64_t* new_pml4 = (uint64_t*)(new_pml4_phys + hhdm_offset);
    for (int i = 0; i < 512; i++) new_pml4[i] = 0;
    
    for (int i = 256; i < 512; i++) {
        new_pml4[i] = old_pml4[i];
    }
    
    asm volatile("mov %0, %%cr3" : : "r"(new_pml4_phys));
    
    printf("VMM: Switched to clean 64-bit page table!\n");
}

void vmm_map_page(uint64_t phys, uint64_t virt, uint64_t flags) {
    uint64_t* pml4 = (uint64_t*)((uint64_t)get_active_page_dir() + hhdm_offset);
    
    uint64_t pml4_idx = (virt >> 39) & 0x1FF;
    uint64_t pdpt_idx = (virt >> 30) & 0x1FF;
    uint64_t pd_idx   = (virt >> 21) & 0x1FF;
    uint64_t pt_idx   = (virt >> 12) & 0x1FF;

    if (!(pml4[pml4_idx] & 0x1)) {
        uint64_t new_pdpt = pmm_alloc_block();
        memset((void*)(new_pdpt + hhdm_offset), 0, 4096);
        pml4[pml4_idx] = new_pdpt | 0x7;
    }

    uint64_t* pdpt = (uint64_t*)((pml4[pml4_idx] & 0x000FFFFFFFFFF000) + hhdm_offset);
    if (!(pdpt[pdpt_idx] & 0x1)) {
        uint64_t new_pd = pmm_alloc_block();
        memset((void*)(new_pd + hhdm_offset), 0, 4096);
        pdpt[pdpt_idx] = new_pd | 0x7;
    }

    uint64_t* pd = (uint64_t*)((pdpt[pdpt_idx] & 0x000FFFFFFFFFF000) + hhdm_offset);
    if (!(pd[pd_idx] & 0x1)) {
        uint64_t new_pt = pmm_alloc_block();
        memset((void*)(new_pt + hhdm_offset), 0, 4096);
        pd[pd_idx] = new_pt | 0x7;
    }

    uint64_t* pt = (uint64_t*)((pd[pd_idx] & 0x000FFFFFFFFFF000) + hhdm_offset);
    pt[pt_idx] = (phys & 0x000FFFFFFFFFF000) | (flags & 0xFFF) | 0x1;

    vmm_flush_tlb_entry(virt);
}
