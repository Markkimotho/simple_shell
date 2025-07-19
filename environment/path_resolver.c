#include "../includes/shell.h"

/**
 * is_cmd - Determines if a file is an executable command.
 * @info: The info struct (unused in this function, but kept for prototype consistency).
 * @path: Path to the file.
 * Return: 1 if true, 0 otherwise.
 */
int is_cmd(info_t *info, char *path)
{
	struct stat st;

	(void)info; // Cast to void to suppress unused parameter warning
	if (!path || stat(path, &st)) // stat() attempts to get file status
		return (0);

	if (st.st_mode & S_IFREG) // Check if it's a regular file
	{
		return (1);
	}
	return (0);
}

/**
 * dup_chars - Duplicates characters from a string within a given range.
 * Used for extracting path components.
 * @pathstr: The PATH string (e.g., "/usr/local/bin:/usr/bin").
 * @start: Starting index for duplication.
 * @stop: Stopping index (exclusive) for duplication.
 * Return: Pointer to a new buffer containing the duplicated characters.
 */
char *dup_chars(char *pathstr, int start, int stop)
{
	static char buf[1024]; // Using static buffer to avoid malloc/free overhead for small strings
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':') // Exclude path delimiters
			buf[k++] = pathstr[i];
	buf[k] = 0; // Null-terminate the duplicated string
	return (buf);
}

/**
 * find_path - Finds the full path of a command in the PATH string.
 * @info: The info struct.
 * @pathstr: The PATH environment variable string.
 * @cmd: The command to find.
 * Return: Full path of cmd if found, or NULL if not found.
 */
char *find_path(info_t *info, char *pathstr, char *cmd)
{
    int i = 0, curr_pos = 0;
    char *path;

    if (!pathstr)
        return (NULL);

    if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
    {
        if (is_cmd(info, cmd))
        {
            return (cmd);
        }
    }

    while (1)
    {
        if (!pathstr[i] || pathstr[i] == ':')
        {
            path = dup_chars(pathstr, curr_pos, i);

            if (!*path)
                _strcat(path, cmd);
            else
            {
                _strcat(path, "/");
                _strcat(path, cmd);
            }
            if (is_cmd(info, path))
            {
                return (path);
            }
            if (!pathstr[i])
                break;
            curr_pos = i + 1;
        }
        i++;
    }
    return (NULL);
}

/* --- The find_cmd function from original shell_loop.c is integrated below --- */

/**
 * find_cmd - Finds a command in PATH or checks if it's an absolute/relative path.
 * Updates info->path with the resolved path.
 * @info: The parameter & return info struct.
 * Return: void
 */
void find_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0]; // Assume command is first argument
	if (info->linecount_flag == 1) // If counting lines for history/errors
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	// Check if command line is empty or just whitespace
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delimiter(info->arg[i], " \t\n")) // is_delimiter in shell_core/shell_utilities.c
			k++;
	if (!k) // If no non-delimiter characters found (empty command)
		return;

	// Try to find command in PATH
	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
	}
	else
	{
		if ((is_interactive(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
		{
			// No fork_cmd here, the main loop handles it.
		}
		else if (*(info->arg) != '\n') // If it's not just a newline and not found
		{
			info->status = 127; // Command not found status
			print_error(info, "not found\n"); // print_error in error_handling/error_reporter.c
		}
	}
}
