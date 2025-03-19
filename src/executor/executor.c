/**
 * @file executor.c
 * @brief Functions for executing built-in and external commands in Minishell.
 */
 #include "minishell.h"

 /**
  * @brief Checks if the given command is a built-in command.
  *
  * Iterates through the list of built-in commands stored in the Minishell
  * structure and compares the given command (`cmd->argv[0]`) against the list.
  *
  * @param cmd Pointer to the command structure containing the command details.
  * @return `true` if the command is a built-in, `false` otherwise.
  */
bool	is_builtin(t_cmd *cmd)
{
	char	**builtin;
	int		i;

	builtin = cmd->minishell->builtin;
	i = 0;
	while (builtin[i] && ft_strcmp(cmd->argv[0], builtin[i]) != 0)
		i++;
	return (builtin[i] != NULL);
}

/**
 * @brief Executes a command in Minishell.
 *
 * This function determines whether the command is a built-in or an external command:
 * - If the command is a built-in and has no following command (`cmd->next` is NULL),
 *   it is executed directly. If the built-in command is `exit`, the shell terminates.
 * - Otherwise, the command is executed as an external command.
 *
 * @param cmd Pointer to the command structure containing command details.
 * @return The exit status of the executed command.
 */
uint8_t run_executor(t_cmd *cmd)
{
	t_mshell	*minishell;
	uint8_t		exit_status;

	minishell = cmd->minishell;
	if (!minishell || !minishell->env || !minishell->hash_table)
	{
		print_error("Error (run_executor): missing components\n");
		return (EXIT_FAILURE);
	}
	// start test -------------------------------------------------------//
	int i = 0;
	while(cmd->argv[i])
	{
		printf("Debug: argv[%d]: {%s}\n", i, cmd->argv[i]);
		i++;
	}
	printf("Debug: argv[%d]: {%s}\n", i, cmd->argv[i]);
	if (cmd->in_redir)
		printf("Debug: filename: {%s}\n", cmd->in_redir->filename);
	if (cmd->out_redir)
		printf("Debug: filename: {%s}\n", cmd->out_redir->filename);
	// end test -------------------------------------------------------------//

	if (is_builtin(cmd) && !cmd->next)
	{
		minishell->exit_status = exec_builtin(cmd);
		if (ft_strcmp(cmd->argv[0], "exit") == 0)
		{
			exit_status = minishell->exit_status;
			free_cmd(cmd);
			free_minishell(cmd->minishell);
			rl_clear_history();
			exit(exit_status);
		}
		free_cmd(cmd);
		return (minishell->exit_status);
	}
	minishell->exit_status = exec_cmd(cmd);
	free_cmd(cmd);
	return (minishell->exit_status);
}

