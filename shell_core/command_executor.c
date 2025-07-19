#include "../includes/shell.h"

/**
 * fork_cmd - Forks a child process to run an external command using execve.
 * @info: The parameter & return info struct.
 * Return: Void.
 */
void fork_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		// TODO: PUT ERROR FUNCTION - Use print_error from error_handling/error_reporter.c
		perror("Error:"); // Generic error for now
		return;
	}
	if (child_pid == 0) // Child process
	{
		// Execute the command
		// get_environ from environment/env_manager.c
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			// If execve fails, free resources and exit child process
			free_info(info, 1); // free_info from shell_core/shell_utilities.c
			if (errno == EACCES) // Permission denied
				exit(126);
			exit(1); // General execution error
		}
		// This part should ideally not be reached if execve is successful
	}
	else // Parent process
	{
		wait(&(info->status)); // Wait for child process to complete
		if (WIFEXITED(info->status)) // Check if child exited normally
		{
			info->status = WEXITSTATUS(info->status); // Get child's exit status
			if (info->status == 126) // Handle permission denied specifically
				print_error(info, "Permission denied\n"); // print_error from error_handling/error_reporter.c
		}
	}
}
