#include "../includes/shell.h"

/**
 * get_history_file - Constructs the full path to the shell's history file.
 * The history file is typically located in the user's home directory.
 * @info: Parameter struct containing shell information.
 * Return: An allocated string containing the full path to the history file, or NULL on error.
 */
char *get_history_file(info_t *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME="); // _getenv in environment/env_manager.c
	if (!dir)
		return (NULL);

	// Allocate memory for the full path: HOME + "/" + HIST_FILE + "\0"
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2)); // _strlen in string_operations/string_manipulation1.c
	if (!buf)
		return (NULL);

	buf[0] = 0; // Initialize buffer
	_strcpy(buf, dir); // _strcpy in string_operations/string_manipulation2.c
	_strcat(buf, "/"); // _strcat in string_operations/string_manipulation1.c
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_history - Creates a history file, or appends to an existing file,
 * writing the current command history to it.
 * @info: The parameter struct containing the history linked list.
 * Return: 1 on success, else -1.
 */
int write_history(info_t *info)
{
	ssize_t fd;
	char *filename = get_history_file(info); // Get history file path
	list_t *node = NULL;

	if (!filename)
		return (-1);

	// Open file: create if not exists, truncate if exists, read/write permissions
	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename); // Free filename after opening
	if (fd == -1)
		return (-1);

	// Iterate through history list and write each command to file
	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, fd); // _putsfd in error_handling/error_reporter.c
		_putfd('\n', fd);       // _putfd in error_handling/error_reporter.c
	}
	_putfd(BUF_FLUSH, fd); // Flush any remaining buffer content
	close(fd);
	return (1);
}

/**
 * read_history - Reads command history from the history file and populates the linked list.
 * @info: The parameter struct where history will be stored.
 * Return: The number of history entries read on success, 0 otherwise.
 */
int read_history(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st; // For file status (size)
	char *buf = NULL, *filename = get_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1) // File doesn't exist or cannot be opened
		return (0);

	// Get file size to allocate buffer
	if (!fstat(fd, &st)) // fstat gets file status using its file descriptor
		fsize = st.st_size;
	if (fsize < 2) // Minimum size for meaningful history (e.g., "a\n")
		return (0);

	buf = malloc(sizeof(char) * (fsize + 1)); // Allocate buffer for file content + null terminator
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize); // Read file content into buffer
	buf[fsize] = 0; // Null-terminate the buffer
	if (rdlen <= 0) // Read error or empty file
		return (free(buf), 0);
	close(fd);

	// Parse buffer by newlines and build history list
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0; // Replace newline with null terminator
			build_history_list(info, buf + last, linecount++); // Add entry to list
			last = i + 1; // Move 'last' to start of next line
		}
	if (last != i) // Handle case where last line doesn't end with newline
		build_history_list(info, buf + last, linecount++);
	free(buf); // Free buffer after parsing

	info->histcount = linecount; // Update total history count
	// Trim history if it exceeds HIST_MAX
	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0); // Remove oldest entry
	renumber_history(info); // Renumber nodes after trimming
	return (info->histcount);
}

/**
 * build_history_list - Adds an entry to the command history linked list.
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 * @buf: The command string to add to history.
 * @linecount: The history line number to assign to this entry.
 * Return: Always 0.
 */
int build_history_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history) // If history list already exists, get its end
		node = info->history;
	add_node_end(&node, buf, linecount); // add_node_end in linked_lists/list_operations.c

	if (!info->history) // If it was an empty list, new node becomes the head
		info->history = node;
	return (0);
}

/**
 * renumber_history - Renumbers the history linked list after changes
 * (e.g., additions or deletions). Ensures sequential numbering.
 * @info: Structure containing potential arguments.
 * Return: The new total history count.
 */
int renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++; // Assign sequential number
		node = node->next;
	}
	return (info->histcount = i); // Update info->histcount
}
