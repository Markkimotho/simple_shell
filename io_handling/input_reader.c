#include "../includes/shell.h"

/**
 * input_buf - Buffers chained commands by reading from standard input.
 * @info: Parameter struct.
 * @buf: Address of buffer where input will be stored.
 * @len: Address of len variable (stores buffer size).
 * Return: Bytes read on success, -1 on EOF or error.
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* If nothing left in the buffer, fill it */
	{
		// bfree((void **)info->cmd_buf); // This line suggests freeing cmd_buf here.
		// It might be better handled in free_info or specific command chaining logic
		free(*buf); // Free previous buffer if any
		*buf = NULL;
		signal(SIGINT, sigintHandler); // Set up Ctrl-C handler

#if USE_GETLINE // Use system getline or custom _getline
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(info, buf, &len_p); // _getline is defined in this file
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* Remove trailing newline */
				r--;
			}
			info->linecount_flag = 1; // Mark this line for counting
			remove_comments(*buf);    // remove_comments from error_handling/error_utilities.c
			build_history_list(info, *buf, info->histcount++); // build_history_list from history/history_manager.c
			/* if (_strchr(*buf, ';')) is this a command chain? */
			// The original logic for cmd_buf and len update was inside this block:
			// {
			// 	*len = r;
			// 	info->cmd_buf = buf;
			// }
			// This logic is better handled outside, in get_input to ensure cmd_buf is set for chaining
			*len = r;
			info->cmd_buf = buf; // Point info->cmd_buf to the buffer holding the chain
		}
	}
	return (r);
}

/**
 * get_input - Gets a line of input, handling command chaining.
 * Processes the buffer to extract one command at a time if chaining.
 * @info: Parameter struct.
 * Return: Bytes read (length of the current command), or -1 on EOF/error.
 */
ssize_t get_input(info_t *info)
{
	static char *buf; /* The ';' command chain buffer */
	static size_t i, j, len; // i: current position, j: iterator, len: buffer length
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p; // buf_p points to info->arg, p is pointer to current command start

	_putchar(BUF_FLUSH); // Flush any pending stdout output (_putchar in string_operations/string_manipulation2.c)
	r = input_buf(info, &buf, &len); // Read a new buffer if needed
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* We have commands left in the chain buffer */
	{
		j = i; /* Initialize new iterator to current buf position */
		p = buf + i; /* Get pointer for current command's start */

		check_chain(info, buf, &j, i, len); // Check if we should continue chaining (from variables/command_chaining.c)
		while (j < len) /* Iterate to semicolon or end of buffer */
		{
			if (is_chain(info, buf, &j)) // is_chain from variables/command_chaining.c
				break; // Found a chain delimiter
			j++;
		}

		i = j + 1; /* Increment past nulled ';' (or other delimiter) */
		if (i >= len) /* Reached end of buffer (no more commands in chain)? */
		{
			i = len = 0; /* Reset position and length for next read */
			info->cmd_buf_type = CMD_NORM; // Reset command type
		}

		*buf_p = p; /* Pass back pointer to current command position (info->arg) */
		return (_strlen(p)); /* Return length of current command */
	}

	*buf_p = buf; /* Else not a chain, pass back buffer from _getline() to info->arg */
	return (r); /* Return length of buffer from _getline() */
}

/**
 * read_buf - Reads a buffer from the current read file descriptor.
 * @info: Parameter struct containing the read file descriptor.
 * @buf: Buffer to read into.
 * @i: Pointer to a size_t variable to store the number of bytes read.
 * Return: Bytes read.
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i) // If there's already data in the buffer from previous read_buf call
		return (0);
	r = read(info->readfd, buf, READ_BUF_SIZE); // Read from the specified fd
	if (r >= 0)
		*i = r; // Update the count of bytes read
	return (r);
}

/**
 * _getline - Gets the next line of input from STDIN (or file).
 * Handles dynamic buffer reallocation if needed.
 * @info: Parameter struct.
 * @ptr: Address of pointer to buffer, preallocated or NULL.
 * @length: Size of preallocated ptr buffer if not NULL.
 * Return: Number of characters read (s), or -1 on EOF/error.
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE]; // Static buffer for reading chunks
	static size_t i, len;           // i: current position in buf, len: total bytes in buf
	size_t k;                       // Length of current line
	ssize_t r = 0, s = 0;           // r: bytes read, s: total characters in *ptr
	char *p = NULL, *new_p = NULL, *c; // p: current *ptr, new_p: reallocated buffer, c: newline char position

	p = *ptr;
	if (p && length)
		s = *length; // Get initial length if buffer is preallocated
	if (i == len) // If current buffer is exhausted
		i = len = 0; // Reset read buffer position and length

	r = read_buf(info, buf, &len); // Read new data into static buffer
	if (r == -1 || (r == 0 && len == 0)) // EOF or read error
		return (-1);

	// Find newline in the static buffer, or assume whole buffer is one line
	c = _strchr(buf + i, '\n'); // _strchr from string_operations/string_manipulation1.c
	k = c ? 1 + (unsigned int)(c - buf) : len; // Length to copy (including newline)

	// Reallocate *ptr to accommodate new line + existing data
	new_p = _realloc(p, s, s ? s + k : k + 1); // _realloc from memory_utils/memory_allocators.c
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? (free(p), -1) : -1); // Free old buffer if it existed

	if (s) // If *ptr already had content, concatenate
		_strncat(new_p, buf + i, k - i); // _strncat from string_operations/string_manipulation1.c
	else // Else, copy from scratch
		_strncpy(new_p, buf + i, k - i + 1); // _strncpy from string_operations/string_manipulation1.c

	s += k - i; // Update total length of line
	i = k;      // Update static buffer position
	p = new_p;  // Update pointer to new buffer

	if (length)
		*length = s; // Pass back new total length
	*ptr = p;    // Pass back pointer to the line
	return (s);  // Return length of the line
}

/**
 * sigintHandler - Signal handler for SIGINT (Ctrl-C).
 * Prints a new prompt and flushes output buffers.
 * @sig_num: The signal number (unused, but required for prototype).
 * Return: void.
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");    // _puts from string_operations/string_manipulation2.c
	_puts("$ ");
	_putchar(BUF_FLUSH); // _putchar from string_operations/string_manipulation2.c
}
