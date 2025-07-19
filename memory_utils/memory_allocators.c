#include "../includes/shell.h"

/**
 * bfree - Frees a pointer and NULLs the address to prevent dangling pointers.
 * @ptr: Address of the pointer to free.
 * Return: 1 if freed, otherwise 0.
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}

/**
 * _memset - Fills memory with a constant byte.
 * @s: The pointer to the memory area.
 * @b: The byte to fill *s with.
 * @n: The amount of bytes to be filled.
 * Return: (s) a pointer to the memory area s.
 */
char *_memset(char *s, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		s[i] = b;
	return (s);
}

/**
 * ffree - Frees a string of strings (an array of char pointers).
 * @pp: Pointer to the array of strings to be freed.
 */
void ffree(char **pp)
{
	char **a = pp; // Keep a pointer to the start for freeing the array itself

	if (!pp) // If the pointer to the array is NULL, nothing to do
		return;
	while (*pp) // Iterate through each string in the array
		free(*pp++); // Free the current string and move to the next
	free(a); // Free the array of pointers itself
}

/**
 * _realloc - Reallocates a block of memory.
 * This custom realloc handles scenarios where ptr is NULL (acts like malloc),
 * new_size is 0 (acts like free), or new_size equals old_size (returns original ptr).
 * @ptr: Pointer to the previous malloc'ated block.
 * @old_size: Byte size of the previous block.
 * @new_size: Byte size of the new block.
 * Return: Pointer to the reallocated memory block, or NULL on failure.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *p;

	if (!ptr) // If ptr is NULL, act like malloc
		return (malloc(new_size));
	if (!new_size) // If new_size is 0, act like free
		return (free(ptr), NULL);
	if (new_size == old_size) // If sizes are same, no reallocation needed
		return (ptr);

	p = malloc(new_size); // Allocate new memory block
	if (!p)
		return (NULL);

	// Copy content from old block to new block, up to the minimum of old_size or new_size
	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		p[old_size] = ((char *)ptr)[old_size];
	free(ptr); // Free the old memory block
	return (p);
}
