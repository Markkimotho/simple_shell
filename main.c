#include "includes/shell.h"

/**
 * main - Entry point for the simple shell.
 * Initializes shell information, handles script file input,
 * and enters the main shell loop.
 * @ac: Argument count.
 * @av: Argument vector (array of strings).
 * Return: 0 on success, 1 on error (or exit code from hsh).
 */
int main(int ac, char **av)
{
	info_t info[] = { INFO_INIT }; // Initialize info_t struct
	int fd = 2; // Default file descriptor for error output (stderr)

	if (ac == 2) // If a script file is provided as an argument
	{
		fd = open(av[1], O_RDONLY); // Try to open the script file for reading
		if (fd == -1) // File opening failed
		{
			if (errno == EACCES) // Permission denied
				exit(126);
			if (errno == ENOENT) // File not found
			{
				_eputs(av[0]); // _eputs from error_handling/error_reporter.c
				_eputs(": 0: Can't open ");
				_eputs(av[1]);
				_eputchar('\n'); // _eputchar from error_handling/error_reporter.c
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE); // Other file open errors
		}
		info->readfd = fd; // Set shell's input file descriptor to the script file
	}
	populate_env_list(info); // Populate environment linked list (from environment/env_manager.c)
	read_history(info);      // Read command history (from history/history_manager.c)
	hsh(info, av);           // Start the main shell loop (from shell_core/shell_loop.c)

	return (EXIT_SUCCESS); // Shell exits normally
}
