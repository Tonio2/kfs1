#include "../include/stdint.h"
#include "../include/stddef.h"
#include "../include/io.h"

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    // Cursor Start Register (0x0A)
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    // Cursor End Register (0x0B)
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

// static void disable_cursor(void) {
//     outb(0x3D4, 0x0A);
//     outb(0x3D5, 0x20);
// }

static void update_cursor(int x, int y) {
    uint16_t pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Scrolls the screen up by 1 line
static void terminal_scroll(void) {
    // Move each line up
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = 
                terminal_buffer[(y + 1) * VGA_WIDTH + x];
        }
    }
    // Clear the last line
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            vga_entry(' ', terminal_color);
    }
}

// Clears the entire screen
static void terminal_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[y * VGA_WIDTH + x] = 
                vga_entry(' ', terminal_color);
        }
    }
}

void terminal_initialize(void) {
    terminal_row    = 0;
    terminal_column = 0;
    terminal_color  = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*)0xB8000;

    // Clear screen
    terminal_clear();

    // Enable & set the cursor
    enable_cursor(0, 15);
    update_cursor(terminal_column, terminal_row);
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                terminal_scroll();
                terminal_row = VGA_HEIGHT - 1;
            }
        }
    }
    update_cursor(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void kernel_main(void) 
{
	terminal_initialize();

    
	terminal_writestring("Hello, kernel World!1\n");
    terminal_writestring("Hello, kernel World!1\n");
    terminal_writestring("Hello, kernel World!1\n");
    terminal_writestring("Hello, kernel World!1\n");
    terminal_writestring("Hello, kernel World!1\n");
    terminal_writestring("Hello, kernel World!2\n");
    terminal_writestring("Hello, kernel World!2\n");
    terminal_writestring("Hello, kernel World!2\n");
    terminal_writestring("Hello, kernel World!2\n");
    terminal_writestring("Hello, kernel World!2\n");
    terminal_writestring("Hello, kernel World!3\n");
    terminal_writestring("Hello, kernel World!3\n");
    terminal_writestring("Hello, kernel World!3\n");
    terminal_writestring("Hello, kernel World!3\n");
    terminal_writestring("Hello, kernel World!3\n");
    terminal_writestring("Hello, kernel World!4\n");
    terminal_writestring("Hello, kernel World!4\n");
    terminal_writestring("Hello, kernel World!4\n");
    terminal_writestring("Hello, kernel World!4\n");
    terminal_writestring("Hello, kernel World!4\n");
    terminal_writestring("Hello, kernel World!5\n");
    terminal_writestring("Hello, kernel World!5\n");
    terminal_writestring("Hello, kernel World!5\n");
    terminal_writestring("Hello, kernel World!5\n");
    terminal_writestring("Hello, kernel World!5\n");
    terminal_writestring("Hello, kernel World!6\n");
    terminal_writestring("Hello, kernel World!6\n");
    terminal_writestring("Hello, kernel World!6\n");
    terminal_writestring("Hello, kernel World!6\n");
    terminal_writestring("Hello, kernel World!6\n");
    terminal_writestring("Hello, kernel World!7\n");
    terminal_writestring("Hello, kernel World!7\n");
    terminal_writestring("Hello, kernel World!7\n");
    terminal_writestring("Hello, kernel World!7\n");
    terminal_writestring("Hello, kernel World!7\n");
    terminal_writestring("Hello, kernel World!8\n");
    terminal_writestring("Hello, kernel World!8\n");
    terminal_writestring("Hello, kernel World!8\n");
    terminal_writestring("Hello, kernel World!8\n");
    terminal_writestring("Hello, kernel World!8\n");
}