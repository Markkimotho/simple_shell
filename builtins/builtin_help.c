#include "../includes/shell.h"

/**
 * _myhelp - Provides help information for shell built-in commands.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Always 0.
 */
int _myhelp(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
