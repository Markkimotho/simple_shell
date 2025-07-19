#include "../includes/shell.h"

/**
 * find_builtin - Finds and executes a built-in command.
 * @info: The parameter & return info struct.
 * Return: -1 if built-in not found,
 * 0 if built-in executed successfully,
 * 1 if built-in found but not successful,
 * -2 if built-in signals exit().
 */
int find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},                 // from builtins/builtin_exit.c
		{"env", _myenv},                   // from builtins/builtin_env.c 
		{"help", _myhelp},                 // from builtins/builtin_help.c
		{"history", _myhistory},           // from builtins/builtin_history_alias.c
		{"setenv", _mysetenv},             // from builtins/builtin_env.c 
		{"unsetenv", _myunsetenv},         // from builtins/builtin_env.c 
		{"cd", _mycd},                     // from builtins/builtin_cd.c
		{"alias", _myalias},               // from builtins/builtin_history_alias.c
        {NULL, NULL},                       // Sentinel to mark the end of the table
        {"", NULL},                         // Empty entry for compatibility
        {"\n", NULL},                       // Newline entry for compatibility
        {";", NULL},                        // Semicolon entry for compatibility
        {"&&", NULL},                       // Logical AND entry for compatibility
        {"||", NULL},                       // Logical OR entry for compatibility
        {"\0", NULL},                       // Null entry for compatibility
        {"exit\n", _myexit},               // Exit with newline for compatibility
        {"env\n", _myenv},                 // Environment with newline for compatibility
        {"help\n", _myhelp},               // Help with newline for compatibility
        {"history\n", _myhistory},         // History with newline for compatibility
        {"setenv\n", _mysetenv},           // Setenv with newline for compatibility
        {"unsetenv\n", _myunsetenv},       // Unsetenv with newline for compatibility
        {"cd\n", _mycd},                   // Change directory with newline for compatibility
        {"alias\n", _myalias},             // Alias with newline for compatibility
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}
