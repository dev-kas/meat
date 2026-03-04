.global gdt_flush

gdt_flush:
	# get the pointer to the gdt passed from the c code
	movl 4(%esp), %eax

	# load gdt into cpu
	lgdt (%eax)

	# tell cpu's data registers to use rule 2 (kernel data)
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	movw %ax, %ss

	# tell cpu's code register to use rule 1 (kernel code)
	ljmp $0x08, $.flush

.flush:
	ret

