#include "../include/minishell.h"

void	minishell(char **envp)
{
	char	*input;
	t_cmd	*cmd; // linked list of commands

	while (1)
	{
		// Step 1: read input from terminal, return a line for parser
		input = readline("minishell:> "); // add some fearures like "username@hostname:> "

		// check for EOF / Ctrl+D
		if (!input)
		{
			printf("\nExit minishell.\n"); // set error func later
			break ;
		}

		// Step 2: add input to history
		if (*input)
			add_history(input);  // need to be freed?

		printf("Return: %s\n", input); // test print statment

		// Step 3: process input
		// use input from readline and return commands table for executor

		//cmd = run_parser(input);

		// Step 4: use command table and execute commands one by one, void func
		// if (cmd)
			//run_executor(cmd, envp);

		//	free_cmds(cmd);
		free(input);
	}
}

// Compile in src/ dir with
// cc -I../include main.c -lreadline
int	main(int argc, char **argv, char **envp)
{
	minishell(envp);
	return (EXIT_SUCCESS);
}
