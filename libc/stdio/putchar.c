#include <stdio.h>
#include <stdint.h>

#if defined(__is_libk)
// #include <kernel/tty.h>
#include <kernel/serial.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	// terminal_write(&c, sizeof(c));
	serial_putchar(c);
#else
	asm volatile(
		"int $0x80"
		:
		: "a"((uint64_t)1), "b"((uint64_t)ic), "c"(0), "d"(0)
		: "memory"
	);
#endif
	return ic;
}
