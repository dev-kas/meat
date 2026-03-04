#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/serial.h>

#define KEYBOARD_BUFFER_SIZE 256

char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
uint8_t write_ptr = 0;
uint8_t read_ptr = 0;

void terminal_initialize(void) { }

void terminal_putchar(char c) {
	serial_putchar(c);
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++) {
		serial_putchar(data[i]);
	}
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

void keyboard_push(char c) {
	uint8_t next_write = (write_ptr + 1) % KEYBOARD_BUFFER_SIZE;
	if (next_write == read_ptr) return;
	keyboard_buffer[write_ptr] = c;
	write_ptr = next_write;
}

char keyboard_getchar() {
	while (read_ptr == write_ptr) {
		asm volatile("hlt");
	}
	char c = keyboard_buffer[read_ptr];
	read_ptr = (read_ptr + 1) % KEYBOARD_BUFFER_SIZE;
	return c;
}
