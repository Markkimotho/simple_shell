#include "../includes/shell.h"

/**
 * _eputs - Prints an input string to stderr.
 * @str: The string to be printed.
 * Return: Nothing.
 */
void _eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eputchar(str[i]); // _eputchar is defined in this file
		i++;
	}
}

/**
 * _eputchar - Writes the character c to stderr.
 * Implements buffering for efficiency.
 * @c: The character to print.
 * Return: On success 1. On error, -1 is returned.
 */
int _eputchar(char c)
{
	static int i; // Static counter for buffer index
	static char buf[WRITE_BUF_SIZE]; // Static buffer for stderr output

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE) // If buffer is full or flush signal
	{
		write(2, buf, i); // Write buffer content to stderr (fd 2)
		i = 0; // Reset buffer index
	}
	if (c != BUF_FLUSH) // If not a flush signal, add char to buffer
		buf[i++] = c;
	return (1);
}

/**
 * _putfd - Writes the character c to a given file descriptor.
 * Implements buffering for efficiency.
 * @c: The character to print.
 * @fd: The file descriptor to write to.
 * Return: On success 1. On error, -1 is returned.
 */
int _putfd(char c, int fd)
{
	static int i; // Static counter for buffer index
	static char buf[WRITE_BUF_SIZE]; // Static buffer

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE) // If buffer is full or flush signal
	{
		write(fd, buf, i); // Write buffer content to specified fd
		i = 0; // Reset buffer index
	}
	if (c != BUF_FLUSH) // If not a flush signal, add char to buffer
		buf[i++] = c;
	return (1);
}

/**
 * _putsfd - Prints an input string to a given file descriptor.
 * @str: The string to be printed.
 * @fd: The file descriptor to write to.
 * Return: The number of characters put.
 */
int _putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _putfd(*str++, fd); // Use _putfd for buffered writing to specified fd
	}
	return (i);
}
