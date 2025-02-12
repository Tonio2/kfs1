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

uint32_t atoi(const char *str)
{
	uint32_t res = 0;
	for (uint32_t i = 0; str[i] != '\0'; ++i)
	{
		res = res * 10 + str[i] - '0';
	}
	return res;
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
			const uint32_t address = atoi(cmd + 12);
			printk("0x%x: 0x%x\n", address, *(uint32_t *)address);
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
