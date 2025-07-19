#include "../includes/shell.h"

/**
 * convert_number - Converter function, a clone of itoa (integer to ASCII).
 * Converts a long integer to a string representation in a specified base.
 * @num: The number to convert.
 * @base: The base for conversion (e.g., 10 for decimal, 16 for hexadecimal).
 * @flags: Argument flags that modify conversion behavior:
 * CONVERT_UNSIGNED (for unsigned numbers),
 * CONVERT_LOWERCASE (for lowercase hex characters).
 * Return: A pointer to the static buffer containing the string representation.
 */
char *convert_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50]; // Static buffer to hold the converted string
	char sign = 0;
	char *ptr;
	unsigned long n = num; // Use unsigned long for conversion logic

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-'; // Set sign for negative numbers
	}
	// Determine the character set based on CONVERT_LOWERCASE flag
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49]; // Start pointer from end of buffer (for null terminator)
	*ptr = '\0'; // Null-terminate the string

	do {
		*--ptr = array[n % base]; // Convert digit and move pointer left
		n /= base; // Divide number by base
	} while (n != 0); // Continue until number becomes 0

	if (sign) // If negative, add the sign
		*--ptr = sign;
	return (ptr); // Return pointer to the start of the converted string
}
