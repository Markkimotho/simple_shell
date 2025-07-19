#include "../includes/shell.h"

/**
 * _strcpy - Copies a string from source to destination.
 * @dest: The destination buffer.
 * @src: The source string.
 * Return: Pointer to the destination string (dest).
 */
char *_strcpy(char *dest, char *src)
{
	int i = 0;

	if (dest == src || src == 0) // Handle self-copy or null source
		return (dest);
	while (src[i]) // Copy characters until null terminator of src
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0; // Null-terminate the destination string
	return (dest);
}

/**
 * _strdup - Duplicates a string by allocating new memory.
 * @str: The string to duplicate.
 * Return: Pointer to the newly allocated and duplicated string, or NULL on failure.
 */
char *_strdup(const char *str)
{
    int length = 0;
    char *ret;

    if (str == NULL)
    {
        return (NULL);
    }

    // Calculate length (safer way than modifying 'str')
    length = _strlen((char *)str); // Use _strlen from your library

    ret = malloc(sizeof(char) * (length + 1));
    if (!ret)
    {
        return (NULL);
    }

    // Copy characters (standard way)
    _strcpy(ret, (char *)str); // Use _strcpy from your library

    return (ret);
}

/**
 * _puts - Prints an input string to stdout.
 * @str: The string to be printed.
 * Return: Nothing.
 */
void _puts(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_putchar(str[i]); // _putchar is defined in this file.
		i++;
	}
}

/**
 * _putchar - Writes the character c to stdout.
 * Implements buffering for efficiency.
 * @c: The character to print.
 * Return: On success 1. On error, -1 is returned.
 */
int _putchar(char c)
{
	static int i; // Static counter for buffer index
	static char buf[WRITE_BUF_SIZE]; // Static buffer for output

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE) // If buffer is full or flush signal
	{
		write(1, buf, i); // Write buffer content to stdout
		i = 0; // Reset buffer index
	}
	if (c != BUF_FLUSH) // If not a flush signal, add char to buffer
		buf[i++] = c;
	return (1);
}
