.global switch_to_user_mode
switch_to_user_mode:
	cli

	mov %rdi, %rbx

	# gdt entry 4 is 0x20, so 0x20 | 3 (ring 3) = 0x23
	mov $0x23, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	# construct iretq frame
	pushq $0x23        # SS (user data)
	pushq $0x800000    # RSP (user stack)
	
	pushfq
	popq %rax
	orq $0x200, %rax
	pushq %rax         # RFLAGS
	
	pushq $0x1B        # CS (user code segment: 0x18 | 3 = 0x1B)
	pushq %rbx         # RIP
	
	iretq              # jump to user mode
