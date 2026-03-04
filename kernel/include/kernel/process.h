#ifndef _KERNEL_PROCESS_H
#define _KERNEL_PROCESS_H

#include <stdint.h>

struct registers {
	uint64_t rdi, rsi, rbp, rbx, rdx, rcx, rax;
	uint64_t r8, r9, r10, r11, r12, r13, r14, r15;
	uint64_t rip, cs, rflags, rsp, ss;
};

typedef struct process {
	int pid; // process id
	uint64_t rsp; // stack pointer (where the process was paused)
	uint64_t* page_directory; // virtual memory context (CR3)
	struct process* next;
} process_t;

void multitasking_initialize();
void create_kernel_thread(void (*function)(void));
void switch_task(struct registers* regs); // scheduler

#endif
