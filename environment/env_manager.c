#include "../includes/shell.h"

/**
 * _myenv - Prints the current environment.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0.
 */
int _myenv(info_t *info)
{
	print_list_str(info->env);
	return (0);
}

/**
 * _getenv - Gets the value of an environment variable.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * @name: The name of the environment variable (e.g., "HOME=").
 * Return: The value of the environment variable, or NULL if not found.
 */
char *_getenv(info_t *info, const char *name)
{
    list_t *node = info->env;
    char *p;


    while (node)
    {
        p = starts_with(node->str, name); // `node->str` is "HOME=/foo", `name` is "HOME="
                                         // If it matches, `p` points to "/foo"


        if (p && *p) // Check if `p` is not NULL and the character it points to is not '\0'
        {
            return (p);
        }
        node = node->next;
    }
    return (NULL);
}

/**
 * _mysetenv - Initialize a new environment variable,
 * or modify an existing one.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0 on success, 1 on error.
 */
int _mysetenv(info_t *info)
{
	if (info->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	// Call the internal _setenv helper
	if (_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - Remove an environment variable.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0 on success, 1 on error.
 */
int _myunsetenv(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unsetenv(info, info->argv[i]); // Call the internal _unsetenv helper

	return (0);
}

/**
 * populate_env_list - Populates the environment linked list.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0.
 */
int populate_env_list(info_t *info)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; environ[i]; i++) // 'environ' is the global variable
    {
        // Issue is likely here:
        // environ[i] is a pointer to the actual env string (e.g., "PATH=/usr/bin")
        // This pointer is passed to add_node_end
        add_node_end(&node, environ[i], 0);
    }
    info->env = node;
    return (0);
}

/**
 * get_environ - Returns the string array copy of our environment.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: A null-terminated array of strings (the environment).
 */
char **get_environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * _unsetenv - Removes an environment variable from the linked list.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * @var: The string name of the environment variable property to remove.
 * Return: 1 on delete, 0 otherwise.
 */
int _unsetenv(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), i);
			i = 0; // Reset index as list might change after deletion
			node = info->env; // Reset node to head
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * _setenv - Initializes a new environment variable,
 * or modifies an existing one.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * @var: The string name of the environment variable property.
 * @value: The string value of the environment variable.
 * Return: Always 0 on success, 1 on error.
 */
int _setenv(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	// Allocate buffer for "VAR=VALUE"
	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);

	node = info->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=') // Found an existing variable
		{
			free(node->str); // Free old string
			node->str = buf; // Assign new string (buf)
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	// Variable not found, add as new node
	add_node_end(&(info->env), buf, 0);
	free(buf); // add_node_end makes a copy, so free our local buf
	info->env_changed = 1;
	return (0);
}
