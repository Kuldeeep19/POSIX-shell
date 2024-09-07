# POSIX Shell Implementation

This project is a custom POSIX-like shell implemented in C++. It provides a range of functionalities including built-in commands, file redirection for input and output, background process management, command piping, and directory searching capabilities.

## Features
- **Built-in Commands**: `cd`, `pwd`, `echo`, `ls`, `pinfo`, and history tracking.
- **Input/Output Redirection**: Support for `<`, `>`, and `>>` operators.
- **Piping**: Ability to pipe the output of one command into another.
- **Background Execution**: Commands can be run in the background using `&`.
- **Directory Search**: Recursive search for files within directories.



## File Descriptions

1. `main.cpp`
Purpose: Entry point of the shell program.

Logic:
Initializes and runs a loop to continuously read and process user input.
Handles built-in commands, pipes, redirections, and background processes.
Manages the execution flow and integrates other functionalities like history and directory operations.

2. `execute.cpp`
Purpose: Executes external commands and handles command piping.

Logic:
Parses input commands, checks for redirections, and handles command execution.
Supports piping commands by creating pipes, forking processes, and redirecting input/output streams between them.

3. `back.cpp`
Purpose: Handles background process execution.

Logic:
Forks a new process to execute the command.
If the command is to be run in the background, it prints the PID of the background process.
Waits for the process to complete if not running in the background.

4. `cd.cpp`
Purpose: Implements the cd command to change directories.

Logic:
Handles various cases for directory navigation including ~, -, and absolute/relative paths.
Updates the OLDPWD environment variable to keep track of the previous directory.

5. `echo.cpp`
Purpose: Implements the echo command to display text.

Logic:
Handles and processes different types of input including text enclosed in quotes and special characters.
Supports background process handling indicated by &.

6. `his.cpp`
Purpose: Manages and displays command history.

Logic:
Add History: Stores executed commands in a history file, avoids consecutive duplicates, and limits the history to a maximum of 20 commands.
Show History: Displays the most recent commands from the history file.

7.` ls.cpp`
Purpose: Implements the ls command to list directory contents.

Logic:
Supports listing files with options like -a (show all) and -l (long format).
Displays file permissions, ownership, size, and modification time for long format listing.

8. `pinfofile.cpp`
Purpose: Provides process information similar to the pinfo command.

Logic:
Reads and displays process information from the /proc/[pid] directory.
Shows process status, memory usage, and executable path.

9. `pipe.cpp`
Purpose: Handles command piping.

Logic:
Creates pipes for connecting multiple commands.
Forks processes and sets up input/output redirection according to the piping requirements.
Executes commands in a piped sequence.

10. `pwd.cpp`
Purpose: Implements the pwd command to print the current working directory.

Logic:
Retrieves and displays the current working directory.
Replaces the home directory prefix with / for a more user-friendly output.

11. `redirection.cpp`
Purpose: Handles input/output redirection for commands.

Logic:
Supports < (input redirection), > (output redirection), and >> (output append).
Redirects input/output streams based on the specified redirection operators.

12.` search.cpp`
Purpose: Provides functionality to search for a file or directory.

Logic:
Recursively searches through directories to find a file with a specified name.

13. `makefile`
Purpose: Automates the build process for the shell project.

Logic:
Build Rule: Compiles all C++ source files into an executable named main.
Clean Rule: Removes the main executable


