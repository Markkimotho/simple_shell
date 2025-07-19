#include "../includes/shell.h"

/**
 * is_chain - Tests if the current character in the buffer is a chain delimiter (;, &&, ||).
 * @info: The parameter struct.
 * @buf: The character buffer holding the command line.
 * @p: Address of the current position in buf.
 * Return: 1 if it's a chain delimiter, 0 otherwise.
 */
int is_chain(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|') // Logical OR
	{
		buf[j] = 0; // Null-terminate current command
		j++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&') // Logical AND
	{
		buf[j] = 0; // Null-terminate current command
		j++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';') /* Found end of this command */
	{
		buf[j] = 0; /* Replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0); // Not a chain delimiter
	*p = j; // Update buffer position to after the delimiter
	return (1);
}

/**
 * check_chain - Checks if command chaining should continue based on the last command's status.
 * Used for '&&' (execute next if previous succeeded) and '||' (execute next if previous failed).
 * @info: The parameter struct containing the last command's status and chain type.
 * @buf: The command line character buffer.
 * @p: Address of current position in buf.
 * @i: Starting position in buf (usually the beginning of the current command).
 * @len: Total length of buf.
 * Return: Void.
 */
void check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p; // Current position in the buffer

	if (info->cmd_buf_type == CMD_AND) // If previous was '&&'
	{
		if (info->status) // And previous command failed
		{
			buf[i] = 0; // Nullify the current command (effectively skip it)
			j = len;    // Advance pointer to end of buffer to stop processing chain
		}
	}
	if (info->cmd_buf_type == CMD_OR) // If previous was '||'
	{
		if (!info->status) // And previous command succeeded
		{
			buf[i] = 0; // Nullify the current command (effectively skip it)
			j = len;    // Advance pointer to end of buffer to stop processing chain
		}
	}

	*p = j; // Update the buffer position
}
