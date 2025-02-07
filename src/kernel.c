#include "kernel.h"
#include "printk.h"
#include "io.h"
#include "cursor.h"

# define NTERM 3
static struct term terms[NTERM];
static uint16_t * const	term_buffer = (uint16_t *)0xb8000;
static uint8_t cur_term = 0;

/* ########################################################################## */

static uint16_t	vga_entry(char c)
{
	return ((uint16_t)c | (uint16_t)terms[cur_term].color << 8);
}

void cpy_term(void) {
    for (uint16_t i = 0; i < 2000; i++) {
        terms[cur_term].buf[i] = term_buffer[i] & 0xFF;
    }
}

void paste_term(uint8_t term_idx) {
    for (uint16_t i = 0; i < 2000; i++) {
        term_buffer[i] = vga_entry(terms[term_idx].buf[i]);
    }
}

void switch_term(uint8_t term_idx) {
    cpy_term();
    cur_term = term_idx;
    paste_term(cur_term);
    set_cursor_coord(terms[cur_term].row, terms[cur_term].col);
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
	terms[cur_term].col = 0;
	if (terms[cur_term].row + 1 >= VGA_HEIGHT)
		terminal_scroll();
	else
		terms[cur_term].row++;
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
	if (c == '\n' || terms[cur_term].col == VGA_WIDTH)
		terminal_newline();
	if (c != '\n')
		terminal_putchar_at(c, terms[cur_term].col++, terms[cur_term].row);
	set_cursor_coord(terms[cur_term].row, terms[cur_term].col);
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
    }

    terms[1].color = RED | BLACK << 4;

	/* Clear screen and set the cursor */
	terminal_clear();
    enable_cursor(14, 15); // default cursor
	set_cursor_coord(terms[cur_term].row, terms[cur_term].col);
}

void term_rainbow_write(const char* str) {
    static uint8_t colors[8] = {RED, LIGHT_RED, LIGHT_BROWN, LIGHT_GREEN, CYAN, LIGHT_BLUE, MAGENTA, WHITE};

    for (uint32_t i = 0; str[i]; ++i) {
        terms[cur_term].color = colors[terms[cur_term].col / 10];
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

	// for (int i =0; i<5; i++) {
	// 	printk("Hello, kernel World! %d\n", i);
	// }
	// printk("Hello, kernel World! %s\n", "coucouuuuu");
	// printk("Hello, kernel World!");
    // switch_term(1);
    // printk("Hello\n");
	// for (int i =0; i<5; i++) {
	// 	printk("Hello, kernel World! %d\n", i);
	// }
    // printk("cursor_row : %d | cursor_col : %d | ", terms[cur_term].row, terms[cur_term].col);
    // printk("cursor_col: %d", terms[cur_term].col);
    // switch_term(0);
    // switch_term(1);


}
