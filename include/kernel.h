#pragma once

#include "types.h"

# define VGA_WIDTH	80
# define VGA_HEIGHT	25

enum vga_color {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHT_GREY = 7,
	DARK_GREY = 8,
	LIGHT_BLUE = 9,
	LIGHT_GREEN = 10,
	LIGHT_CYAN = 11,
	LIGHT_RED = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN = 14,
	WHITE = 15,
};

struct term {
	uint32_t row;
	uint32_t col;
	uint8_t	 color;
};

void	terminal_putchar(char c);
void	terminal_write(const char* str);
