#include <stdio.h>
#include <kernel/process.h>
#include <kernel/tty.h>
#include <kernel/serial.h>

void syscall_handler(struct registers* regs) {
	switch (regs->rax) {
	case 1: // putchar
		char c = (char)regs->rbx;
		terminal_putchar(c);
		break;
	case 2: // exit / halt
		printf("\nUser Process Exited.\n");
		while(1) { asm volatile("cli; hlt"); }
		break;
	default:
		printf("Unknown syscall %d (0x%x)\n", regs->rax, regs->rax);
		break;
	}
}
