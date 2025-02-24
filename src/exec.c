#include "exec.h"
#include "kernel.h"
#include "io.h"
#include "printk.h"

static uint16_t *const term_buffer = (uint16_t *)VGA_BUF_ADDR;

int strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, uint32_t n)
{
	while (n && *s1 && *s1 == *s2)
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return 0;
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void get_cmd(char *cmd, uint32_t row)
{
	for (uint8_t i = 0; i < VGA_WIDTH; ++i)
	{
		cmd[i] = (uint8_t)term_buffer[row * VGA_WIDTH + i];
	}
	cmd[VGA_WIDTH] = '\0';

	for (int8_t i = VGA_WIDTH - 1; i >= 0; --i)
	{
		if (cmd[i] == ' ' || cmd[i] == '\n' || cmd[i] == '\0')
			cmd[i] = '\0';
		else
			break;
	}
}

uint32_t hexstring_to_uint32(const char *str, uint32_t *res)
{
	if (!str || !res)
		return -1;

	uint32_t val = 0;
	for (uint8_t i = 0; i < 8 && str[i] != '\0'; ++i)
	{
		uint8_t c = str[i];
		if (c >= '0' && c <= '9')
			val = val * 16 + (c - '0');
		else if (c >= 'a' && c <= 'f')
			val = val * 16 + (c - 'a' + 10);
		else if (c >= 'A' && c <= 'F')
			val = val * 16 + (c - 'A' + 10);
		else
			return -1; // Invalid character
	}
	*res = val;
	return 0;
}

void exec(uint32_t row)
{
	char cmd[VGA_WIDTH + 1];
	get_cmd(cmd, row);

	if (strcmp(cmd, "help") == 0)
	{
		terminal_write("Available commands:\n");
		terminal_write("help: display this help\n");
		terminal_write("clear: clear the screen\n");
		terminal_write("echo: print the arguments\n");
		terminal_write("reboot: reboot the system\n");
		terminal_write("print8mem 0xADDRESS: print 16 * 4 bytes of memory starting at ADDRESS\n");
	}
	else if (strcmp(cmd, "clear") == 0)
	{
		terminal_clear();
	}
	else if (strncmp(cmd, "echo ", 5) == 0)
	{
		terminal_write(cmd + 5); // Print argument after "echo "
		terminal_putchar('\n');
	}
	else if (strcmp(cmd, "echo") == 0)
	{
		terminal_write("\n"); // Just newline if "echo" is typed alone
	}
	else if (strcmp(cmd, "reboot") == 0)
	{
		out8(0x64, 0xFE);
	}
	else if (strncmp(cmd, "print8mem", 9) == 0)
	{
		if (cmd[9] == ' ' && strncmp(cmd + 10, "0x", 2) == 0)
		{
			uint32_t address;
			if (hexstring_to_uint32(cmd + 12, &address) != 0)
			{
				terminal_write("Invalid hex address\n");
				return;
			}
			for (uint8_t j = 0; j < 4; j++)
			{
				uint32_t cur_address = address + j * 16;
				printk("0x%x: ", cur_address);
				for (uint8_t i = 0; i < 4; ++i)
				{
					printk("0x%x ", *(uint32_t *)(cur_address + 4 * i));
				}

				// Print ascii characters
				for (uint8_t i = 0; i < 16; ++i)
				{
					if (i % 4 == 0 && i != 0)
						printk(" ");
					const uint8_t c = *(uint8_t *)(cur_address + i);
					if (c >= 32 && c <= 126)
						terminal_putchar(c);
					else
						terminal_putchar('.');
				}
				terminal_putchar('\n');
			}
		}
		else
		{
			terminal_write("Usage: print_mem 0xADDRESS\n");
		}
	}
	else
	{
		terminal_write("Command not found: ");
		terminal_write(cmd);
		terminal_putchar('\n');
	}
}
