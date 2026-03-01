.global switch_to_user_mode
switch_to_user_mode:
	cli

	# gdt entry 4 is 0x20, so 0x20 | 3 (ring 3) = 0x23
	mov $0x23, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs

	mov %esp, %eax # save current esp into eax

	pushl $0x23 # ss (user data)
	pushl %eax # esp (user stack)
	pushf # eflags

	popl %eax
	orl $0x200, %eax
	pushl %eax

	pushl $0x1B # push cs (user code segment)
	pushl $1f # eip (return addr)
	iret # jump to user mode

1:
	# we are now in ring 3 aka user mode!!
	ret
