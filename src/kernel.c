#include "kernel.h"
#include "printk.h"
#include "io.h"
#include "cursor.h"

# define NTERM 3
static struct term terms[NTERM];
static uint16_t * const	term_buffer = (uint16_t *)0xb8000;
static struct term * term = &terms[0];


/* ########################################################################## */

static uint16_t	vga_entry(char c)
{
	return ((uint16_t)c | (uint16_t)term->color << 8);
}

void cpy_term(void) {
    for (uint16_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        term->buf[i] = term_buffer[i];
    }
}

void paste_term() {
    for (uint16_t i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        term_buffer[i] = term->buf[i];
    }
}

void switch_term(uint8_t term_idx) {
    cpy_term();
    term = &terms[term_idx];
    paste_term();
    set_cursor_coord(term->row, term->col);
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
	term->col = 0;
	if (term->row + 1 >= VGA_HEIGHT)
		terminal_scroll();
	else
		term->row++;
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
	if (c == '\n' || term->col == VGA_WIDTH)
		terminal_newline();
	if (c != '\n')
		terminal_putchar_at(c, term->col++, term->row);
	set_cursor_coord(term->row, term->col);
}

void	terminal_write(const char* str)
{
	for (uint32_t i = 0; str[i]; ++i)
		terminal_putchar(str[i]);
}

void	terminal_initialize(void)
{
    for (uint8_t i = 0; i < NTERM; i++) {
        terms[i].row	= 0;
        terms[i].col	= 0;
        terms[i].color	= LIGHT_GREY | BLACK << 4;
		for (uint32_t j = 0; j < VGA_HEIGHT * VGA_WIDTH; j++)
			terms[i].buf[j] = vga_entry(' ');
    }

    terms[1].color = RED | BLACK << 4;

	/* Clear screen and set the cursor */
	terminal_clear();
    resize_cursor(14, 15); // default cursor
	set_cursor_coord(term->row, term->col);
}

void term_rainbow_write(const char* str) {
    static uint8_t colors[8] = {RED, LIGHT_RED, LIGHT_BROWN, LIGHT_GREEN, CYAN, LIGHT_BLUE, MAGENTA, WHITE};

    for (uint32_t i = 0; str[i]; ++i) {
        term->color = colors[term->col / 10];
        terminal_putchar(str[i]);
    }
}

void welcome_msg() {

	term_rainbow_write("\n\n\n\n\n");
	term_rainbow_write("               :::     ::::::::        :::    ::: ::::::::: ::::::::: \n");
    term_rainbow_write("             :+:     :+:    :+:       :+:   :+:  :+:       :+:    :+: \n");
    term_rainbow_write("           +:+ +:+        +:+        +:+  +:+   +:+       +:+         \n");
    term_rainbow_write("         +#+  +:+      +#+          +#++:++    :#::+::#  +#++:++#++   \n");
    term_rainbow_write("       +#+#+#+#+#+  +#+            +#+  +#+   +#+              +#+    \n");
    term_rainbow_write("            #+#   #+#             #+#   #+#  #+#       #+#    #+#     \n");
    term_rainbow_write("           ###  ##########       ###    ### ###        ########       \n");
	term_rainbow_write("\n\n\n\n\n");
	term_rainbow_write("                       KERNEL FROM SCRATCH (KFS)                      \n");
	term_rainbow_write("                  Crafted with <3 by  pitriche & alabalet             \n");
	term_rainbow_write("                             School 42                                \n");
}


void	kernel_main(void)
{
	terminal_initialize();

	welcome_msg();

	switch_term(1);

	for (int i =0; i<5; i++) {
		printk("Hello, kernel World! %d\n", i);
	}

	switch_term(0);

	switch_term(1);
}
