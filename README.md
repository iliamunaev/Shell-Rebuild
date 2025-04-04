# Wiki Shell-Rebuild
- [Home](https://github.com/iliamunaev/Shell-Rebuild/wiki): Basic commands for contributors

- [Data Structure](https://github.com/iliamunaev/Shell-Rebuild/wiki/Data-Structure): data structures used in the project

- [Functions](https://github.com/iliamunaev/Shell-Rebuild/wiki/Functions): the scope of functions might be used in the project

- [Git Commit Message Standard](https://github.com/iliamunaev/Shell-Rebuild/wiki/Git-Commit-Message-Standard): rules for clean commit messages

- [Processes Analyzing Shell Commands](https://github.com/iliamunaev/Shell-Rebuild/wiki/Process-Management-Shell-Commands)

### Error tests
```bash
valgrind --track-fds=yes ./minishell
```
```bash
valgrind  --leak-check=full ./minishell
 ```
### Testers to try with
```bash
https://github.com/LucasKuhn/minishell_tester
```
```bash
https://github.com/zstenger93/42_minishell_tester
```


## Use supression file to avoid readline() leaks
```bash
valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
```
