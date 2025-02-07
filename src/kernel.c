#include "kernel.h"
#include "printk.h"
#include "io.h"
#include "cursor.h"
#include "keycodes.h"

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


/* 0:UP, 1:DOWN, 2:LEFT, 3:RIGHT */
void	terminal_cursor_move(uint8_t dir)
{
	switch (dir)
	{
		/* UP */
		case 0:
			if (term->row > 0)
				--term->row;
			break;

		/* DOWN */
		case 1:
			if (term->row < VGA_HEIGHT - 1)
				++term->row;
			break;

		/* LEFT */
		case 2:
			if (term->col <= 0 && term->row != 0)
			{
				term->col = VGA_WIDTH - 1;
				--term->row;
			}
			else if (term->col > 0)
				--term->col;
			break;

		/* RIGHT */
		case 3:
			if (term->col >= VGA_WIDTH - 1 && term->row != VGA_HEIGHT - 1)
			{
				term->col = 0;
				++term->row;
			}
			else if (term->col < VGA_WIDTH - 1)
				++term->col;
			break;
	}
	position_cursor(term->row, term->col);
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

/* writes a char at the designated coordinates without changing the cursor */
void	terminal_putchar_at(char c, uint32_t x, uint32_t y)
{
	term_buffer[y * VGA_WIDTH + x] = vga_entry(c);
}

void	terminal_del(void)
{
	terminal_cursor_move(2);
	terminal_putchar_at(' ', term->col, term->row);
	position_cursor(term->row, term->col);
}

void	terminal_putchar(char c)
{
	if (c != '\n')
		terminal_putchar_at(c, term->col++, term->row);
	if (term->col >= VGA_WIDTH || c == '\n')
		terminal_newline();
	position_cursor(term->row, term->col);
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
	uint16_t in;

	terminal_initialize();

	welcome_msg();

	switch_term(1);

	for (int i =0; i<5; i++) {
		printk("Hello, kernel World! %d\n", i);
	}

	switch_term(0);

	switch_term(2);
	printk("Hello, kernel World! %s\n", "coucouuuuu");
	printk("Hello, kernel World!\n");

	while (1)
	{

		/*
		 * Serial port 0x64 (status register) polling to wait for bit 1
		 * (output buffer full)
		 */
		if (in8(0x64) & 1)
		{
			in = in16(0x60);
			// printk("keypress [0x%x]-", in);
			// printk("keypress [0x%x]  ", in & 0xff);
			switch (in & 0xff)
			{
				case a:			terminal_putchar('a'); break;
				case b:			terminal_putchar('b'); break;
				case c:			terminal_putchar('c'); break;
				case d:			terminal_putchar('d'); break;
				case e:			terminal_putchar('e'); break;
				case f:			terminal_putchar('f'); break;
				case g:			terminal_putchar('g'); break;
				case h:			terminal_putchar('h'); break;
				case i:			terminal_putchar('i'); break;
				case j:			terminal_putchar('j'); break;
				case k:			terminal_putchar('k'); break;
				case l:			terminal_putchar('l'); break;
				case m:			terminal_putchar('m'); break;
				case n:			terminal_putchar('n'); break;
				case o:			terminal_putchar('o'); break;
				case p:			terminal_putchar('p'); break;
				case q:			terminal_putchar('q'); break;
				case r:			terminal_putchar('r'); break;
				case s:			terminal_putchar('s'); break;
				case t:			terminal_putchar('t'); break;
				case u:			terminal_putchar('u'); break;
				case v:			terminal_putchar('v'); break;
				case w:			terminal_putchar('w'); break;
				case x:			terminal_putchar('x'); break;
				case y:			terminal_putchar('y'); break;
				case z:			terminal_putchar('z'); break;

				case one:		terminal_putchar('1'); break;
				case one + 1:	terminal_putchar('2'); break;	/* je suis désolé.. */
				case one + 2:	terminal_putchar('3'); break;
				case one + 3:	terminal_putchar('4'); break;
				case one + 4:	terminal_putchar('5'); break;
				case one + 5:	terminal_putchar('6'); break;
				case one + 6:	terminal_putchar('7'); break;
				case one + 7:	terminal_putchar('8'); break;
				case one + 8:	terminal_putchar('9'); break;
				case zero:		terminal_putchar('0'); break;

				case dash:		terminal_putchar('-'); break;
				case equal:		terminal_putchar('='); break;
				case del:		terminal_del(); break;
				case enter:		terminal_putchar('\n'); break;
				case space:		terminal_putchar(' '); break;

				case f1:		terminal_putchar('@'); break;
				case f2:		terminal_putchar('@'); break;
				case f3:		terminal_putchar('@'); break;
				case f4:		terminal_putchar('@'); break;
				case f5:		terminal_putchar('@'); break;
				case f6:		terminal_putchar('@'); break;
				case f7:		terminal_putchar('@'); break;
				case f8:		terminal_putchar('@'); break;

				case extended:
					switch ((in >> 8) & 0xff)
					{
						case up:	terminal_cursor_move(0); break;
						case down:	terminal_cursor_move(1); break;
						case left:	terminal_cursor_move(2); break;
						case right:	terminal_cursor_move(3); break;
					}
				break;

				// case esc: asm("hlt"); break;
			}
		}
	}
}
