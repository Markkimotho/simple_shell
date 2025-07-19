#include "../includes/shell.h"

/**
 * replace_alias - Replaces an alias in the tokenized string (argv[0]).
 * Attempts up to 10 replacements to handle nested aliases.
 * @info: The parameter struct.
 * Return: 1 if an alias was replaced, 0 otherwise.
 */
int replace_alias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++) // Max 10 alias replacements to prevent infinite loop
	{
		// Check if argv[0] is an alias
		node = node_starts_with(info->alias, info->argv[0], '='); // node_starts_with in linked_lists/list_utilities.c
		if (!node)
			return (0); // No alias found

		free(info->argv[0]); // Free the old argv[0] string

		p = _strchr(node->str, '='); // _strchr in string_operations/string_manipulation1.c
		if (!p)
			return (0); // Should not happen for a valid alias node

		p = _strdup(p + 1); // Duplicate the alias value (part after '=') (_strdup in string_operations/string_manipulation2.c)
		if (!p)
			return (0); // Malloc failure

		info->argv[0] = p; // Assign the new alias value to argv[0]
	}
	return (1); // Alias was replaced
}

/**
 * replace_vars - Replaces variables in the tokenized string (argv).
 * Handles $?, $$, $ENV_VAR.
 * @info: The parameter struct.
 * Return: 1 if any variable was replaced, 0 otherwise.
 */
int replace_vars(info_t *info)
{
	int i = 0;
	list_t *node;
	int replaced_any = 0;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1]) // Not a variable or just '$'
			continue;

		replaced_any = 1; // Assume replacement will happen

		if (!_strcmp(info->argv[i], "$?")) // Replace with last exit status
		{
			replace_string(&(info->argv[i]),
				_strdup(convert_number(info->status, 10, 0))); // convert_number in memory_utils/string_converters.c
		}
		else if (!_strcmp(info->argv[i], "$$")) // Replace with shell's PID
		{
			replace_string(&(info->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
		}
		else // Replace with environment variable
		{
			node = node_starts_with(info->env, &info->argv[i][1], '='); // Search env list
			if (node)
			{
				replace_string(&(info->argv[i]),
					_strdup(_strchr(node->str, '=') + 1)); // Replace with env value
			}
			else
			{
				replace_string(&info->argv[i], _strdup("")); // Variable not found, replace with empty string
			}
		}
	}
	return (replaced_any);
}

/**
 * replace_string - Replaces an old string pointer with a new string pointer,
 * and frees the memory of the old string.
 * @old: Address of the old string pointer.
 * @new: New string pointer.
 * Return: 1 if replaced, 0 otherwise (should always be 1 if old is not NULL).
 */
int replace_string(char **old, char *new)
{
	free(*old); // Free the memory pointed to by old
	*old = new; // Update the pointer to the new string
	return (1);
}
