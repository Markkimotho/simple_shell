#include "../includes/shell.h"

/**
 * is_interactive - Returns true if the shell is in interactive mode.
 * @info: Struct address containing shell information.
 * Return: 1 if interactive mode, 0 otherwise.
 */
int is_interactive(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_delimiter - Checks if a character is a delimiter.
 * @c: The character to check.
 * @delim: The delimiter string.
 * Return: 1 if true, 0 if false.
 */
int is_delimiter(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 * is_alpha_char - Checks for an alphabetic character.
 * @c: The character to input.
 * Return: 1 if c is alphabetic, 0 otherwise.
 */
int is_alpha_char(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * string_to_int - Converts a string to an integer.
 * @s: The string to be converted.
 * Return: 0 if no numbers in string, converted number otherwise.
 */
int string_to_int(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}

/**
 * clear_info - Initializes info_t struct members to NULL or 0.
 * @info: Struct address.
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
	// Reset other relevant fields for a new command cycle if needed
	info->cmd_buf = NULL;
	info->cmd_buf_type = CMD_NORM;
	info->readfd = 0; // Default to stdin
	info->linecount_flag = 0;
	info->err_num = 0;
	// histcount, env, history, alias, environ, env_changed, status remain across commands
}

/**
 * set_info - Initializes info_t struct members from argument vector.
 * Parses the raw argument string into an array of strings.
 * @info: Struct address.
 * @av: Argument vector from main().
 */
void set_info(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0]; // Program name
	if (info->arg) // If there's a command line string from input_reader
	{
		// Tokenize the argument string into argv
		info->argv = strtow(info->arg, " \t"); // strtow from string_operations/string_tokenization.c
		if (!info->argv) // If tokenization fails (e.g., only spaces)
		{
			info->argv = malloc(sizeof(char *) * 2); // Allocate for at least argv[0] and NULL
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg); // _strdup from string_operations/string_manipulation2.c
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			; // Count argc
		info->argc = i;

		replace_alias(info); // replace_alias from variables/variable_expansion.c
		replace_vars(info);   // replace_vars from variables/variable_expansion.c
	}
}

/**
 * free_info - Frees fields of info_t struct.
 * @info: Struct address.
 * @all: True if freeing all fields (including persistent ones like env, history, alias lists).
 */
void free_info(info_t *info, int all)
{
	ffree(info->argv); // ffree from memory_utils/memory_allocators.c
	info->argv = NULL;
	info->path = NULL; // path is usually a pointer to memory managed by find_path or argv[0]

	if (all) // Free all fields, including persistent lists
	{
		if (!info->cmd_buf) // If cmd_buf wasn't managed by chaining, free info->arg
			free(info->arg);
		if (info->env)
			free_list(&(info->env)); // free_list from linked_lists/list_operations.c
		if (info->history)
			free_list(&(info->history));
		if (info->alias)
			free_list(&(info->alias));
		ffree(info->environ); // Free the char** array copy of environ
		info->environ = NULL;
		bfree((void **)info->cmd_buf); // bfree from memory_utils/memory_allocators.c
		if (info->readfd > 2) // Close non-standard input file descriptors
			close(info->readfd);
		_putchar(BUF_FLUSH); // Flush any remaining buffered output
	}
}
