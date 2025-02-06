#include "kernel.h"
#include "printk.h"
#include "io.h"

static struct term	term;
static uint16_t		*term_buffer = (uint16_t *)0xb8000;

/* ########################################################################## */

static void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    // Cursor Start Register (0x0A)
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    // Cursor End Register (0x0B)
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

static void update_cursor(int x, int y) {
    uint16_t pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

static uint16_t	vga_entry(char c)
{
	return ((uint16_t)c | (uint16_t)term.color << 8);
}

/* Scrolls the screen up by 1 line */
static void terminal_scroll(void)
{
	uint32_t i = 0;

	/* displace all screen data backwards by VGA_WIDTH * 2 bytes */
	for (; i < VGA_HEIGHT * VGA_WIDTH - VGA_WIDTH; ++i)
		term_buffer[i] = term_buffer[i + VGA_WIDTH];

	/* Clears the last line */
	for (; i < VGA_HEIGHT * VGA_WIDTH; i++)
		term_buffer[i] = vga_entry(' ');
}

void	terminal_newline()
{
	term.col = 0;
	if (term.row + 1 >= VGA_HEIGHT)
		terminal_scroll();
	else
		term.row++;
}

/* Clears the entire screen */
void	terminal_clear(void)
{
	for (uint32_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
		term_buffer[i] = vga_entry(' ');
}

void	terminal_putchar_at(char c, uint32_t x, uint32_t y)
{
	term_buffer[y * VGA_WIDTH + x] = vga_entry(c);
}

void	terminal_putchar(char c)
{
	if (c == '\n' || term.col == VGA_WIDTH)
		terminal_newline();
	if (c != '\n')
		terminal_putchar_at(c, term.col++, term.row);
	update_cursor(term.col, term.row);
}

void	terminal_write(const char* str)
{
	for (uint32_t i = 0; str[i]; ++i)
		terminal_putchar(str[i]);
}

void	terminal_initialize(void) {
	term.row	= 0;
	term.col	= 0;
	term.color	= LIGHT_GREY | BLACK << 4;

	/* Clear screen and set the cursor */
	terminal_clear();

	enable_cursor(14, 15);
    update_cursor(term.col, term.row);
}

void	kernel_main(void) 
{
	terminal_initialize();
	
	for (int i =0; i<5; i++) {
		printk("Hello, kernel World! %d\n", i);
	}
	printk("Hello, kernel World! %s\n", "coucouuuuu");
	printk("Hello, kernel World!");

}