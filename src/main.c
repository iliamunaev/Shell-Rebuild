#include "minishell.h"

uint8_t	run_script_mode(t_mshell *mshell, const char *file)
{
	char	*input;
	uint8_t	exit_status;
	t_cmd	*cmd;
	int		in_fd;

	in_fd = open(file, O_RDONLY);
    if (in_fd < 0)
    {
        print_error("minishell: cannot open script file\n");
        return (EXIT_FAILURE);
    }
	input = NULL;
    while ((input = get_next_line(in_fd)) != NULL)
    {
		cmd = run_parser(mshell, input);
		if (!cmd)
			continue ;
		exit_status = run_executor(cmd);
		free(input);
    }
    close(in_fd);
	return (exit_status);
}

uint8_t	run_interactive_mode(t_mshell *mshell)
{
	char	*input;
	uint8_t	exit_status;
	t_cmd	*cmd;

	while (1)
	{
		input = readline("minishell: ");

		printf("Input line: [%s]\n", input);

		// check for EOF / Ctrl+D
		if (!input)
			return (EXIT_FAILURE);

		// Step 2: add input to history
		if (*input)
			add_history(input);

		debug_printf("Return: %s\n", input); // test print statment

		// Step 3: process input
		// use input from readline and return commands table for executor
		cmd = run_parser(mshell, input);
		if (!cmd)
			continue ;

		exit_status = run_executor(cmd);
		// printf("run_minishell(): Exit status run_executor       : %d\n", exit_status);
		// printf("run_minishell(): Exit status mshell->exit_status: %d\n", mshell->exit_status);

		free(input);
	}
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_mshell	*minishell;
	uint8_t		exit_status;

	if (argc > 2)
	{
		print_error("Usage: ./minishell: [scriptfile] or no args.\n");
		return (EXIT_FAILURE);
	}
	setup_signal_handlers(); // Set up signal handlers
	minishell = init_mshell(envp);
	if (!minishell)
		return (EXIT_FAILURE);
	if (argc == 1)
		exit_status = run_interactive_mode(minishell);
	else
		exit_status = run_script_mode(minishell, argv[1]);
	free_minishell(minishell);
	rl_clear_history();
	return (exit_status);
}
