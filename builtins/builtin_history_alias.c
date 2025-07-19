#include "../includes/shell.h"

/**
 * _myhistory - Displays the history list, one command by line, preceded
 * with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0.
 */
int _myhistory(info_t *info)
{
	print_list(info->history); // print_list is in linked_lists/list_utilities.c
	return (0);
}

/**
 * unset_alias - Unsets an alias by removing it from the alias list.
 * @info: Parameter struct.
 * @str: The alias string (e.g., "la=ls -al").
 * Return: Always 0 on success, 1 on error.
 */
int unset_alias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = _strchr(str, '='); // _strchr is in string_operations/string_manipulation1.c
	if (!p)
		return (1);
	c = *p;
	*p = 0; // Temporarily null-terminate to get the alias name
	ret = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	if (ret == 0)
	{
		_puts("Error: Alias not found\n");
		return (1);
	}
	*p = c; // Restore the original string
	return (ret);
}

/**
 * set_alias - Sets an alias by adding or updating it in the alias list.
 * @info: Parameter struct.
 * @str: The alias string (e.g., "la=ls -al").
 * Return: Always 0 on success, 1 on error.
 */
int set_alias(info_t *info, char *str)
{
	char *p;

	p = _strchr(str, '=');
	if (!p)
		return (1);
	if (!*++p) // If there's no value after '=', unset the alias
		return (unset_alias(info, str));

	unset_alias(info, str); // Unset existing alias before setting new one
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * print_alias - Prints an alias string in 'name='value'' format.
 * @node: The alias list node.
 * Return: Always 0 on success, 1 on error.
 */
int print_alias(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = _strchr(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - Mimics the alias built-in (man alias).
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0.
 */
int _myalias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1) // If no arguments, print all aliases
	{
		node = info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++) // Process arguments
	{
		p = _strchr(info->argv[i], '=');
		if (p) // If argument contains '=', it's a set operation
			set_alias(info, info->argv[i]);
		else // Otherwise, it's a print operation
			print_alias(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
