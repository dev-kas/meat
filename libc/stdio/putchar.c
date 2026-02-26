#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#include <kernel/serial.h>
#endif

int putchar(int ic) {
#if defined(__is_libk)
	char c = (char) ic;
	terminal_write(&c, sizeof(c));
	serial_putchar(c);
#else
	// TODO: implement stdio and the write syscall
#endif
	return ic;
}
