#include "../includes/shell.h"

/**
 * list_len - Determines the length (number of nodes) of a linked list.
 * @h: Pointer to the first node of the list.
 * Return: The size (count of nodes) of the list.
 */
size_t list_len(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * list_to_strings - Converts a linked list of strings into an array of strings.
 * Each string in the array is a duplicate of the list node's 'str' field.
 * @head: Pointer to the first node of the list.
 * Return: An array of strings (char **), or NULL on failure.
 */
char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t i = list_len(head), j; // Get length to know array size
	char **strs;
	char *str;

	if (!head || !i) // Empty list or invalid head
		return (NULL);

	strs = malloc(sizeof(char *) * (i + 1)); // Allocate array of pointers (+1 for NULL terminator)
	if (!strs)
		return (NULL);

	for (i = 0; node; node = node->next, i++)
	{
		str = _strdup(node->str); // _strdup in string_operations/string_manipulation2.c
		if (!str) // Malloc failure for a string
		{
			for (j = 0; j < i; j++) // Free previously allocated strings
				free(strs[j]);
			free(strs); // Free the array of pointers
			return (NULL);
		}
		strs[i] = str; // Assign the duplicated string to the array
	}
	strs[i] = NULL; // Null-terminate the array of strings
	return (strs);
}

/**
 * print_list - Prints all elements of a list_t linked list,
 * including node number and string.
 * @h: Pointer to the first node of the list.
 * Return: The size (count of nodes) of the list.
 */
size_t print_list(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(convert_number(h->num, 10, 0)); // convert_number in memory_utils/string_converters.c
		_putchar(':');                        // _putchar in string_operations/string_manipulation2.c
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)"); // _puts in string_operations/string_manipulation2.c
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * print_list_str - Prints only the 'str' element of a list_t linked list.
 * @h: Pointer to the first node of the list.
 * Return: The size (count of nodes) of the list.
 */
size_t print_list_str(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)"); // _puts in string_operations/string_manipulation2.c
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * node_starts_with - Returns the node whose string starts with a given prefix.
 * Allows an optional character 'c' to match immediately after the prefix.
 * @node: Pointer to the list head.
 * @prefix: The string prefix to match.
 * @c: The character to match immediately after the prefix. Use -1 to ignore.
 * Return: A pointer to the matching node, or NULL if no match is found.
 */
list_t *node_starts_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = starts_with(node->str, prefix); // starts_with in string_operations/string_manipulation1.c
		if (p && ((c == -1) || (*p == c))) // If prefix matches AND (c is ignored OR *p matches c)
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * get_node_index - Gets the index (position) of a specific node in a list.
 * @head: Pointer to the list head.
 * @node: Pointer to the node whose index is to be found.
 * Return: The 0-based index of the node, or -1 if the node is not found.
 */
ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node) // Found the target node
			return (i);
		head = head->next;
		i++;
	}
	return (-1); // Node not found
}
