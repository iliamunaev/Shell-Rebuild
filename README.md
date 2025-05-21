# Shell-Rebuild / Minishell

**Minishell** is a simplified Unix shell implemented in C. It replicates key features of `bash`, including command parsing, pipelines, redirections, environment variable handling, and built-in commands.


## Contributors
Ilia Munaev [LinkedIn](https://www.linkedin.com/in/iliamunaev/),
Pavel Vershinin [LinkedIn](https://www.linkedin.com/in/pavelvershinin/)


## Features

- Execution of simple and compound commands
- Custom Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Input/output redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Support for pipelines (`|`)
- Environment variable expansion (`$VAR`)
- Quoting rules:
  - Double quotes allow variable expansion
  - Single quotes preserve literal value
- Signal handling: `Ctrl+C`, `Ctrl+D`, `Ctrl+\`
- Command history using GNU Readline

---

## Implementation Highlights

- Written in **C**, with no external dependencies beyond standard custom librarie (libft) and `readline`
- Modular structure following a microservice-inspired architecture
- Custom tokenizer and parser
- Uses `fork()`, `execve()`, `pipe()`, and `dup2()` for process and I/O management
- Manages memory manually, verified with **Valgrind** for leak-free execution
- Suppression file for readline and external commands memory leaks included

---

## Minishell Structure (t_mshell)

The Minishell structure is the core of the Minishell. It keeps track of everything the shell needs while it's running.

It stores:

A copy of the environment variables (so the shell can access and modify them).

A hash table version of the environment for faster lookups.

The last command’s exit status (so $? works like in Bash).

The last syntax error status (useful for error handling).

A flag to detect if a memory allocation failed (to handle errors safely).

### Command Strunture (t_cmd)

In Minishell, every command you type is stored in a structured format using the t_cmd struct. This includes simple commands like ls or more complex ones like ls -l | grep minishell.

t_cmd is a single command in a pipeline, like one part of ls -l | grep foo | wc -l.

It contains:

- argv: The command and its arguments, as a string array. For example: ["ls", "-l", NULL].

- binary: The actual path to the executable (like /bin/ls), resolved before running.

- next: A pointer to the next command in a pipeline (if you're using |).

- minishell: A link back to the shell's main state (t_mshell), so each command can access environment variables, error handling, etc.

- redirs: A list of redirection rules (<, >, >>, <<) attached to the command.

- origin_head: A pointer to the start of the command chain—useful for keeping track of where the pipeline began.

### Redirection (t_redir)

Each redirection in a command (like < input.txt or >> output.log) is stored in a t_redir struct. It includes:

- Type of redirection (<, >, >>, <<)

- File name or heredoc delimiter

- Which file descriptor is being redirected (like stdin or stdout)

- Runtime FD used when executing

- Whether variable expansion should happen in heredoc mode

### Start minishell
```bash
make && ./minishell
```
### Clean
```bash
make fclean
```

## Error tests

### all erros + supp
```bash
valgrind --leak-check=full --trace-children=yes --gen-suppressions=all --show-leak-kinds=all --track-fds=yes  --error-limit=no --suppressions=mshell.supp ./minishell
```

### all errors
```bash
valgrind --leak-check=full  --track-origins=yes  --trace-children=yes --gen-suppressions=all --show-leak-kinds=all --track-fds=yes --error-limit=no  ./minishell
```
