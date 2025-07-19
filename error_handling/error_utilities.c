#include "../includes/shell.h"

/**
 * _erratoi - Converts a string to an integer, specifically for error codes.
 * Returns -1 on error (e.g., non-numeric input or overflow).
 * @s: The string to be converted.
 * Return: 0 if no numbers in string, converted number otherwise, -1 on error.
 */
int _erratoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+') // Handle optional leading '+'
		s++;
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX) // Check for overflow
				return (-1);
		}
		else // Non-numeric character found
			return (-1);
	}
	return (result);
}

/**
 * print_error - Prints a standardized error message to stderr.
 * Format: "shell_name: line_count: command: error_message\n"
 * @info: The parameter & return info struct containing shell state.
 * @estr: String containing the specified error type or message.
 * Return: void.
 */
void print_error(info_t *info, char *estr)
{
	_eputs(info->fname); // _eputs in error_handling/error_reporter.c
	_eputs(": ");
	print_d(info->line_count, STDERR_FILENO); // print_d is defined in this file
	_eputs(": ");
	_eputs(info->argv[0]);
	_eputs(": ");
	_eputs(estr);
}

/**
 * print_d - Function prints a decimal (integer) number (base 10) to a given fd.
 * @input: The integer input.
 * @fd: The file descriptor to write to (e.g., STDERR_FILENO).
 * Return: Number of characters printed.
 */
int print_d(int input, int fd)
{
	int (*__putchar)(char) = _putchar; // Default to _putchar (stdout)
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar; // Use _eputchar for stderr

	if (input < 0)
	{
		_abs_ = -input; // Convert to positive
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;
	current = _abs_;

	// Print digits from most significant to least significant
	for (i = 1000000000; i > 1; i /= 10) // Start with largest power of 10
	{
		if (_abs_ / i) // If there's a non-zero digit at this place
		{
			__putchar('0' + current / i); // Print the digit
			count++;
		}
		current %= i; // Get the remaining part of the number
	}
	__putchar('0' + current); // Print the last digit (ones place)
	count++;

	return (count);
}

/**
 * remove_comments - Function replaces the first instance of '#' with '\0'.
 * This effectively removes comments from a command line string.
 * Comments are ignored if they are not preceded by a space or are at the start of the line.
 * @buf: Address of the string to modify.
 * Return: Always 0; (void function, returns nothing significant).
 */
void remove_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' ')) // Check for '#' and if it's start of line or preceded by space
		{
			buf[i] = '\0'; // Null-terminate at the '#'
			break;
		}
}
