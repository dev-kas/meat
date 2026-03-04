#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/kex.h>

extern void switch_to_user_mode(uint64_t entry_point);

void load_kex_and_run(void* file_data) {
	kex_header* hdr = (kex_header*)file_data;

	// verify magic number
	if (hdr->magic[0] != 'K' || hdr->magic[1] != 'E' ||
		hdr->magic[2] != 'X' || hdr->magic[3] != '\0') {
		printf("Error: Invalid KEX magic!\n");
		return;
	}

	// parse commands
	uint64_t current_offset = hdr->cmd_offset;

	for (uint64_t i = 0; i < hdr->command_count; i++) {
		kex_cmd* cmd = (kex_cmd*)((uint64_t)file_data + current_offset);

		if (cmd->type == KEX_CMD_SEGMENT) {
			kex_segment_cmd* seg = (kex_segment_cmd*)cmd;

			for (uint64_t offset = 0; offset < seg->mem_size; offset += 4096) {
				uint64_t phys = pmm_alloc_block();
				vmm_map_page(phys, seg->rva + offset, 0x7);
			}

			// zero-initialize the memory
			memset((void*)(uint64_t)seg->rva, 0, seg->mem_size);

			// copy the data from the KEX file into the RAM
			if (seg->file_size > 0) {
				memcpy((void*)(uint64_t)seg->rva,
					(void*)((uint64_t)file_data + (uint64_t)seg->file_offset),
					seg->file_size);
			}
		} else if (cmd->type == KEX_CMD_INTEGRITY) {
			// TODO: implement sha256 verification then verify
		}

		// move to next cmd based on the size defined in the header
		current_offset += cmd->size;
	}

	uint64_t stack_phys = pmm_alloc_block();
	vmm_map_page(stack_phys, 0x7FF000, 0x7);

	switch_to_user_mode((uint64_t)hdr->entry_rva);
}
