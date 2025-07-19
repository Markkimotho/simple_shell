#include "../includes/shell.h"

/**
 * hsh - Main shell loop.
 * Orchestrates input reading, command parsing, built-in checking,
 * and external command execution.
 * @info: The parameter & return info struct.
 * @av: The argument vector from main().
 * Return: 0 on success, 1 on error, or an error code signaling exit.
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0; // Bytes read from input
	int builtin_ret = 0; // Return value of built-in command

	// Loop until EOF or an exit signal from a built-in
	while (r != -1 && builtin_ret != -2)
	{
		clear_info(info); // Clear info struct for new command cycle (from shell_core/shell_utilities.c)
		if (is_interactive(info))
		{
			char cwd[1024]; // Buffer to hold the current working directory
			char *p_cwd = getcwd(cwd, sizeof(cwd)); // Get CWD

			if (p_cwd) // If getcwd was successful
			{
				// Optional: Display only the last directory name for brevity
				char *last_slash = _strrchr(p_cwd, '/'); // _strrchr would be in string_manipulation.c if implemented
				if (last_slash && *(last_slash + 1) != '\0') // If not root and not trailing slash
				{
					_puts(last_slash + 1); // Print just the directory name
				}
				else if (last_slash && *(last_slash + 1) == '\0' && _strlen(p_cwd) > 1) // For /home/user/
				{
					char *prev_slash = NULL;
					int len = _strlen(p_cwd);
					if (len > 1) { // Not just "/"
						// Find the second to last slash to get the last component before a trailing slash
						for (int i = len - 2; i >= 0; i--) {
							if (p_cwd[i] == '/') {
								prev_slash = &p_cwd[i];
								break;
							}
						}
						if (prev_slash) {
							_puts(prev_slash + 1);
						} else {
							_puts(p_cwd); // Fallback for /dir (no second slash)
						}
					} else { // It's just "/"
						_puts("/");
					}
				}
				else // For root directory "/"
				{
					_puts(p_cwd); // Print full path if simple or root
				}
				_puts("$ "); // Append the dollar sign
			}
			else
			{
				_puts("hsh$ "); // Fallback prompt if getcwd fails
			}
		}
		_eputchar(BUF_FLUSH); // Flush stderr buffer (from error_handling/error_reporter.c)

		r = get_input(info); // Read input line (from io_handling/input_reader.c)
		if (r != -1) // If input was successfully read
		{
			set_info(info, av); // Parse arguments and set info struct (from shell_core/shell_utilities.c)
			builtin_ret = find_builtin(info); // Check if it's a built-in
            if (builtin_ret == -1) // If not a built-in
            {
                find_cmd(info); // Find the path for the command (updates info->path)
                if (info->path) // If a path was successfully found
                {
                    fork_cmd(info); // Execute the command (from shell_core/command_executor.c)
                }
            }
		}
		else if (is_interactive(info)) // If EOF encountered in interactive mode, print newline
			_putchar('\n');             // _putchar from string_operations/string_manipulation2.c

		free_info(info, 0); // Free command-specific info fields (from shell_core/shell_utilities.c)
	}

	write_history(info); // Write history to file before exit (from history/history_manager.c)
	free_info(info, 1); // Free all remaining info fields (including lists)

	if (!is_interactive(info) && info->status) // If not interactive and there's a status
		exit(info->status); // Exit with last command's status
	if (builtin_ret == -2) // If a built-in explicitly signaled exit
	{
		if (info->err_num == -1) // Default exit status
			exit(info->status);
		exit(info->err_num); // Exit with the built-in's error number
	}
	return (builtin_ret); // Return final built-in status
}

/*
 * loophsh is commented out as it was noted it might become obsolete.
 * In a fully refactored shell, hsh() itself manages the loop.
 *
 * int loophsh(char **av)
 * {
 * // This function's logic would largely be absorbed by the hsh function directly.
 * // If needed, it would simply call hsh.
 * // For now, we omit an explicit implementation of loophsh.
 * return (0);
 * }
 */
