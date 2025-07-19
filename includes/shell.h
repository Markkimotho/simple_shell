#ifndef _SHELL_H_
#define _SHELL_H_

// Standard Library Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

// Macros for Read/Write Buffers
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

// Macros for Command Chaining Types
#define CMD_NORM    0
#define CMD_OR      1
#define CMD_AND     2
#define CMD_CHAIN   3

// Macros for convert_number() flags
#define CONVERT_LOWERCASE   1
#define CONVERT_UNSIGNED    2

// Feature Flags (for optional system calls vs custom implementations)
#define USE_GETLINE 0 // Set to 1 to use system getline(), 0 for custom _getline
#define USE_STRTOK 0  // Set to 1 to use system strtok(), 0 for custom tokenizer

// History File and Max History Size
#define HIST_FILE   ".simple_shell_history"
#define HIST_MAX    4096

// External Global Variable
extern char **environ; // Declares the global environment array

/**
 * struct liststr - singly linked list node for history, environment, or aliases
 * @num: The number field (e.g., history line number)
 * @str: A string associated with the node (e.g., environment variable string)
 * @next: Pointer to the next node in the list
 */
typedef struct liststr
{
    int num;
    char *str;
    struct liststr *next;
} list_t;

/**
 * struct passinfo - Contains pseudo-arguments and shell state information.
 * Allows uniform prototype for function pointer structs and
 * centralized access to shell data.
 * @arg: A string generated from getline, containing the raw command line input.
 * @argv: An array of strings (tokens) parsed from @arg, representing command and its arguments.
 * @path: The resolved full path for the current command if it's an external executable.
 * @argc: The number of arguments in @argv.
 * @line_count: The count of lines processed, used for error reporting.
 * @err_num: The error code to be used for exit() status.
 * @linecount_flag: Flag to indicate if the current line of input should be counted.
 * @fname: The program filename (e.g., "hsh").
 * @env: Pointer to the head of a linked list representing the shell's environment.
 * @environ: A null-terminated array of strings, a copy of the shell's environment.
 * @history: Pointer to the head of a linked list storing command history.
 * @alias: Pointer to the head of a linked list storing shell aliases.
 * @env_changed: Flag indicating if the environment has been modified.
 * @status: The return status of the last executed command.
 * @cmd_buf: Pointer to the command chain buffer, used when commands are chained (e.g., `cmd1 ; cmd2`).
 * @cmd_buf_type: Type of command chaining (CMD_NORM, CMD_OR, CMD_AND, CMD_CHAIN).
 * @readfd: The file descriptor from which to read input (0 for stdin, or a file for script execution).
 * @histcount: The current count of history entries.
 */
typedef struct passinfo
{
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int line_count;
    int err_num;
    int linecount_flag;
    char *fname;
    list_t *env;
    list_t *history;
    list_t *alias;
    char **environ;
    int env_changed;
    int status;

    char **cmd_buf; /* pointer to cmd ; chain buffer, for memory management */
    int cmd_buf_type; /* CMD_type ||, &&, ; */
    int readfd;
    int histcount;
} info_t;

// Macro for initializing info_t struct
#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0}

/**
 * struct builtin - Maps a built-in command string to its corresponding function.
 * @type: The string name of the built-in command (e.g., "exit", "env").
 * @func: A pointer to the function that implements the built-in command.
 */
typedef struct builtin
{
    char *type;
    int (*func)(info_t *);
} builtin_table;


// =======================================================
// Function Prototypes - Organized by New File Structure
// =======================================================

// shell_core/shell_loop.c
int hsh(info_t *, char **);
int loophsh(char **); // This function might become obsolete or integrated into hsh

// shell_core/command_executor.c
void fork_cmd(info_t *);

// shell_core/shell_utilities.c
int is_interactive(info_t *); // Corrected name
int is_delimiter(char, char *); // Corrected name
int is_alpha_char(int);       // Corrected name
int string_to_int(char *);    // Corrected name
void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);


// builtins/builtin_manager.c
int find_builtin(info_t *);

// builtins/builtin_exit.c
int _myexit(info_t *);

// builtins/builtin_cd.c
int _mycd(info_t *);

// builtins/builtin_help.c
int _myhelp(info_t *);

// builtins/builtin_history_alias.c
int _myhistory(info_t *);
int _myalias(info_t *);
int unset_alias(info_t *, char *); // Helper for alias
int set_alias(info_t *, char *);   // Helper for alias
int print_alias(list_t *);         // Helper for alias

// environment/env_manager.c
int _myenv(info_t *);
char *_getenv(info_t *, const char *);
int _mysetenv(info_t *);
int _myunsetenv(info_t *);
int populate_env_list(info_t *);
char **get_environ(info_t *); // from getenv.c
int _unsetenv(info_t *, char *); // from getenv.c (renamed from _unsetenv in getenv.c, if it differs)
int _setenv(info_t *, char *, char *); // from getenv.c (renamed from _setenv in getenv.c, if it differs)

// environment/path_resolver.c
int is_cmd(info_t *, char *);
char *dup_chars(char *, int, int);
char *find_path(info_t *, char *, char *);
void find_cmd(info_t *); // From shell_loop.c, related to path finding


// io_handling/input_reader.c
ssize_t input_buf(info_t *, char **, size_t *);
ssize_t get_input(info_t *);
ssize_t read_buf(info_t *, char *, size_t *);
int _getline(info_t *, char **, size_t *);
void sigintHandler(int);

// memory_utils/memory_allocators.c
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);
int bfree(void **);

// memory_utils/string_converters.c
char *convert_number(long int, int, int);

// linked_lists/list_operations.c
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

// linked_lists/list_utilities.c
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
size_t print_list_str(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

// string_operations/string_manipulation1.c
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);
char *_strncpy(char *, char *, int); 
char *_strncat(char *, char *, int); 
char *_strchr(char *, char);         
char *_strrchr(char *, char);

// string_operations/string_manipulation2.c
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

// string_operations/string_tokenization.c
char **strtow(char *, char *);
char **strtow2(char *, char);

// error_handling/error_reporter.c
void _eputs(char *);
int _eputchar(char);
int _putfd(char c, int fd);
int _putsfd(char *str, int fd);

// error_handling/error_utilities.c
int _erratoi(char *);
void print_error(info_t *, char *);
int print_d(int, int);
void remove_comments(char *);

// history/history_manager.c
char *get_history_file(info_t *info);
int write_history(info_t *info);
int read_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);

// variables/command_chaining.c
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);

// variables/variable_expansion.c
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);


#endif // _SHELL_H_
