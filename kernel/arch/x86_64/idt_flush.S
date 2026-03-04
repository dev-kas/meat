.global idt_flush
idt_flush:
	lidt (%rdi)
	ret

.macro pusha64
	pushq %r15
	pushq %r14
	pushq %r13
	pushq %r12
	pushq %r11
	pushq %r10
	pushq %r9
	pushq %r8
	pushq %rax
	pushq %rcx
	pushq %rdx
	pushq %rbx
	pushq %rbp
	pushq %rsi
	pushq %rdi
.endm

.macro popa64
	popq %rdi
	popq %rsi
	popq %rbp
	popq %rbx
	popq %rdx
	popq %rcx
	popq %rax
	popq %r8
	popq %r9
	popq %r10
	popq %r11
	popq %r12
	popq %r13
	popq %r14
	popq %r15
.endm

.global isr0
.extern isr0_handler
isr0:
	cli
	pusha64
	mov %rsp, %rbx
	and $-16, %rsp
	call isr0_handler
	mov %rbx, %rsp
	popa64
	sti
	iretq

.global isr13
.extern isr13_handler
isr13:
	cli
	pusha64
	mov %rsp, %rbx
	and $-16, %rsp
	call isr13_handler
	mov %rbx, %rsp
	popa64
	addq $8, %rsp
	sti
	iretq

.global isr14
.extern isr14_handler
isr14:
	cli
	pusha64
	mov %rsp, %rbx
	and $-16, %rsp
	call isr14_handler
	mov %rbx, %rsp
	popa64
	addq $8, %rsp
	sti
	iretq

.global isr32
.extern irq0_handler
isr32:
	cli
	pusha64
	mov %rsp, %rdi
	mov %rsp, %rbx
	and $-16, %rsp
	call irq0_handler
	mov %rax, %rsp
	popa64
	sti
	iretq

.global isr33
.extern isr33_handler
isr33:
	cli
	pusha64
	mov %rsp, %rbx
	and $-16, %rsp
	call isr33_handler
	mov %rbx, %rsp
	popa64
	sti
	iretq

.global isr128
.extern syscall_handler
isr128:
	cli
	pusha64
	mov %rsp, %rdi
	mov %rsp, %rbx
	and $-16, %rsp
	call syscall_handler
	mov %rbx, %rsp
	popa64
	sti
	iretq
