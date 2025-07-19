#include "../includes/shell.h"

/**
 * _strlen - Calculates the length of a string.
 * @s: The string whose length to check.
 * Return: Integer length of the string, or 0 if NULL.
 */
int _strlen(char *s)
{
	int i = 0;

	if (!s)
		return (0);

	while (*s++)
		i++;
	return (i);
}

/**
 * _strcmp - Performs lexicographical comparison of two strings.
 * @s1: The first string.
 * @s2: The second string.
 * Return: Negative if s1 < s2, positive if s1 > s2, zero if s1 == s2.
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	if (*s1 == *s2) // If both reached null terminator at same time, strings are equal
		return (0);
	else // One string is shorter than the other
		return (*s1 < *s2 ? -1 : 1);
}

/**
 * starts_with - Checks if a haystack string starts with a needle substring.
 * @haystack: The string to search within.
 * @needle: The substring to find at the beginning.
 * Return: Address of the character in haystack immediately following the needle,
 * or NULL if haystack does not start with needle.
 */
char *starts_with(const char *haystack, const char *needle)
{

    const char *h = haystack;
    const char *n = needle;
    while (*n) // Loop while needle has characters
    {


        if (*n++ != *h++) // Compare char by char and advance both pointers
        {
            return (NULL); // Mismatch
        }
    }
    // If loop completes, it means all characters in needle matched
    return ((char *)h); // Returns pointer in haystack AFTER the matched needle.
}

/**
 * _strcat - Concatenates two strings.
 * Appends the src string to the dest string, overwriting the null byte
 * at the end of dest, and then adds a terminating null byte.
 * @dest: The destination buffer.
 * @src: The source buffer.
 * Return: Pointer to the destination buffer (dest).
 */
char *_strcat(char *dest, char *src)
{
	char *ret = dest; // Save starting address of dest

	while (*dest) // Find the null terminator of dest
		dest++;
	while (*src) // Copy characters from src to dest
		*dest++ = *src++;
	*dest = *src; // Copy the null terminator from src
	return (ret);
}

/* --- Functions from original exits.c are integrated below --- */

/**
 * _strncpy - Copies up to n characters from a source string to a destination.
 * If n is less than the length of src, only n characters are copied.
 * If n is greater than the length of src, the destination is padded with null bytes.
 * @dest: The destination string to be copied to.
 * @src: The source string.
 * @n: The maximum amount of characters to be copied.
 * Return: The pointer to the destination string (dest).
 */
char *_strncpy(char *dest, char *src, int n)
{
	int i, j;
	char *s = dest; // Save destination address

	i = 0;
	while (src[i] != '\0' && i < n - 1) // Copy characters until n-1 or null terminator of src
	{
		dest[i] = src[i];
		i++;
	}
	if (i < n) // If less than n characters copied, pad with null bytes
	{
		j = i;
		while (j < n)
		{
			dest[j] = '\0';
			j++;
		}
	}
	return (s);
}

/**
 * _strncat - Concatenates two strings, copying at most n bytes from src.
 * Appends the src string to the dest string, but only copies up to 'n' bytes.
 * @dest: The first string (destination).
 * @src: The second string (source).
 * @n: The maximum amount of bytes to be used from src.
 * Return: The pointer to the concatenated string (dest).
 */
char *_strncat(char *dest, char *src, int n)
{
	int i, j;
	char *s = dest;

	i = 0;
	j = 0;
	while (dest[i] != '\0') // Find end of dest string
		i++;
	while (src[j] != '\0' && j < n) // Copy up to n characters from src
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	if (j < n) // Ensure dest is null-terminated if src was shorter than n
		dest[i] = '\0';
	return (s);
}

/**
 * _strchr - Locates the first occurrence of a character in a string.
 * @s: The string to be parsed.
 * @c: The character to look for.
 * Return: A pointer to the first occurrence of the character 'c' in 's',
 * or NULL if the character is not found.
 */
char *_strchr(char *s, char c)
{
	do {
		if (*s == c) // Check current character
			return (s);
	} while (*s++ != '\0'); // Move to next character until null terminator

	return (NULL); // Character not found
}

/**
 * _strrchr - Locates the last occurrence of a character in a string.
 * @s: The string to be parsed.
 * @c: The character to look for.
 * Return: A pointer to the last occurrence of the character 'c' in 's',
 * or NULL if the character is not found.
 */
char *_strrchr(char *s, char c)
{
    char *last_occurrence = NULL;
    if (!s)
        return (NULL);

    while (*s != '\0')
    {
        if (*s == c)
            last_occurrence = s;
        s++;
    }
    // Also check for null terminator if 'c' is '\0'
    if (c == '\0' && *s == '\0')
        return s;

    return (last_occurrence);
}
