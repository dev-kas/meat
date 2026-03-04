#ifndef _KERNEL_TSS_H
#define _KERNEL_TSS_H

#include <stdint.h>

struct tss_entry_struct {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iopb_offset;
} __attribute__((packed));

typedef struct tss_entry_struct tss_entry_t;

void tss_set_stack(uint16_t kernel_ss, uint64_t kernel_rsp);
void tss_install(int gdt_idx, uint16_t kernel_ss, uint64_t kernel_rsp);

#endif
