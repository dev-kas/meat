.section .init
.global _init
.type _init, @function
_init:
	push %rbp
	mov %rsp, %rbp
	/* gcc will put the contents of crtbegin.o's .init section here */

.section .fini
.global _fini
.type _fini, @function
_fini:
	push %rbp
	mov %rsp, %rbp
	/* gcc will put crtbegin.o's .fini section here */

