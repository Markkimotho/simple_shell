# Simple Shell

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C Language](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Shell Type](https://img.shields.io/badge/Type-UNIX%20Shell-green.svg)](https://en.wikipedia.org/wiki/Unix_shell)

## Table of Contents

- [About The Project](#about-the-project)
  - [Features](#features)
  - [Goals & Enhancements](#goals--enhancements)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Compilation](#compilation)
  - [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [External Commands](#external-commands)
- [Process Management](#process-management)
- [File I/O and Redirections](#file-io-and-redirections)
- [Environment Variables](#environment-variables)
- [Command Chaining](#command-chaining)
- [Memory Management](#memory-management)
- [File Structure](#file-structure)
- [Contributing](#contributing)
- [License](#license)
- [Authors](#authors)
- [Acknowledgments](#acknowledgments)

---

## About The Project

"My Simple Shell" is a custom-built UNIX command line interpreter developed in C. This project aims to replicate the fundamental functionalities of standard shells like Bash, providing a hands-on understanding of low-level systems programming concepts.

It supports executing both external programs and a set of custom built-in commands, managing processes, and handling environment variables. This project serves as a foundational step in mastering systems programming in a UNIX-like environment.

### Features

* **Command Execution:** Execute programs available in the system's `PATH`.
* **Built-in Commands:** Support for a core set of internal shell commands.
* **Process Management:** Handles process creation (`fork()`) and execution (`execve()`) for commands.
* **Environment Variables:** Access, set, and unset environment variables within the shell's context.
* **Command Chaining:** Support for sequential (`;`), logical AND (`&&`), and logical OR (`||`) command execution.
* **History:** Basic command history management.
* **Alias Management:** Create and use custom command aliases.
* **Error Handling:** Basic error reporting for command not found, permission denied, etc.
* **Prompt with CWD:** Displays the current working directory in the shell prompt.

### Goals & Enhancements

This project is continuously evolving. Planned and future enhancements include:

* **File I/O Redirection:** Implement `>`, `>>`, and `<` operators for standard input/output redirection.
* **Piping (`|`):** Support for connecting the output of one command to the input of another.
* **Advanced Variable Expansion:** More robust handling of `$?`, `$$`, and custom variable expansion.
* **Interactive Line Editing (Future Consideraton):** Basic interactive features like `Ctrl+C` handling (already done) and potentially basic cursor movement, though full tab-completion is out of scope for a "simple shell" without a dedicated library like `readline`.
* **Background Processes:** Basic job control (running commands in the background).

---

## Getting Started

To get a local copy up and running, follow these simple steps.

### Prerequisites

You will need a C compiler (like GCC) and standard UNIX development tools.

`gcc`

```bash
sudo apt update
sudo apt install build-essential # For Debian/Ubuntu

# Or for macOS with Homebrew:
xcode-select --install
brew install gcc
````

### Compilation

#### Option 1: Using a Makefile (Recommended for modular projects)

Navigate to the root directory of the project in your terminal and compile using the provided `Makefile`:

```bash
make
```

Upon successful compilation, an executable file named `hsh` will be created in your project root.

#### Makefile Usage Guide

The `Makefile` provides several convenient targets to manage your project:

  * **`make all` (or just `make`)**:

      * Compiles all source files and links them to create the `hsh` executable.
      * Creates a `build/` directory for object files (`.o`) if it doesn't exist.
      * Example:
        ```bash
        make
        ```

  * **`make clean`**:

      * Removes all compiled object files (`.o`) and the `build/` directory.
      * Does *not* remove the `hsh` executable.
      * Example:
        ```bash
        make clean
        ```

  * **`make fclean`**:

      * Performs a `clean` operation.
      * Additionally removes the `hsh` executable.
      * Example:
        ```bash
        make fclean
        ```

  * **`make re`**:

      * Performs a `fclean` (full clean) operation.
      * Then performs an `all` (full build) operation.
      * Useful for a fresh rebuild from scratch.
      * Example:
        ```bash
        make re
        ```

  * **`make debug`**:

      * Compiles the project with debugging symbols (`-g`).
      * Useful when you need to use a debugger like `gdb` to step through your code.
      * Example:
        ```bash
        make debug
        gdb ./hsh
        ```


#### Option 2: Direct GCC command
**Ensure you are in the directory containing `main.c` and the subdirectories**
```bash
gcc -Wall -Wextra -Werror -pedantic \
main.c \
shell_core/*.c \
builtins/*.c \
environment/*.c \
io_handling/*.c \
memory_utils/*.c \
linked_lists/*.c \
string_operations/*.c \
error_handling/*.c \
history/*.c \
variables/*.c \
-o hsh
```

### Usage

Run the shell executable:

```bash
./hsh
```

You will see a prompt (e.g., ` current_dir_name$  `). You can then type commands:

**Example**

```bash
Documents$ ls
main.c  shell_core  builtins  ... (your project files)
Documents$ pwd
/Users/ktinega/Documents/My_Projects/simple_shell
Documents$ cd ..
My_Projects$ pwd
/Users/ktinega/Documents/My_Projects
My_Projects$ env
USER=ktinega
PATH=/usr/local/bin:...
...
My_Projects$ echo Hello Shell
Hello Shell
My_Projects$ exit 0
```

To execute a script file:

```bash
./hsh your_script.sh
```

-----

## Built-in Commands

The following commands are implemented directly within the `hsh` shell:

  * **`exit [status]`**: Exits the shell with an optional status code.
  * **`env`**: Prints the current environment variables.
  * **`setenv VARIABLE VALUE`**: Initializes a new environment variable, or modifies an existing one.
  * **`unsetenv VARIABLE`**: Removes an environment variable.
  * **`cd [directory]`**: Changes the current working directory. Supports `cd`, `cd .`, `cd ..`, `cd -`, and `cd /path/to/dir`.
  * **`alias [name[='value']]`**: Displays, sets, or unsets aliases.
      * `alias`: Lists all aliases.
      * `alias name`: Displays a specific alias.
      * `alias name='value'`: Sets or updates an alias.
  * **`history`**: Displays the command history list with line numbers.
  * **`help`**: Displays basic help information about shell usage.

-----

## External Commands

`hsh` can execute any executable program available in your system's `PATH`. This includes common UNIX utilities such as:

  * `ls`
  * `pwd`
  * `echo`
  * `cat`
  * `grep`
  * `find`
  * etc.

When you type an external command, `hsh` searches for it in the directories listed in your `PATH` environment variable.

-----

## Process Management

The shell uses `fork()` to create a new child process for each external command. The child process then uses `execve()` to replace itself with the new program. The parent process (your shell) uses `wait()` to pause execution until the child process completes, ensuring proper command sequencing.

-----

## File I/O and Redirections

*(This section will be expanded as I implement more of these features)*

Currently, `hsh` handles standard input from the terminal or a specified script file. Future development will include:

  * **Standard Output Redirection (`>`):** Directing command output to a file, overwriting its contents.
  * **Append Output Redirection (`>>`):** Directing command output to a file, appending to its contents.
  * **Standard Input Redirection (`<`):** Taking command input from a file.

-----

## Environment Variables

`hsh` maintains its own copy of the environment variables inherited from its parent process. These can be manipulated using the `env`, `setenv`, and `unsetenv` built-in commands. Environment variables are crucial for `hsh` to locate external commands (via `PATH`) and perform operations like changing the home directory (`HOME`).

Variable expansion (e.g., `$HOME`, `$PATH`, `$?` for last exit status, `$$` for shell PID) is supported.

-----

## Command Chaining

`hsh` supports the following command chaining operators:

  * **`;` (Semicolon):** Commands are executed sequentially, regardless of the success or failure of the preceding command.
    ```bash
    command1 ; command2 ; command3
    ```
  * **`&&` (Logical AND):** The command following `&&` is executed only if the preceding command succeeds (exits with a status of 0).
    ```bash
    mkdir my_new_dir && echo "Directory created successfully"
    ```
  * **`||` (Logical OR):** The command following `||` is executed only if the preceding command fails (exits with a non-zero status).
    ```bash
    cat non_existent_file || echo "File not found!"
    ```

-----

## Memory Management

The shell is meticulously designed with custom memory allocation and deallocation functions (`_realloc`, `bfree`, `ffree`, `_memset`) to minimize memory leaks, which are critical for long-running processes like a shell. All dynamically allocated memory (for command arguments, environment lists, history, etc.) is carefully freed when no longer needed or upon shell exit.

-----

## File Structure

The project is organized into modular directories for clarity and maintainability:

```
.
├── README.md                           # This file
├── main.c                              # Main entry point
├── shell_core/                         # Core shell loop, command execution, and general utilities
│   ├── command_executor.c
│   ├── command_parser.c
│   ├── shell_loop.c
│   └── shell_utilities.c
├── builtins/                           # Implementations of shell built-in commands
│   ├── builtin_cd.c
│   ├── builtin_env.c
│   ├── builtin_exit.c
│   ├── builtin_help.c
│   ├── builtin_history_alias.c
│   └── builtin_manager.c
├── environment/                        # Environment variable management and PATH resolution
│   ├── env_manager.c
│   └── path_resolver.c
├── io_handling/                        # Input/output reading, redirections, and pipes
│   ├── file_redirection.c
│   ├── input_reader.c
│   └── pipe_handler.c
├── memory_utils/                       # Custom memory allocation and string conversion utilities
│   ├── memory_allocators.c
│   └── string_converters.c
├── linked_lists/                       # Generic linked list operations and utilities
│   ├── list_operations.c
│   └── list_utilities.c
├── string_operations/                  # Various string manipulation and tokenization functions
│   ├── string_manipulation1.c
│   ├── string_manipulation2.c
│   └── string_tokenization.c
├── error_handling/                     # Error reporting and utility functions
│   ├── error_reporter.c
│   └── error_utilities.c
├── includes/                           # Central header file with structs, macros, and prototypes
│   └── shell.h
├── history/                            # Command history management
│   └── history_manager.c
├── variables/                          # Command chaining and variable/alias expansion logic
│   ├── command_chaining.c
│   └── variable_expansion.c
└── tests/                              # Directory for test scripts and files (future)
    └── run_tests.sh
```

-----

## Contributing

Contributions are what make the open-source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star\! Thanks again\!

1.  Fork the Project
2.  Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3.  Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4.  Push to the Branch (`git push origin feature/AmazingFeature`)
5.  Open a Pull Request

-----

## License

Distributed under the MIT License. See `LICENSE` for more information. (You might want to create an actual `LICENSE` file in your repo if you haven't).

-----

## Authors

  * **GitHub Username(s)**
      * [Mark Tinega](https://github.com/Markkimotho) - Initial work

-----

## Acknowledgments

  * [Holberton School](https://www.holbertonschool.com/) - For providing the foundational project idea and guiding curriculum.
  * [Alx Africa](https://www.alxafrica.com/) - For the rigorous program.
  * [Stack Overflow](https://stackoverflow.com/)
  * [The C Programming Language (K\&R)](https://en.wikipedia.org/wiki/The_C_Programming_Language)
  * And many other online resources and documentation.

-----