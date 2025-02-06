#include "printk.h"
#include "kernel.h"

static void print_uint32_hex(uint32_t val) {
	char buf[9];
	buf[8] = '\0';
	for (int i = 7; i >= 0; i--) {
		uint8_t nibble = val & 0xF;
		buf[i] = (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));
		val >>= 4;
	}
	terminal_write(buf);
}

static void print_int(int val) {
	char buf[32];
	int idx = 0;
	if (val == 0) {
		terminal_putchar('0');
		return;
	}
	if (val < 0) {
		terminal_putchar('-');
		val = -val;
	}
	while (val > 0) {
		buf[idx++] = (char)('0' + (val % 10));
		val /= 10;
	}
	for (int i = idx - 1; i >= 0; i--) {
		terminal_putchar(buf[i]);
	}
}

void printk(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);

	while (*fmt) {
		if (*fmt == '%' && *(fmt+1) != '\0') {
			fmt++;
			switch (*fmt) {
				case 's': {
					const char* s = va_arg(args, const char*);
					terminal_write(s);
					break;
				}
				case 'x': {
					uint32_t val = va_arg(args, uint32_t);
					print_uint32_hex(val);
					break;
				}
				case 'd': {
					int val = va_arg(args, int);
					print_int(val);
					break;
				}
				case 'c': {
					int c = va_arg(args, int);
					terminal_putchar((char)c);
					break;
				}
				default:
					terminal_putchar('%');
					terminal_putchar(*fmt);
					break;
			}
		} else {
			terminal_putchar(*fmt);
		}
		fmt++;
	}

	va_end(args);
}
