#include "kernel.h"

static struct term	term;
static uint16_t		*term_buffer = (uint16_t *)0xb8000;

/* ########################################################################## */

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

	/* Clear screen and hide the cursor */
	terminal_clear();
	hide_cursor();
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