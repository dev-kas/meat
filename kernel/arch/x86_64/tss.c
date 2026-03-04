#include <kernel/tss.h>
#include <kernel/gdt.h>
#include <string.h>

tss_entry_t tss_entry;
extern struct gdt_entry gdt[];

extern void gdt_set_gate(int num, uint64_t base, uint32_t limit, uint8_t access, uint8_t gran);
extern void tss_flush();

void tss_install(int gdt_idx, uint16_t kernel_ss, uint64_t kernel_rsp) {
	(void)kernel_ss; // not used in 64-bit mode
	uint64_t base = (uint64_t) &tss_entry;
	uint32_t limit = sizeof(tss_entry);

	// lower 8 bytes of the TSS descriptor
	gdt_set_gate(gdt_idx, base, limit, 0x89, 0x00);

	// upper 8 bytes of the TSS descriptor into the next GDT entry
	uint64_t base_upper = (base >> 32);
	gdt[gdt_idx + 1].limit_low = base_upper & 0xFFFF;
	gdt[gdt_idx + 1].base_low = (base_upper >> 16) & 0xFFFF;
	gdt[gdt_idx + 1].base_middle = 0;
	gdt[gdt_idx + 1].access = 0;
	gdt[gdt_idx + 1].granularity = 0;
	gdt[gdt_idx + 1].base_high = 0;

	memset(&tss_entry, 0, sizeof(tss_entry));
	tss_entry.rsp0 = kernel_rsp;
	tss_entry.iopb_offset = sizeof(tss_entry);

	tss_flush(); 
}

void tss_set_stack(uint16_t kernel_ss, uint64_t kernel_rsp) {
	(void)kernel_ss;
	tss_entry.rsp0 = kernel_rsp;
}
