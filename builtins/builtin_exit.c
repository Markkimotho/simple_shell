#include "../includes/shell.h"

/**
 * _myexit - Exits the shell.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * Return: Exits with a given exit status.
 * (0) if info->argv[0] != "exit"
 */
int _myexit(info_t *info)
{
	int exitcheck;

	if (info->argv[1]) /* If there is an exit argument */
	{
		exitcheck = _erratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			_eputs(info->argv[1]);
			_eputchar('\n');
			return (1);
		}
		info->err_num = exitcheck; // Use exitcheck directly
		return (-2); // Special return to signal shell_loop to exit
	}
	info->err_num = -1; // No exit argument provided, signal default exit
	return (-2); // Special return to signal shell_loop to exit
}
