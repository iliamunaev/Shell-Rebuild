/**
 * @file executor.c
 * @brief Functions for executing built-in and external commands in Minishell.
 */
#include "minishell.h"

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
 * @brief Executes a command, either built-in or external.
 *
 * - If the command is built-in and there is no following command, executes it directly.
 * - Otherwise, marks `cmd->in_pipe` as `true` and executes the command externally.
 *
 * @param cmd Pointer to the command structure.
 * @return The exit status of the command.
 */
uint8_t run_executor(t_cmd *cmd)
{
	t_mshell	*minishell;

	minishell = cmd->minishell;
	if (!minishell || !minishell->env || !minishell->hash_table)
	{
		print_error("Error (run_executor): missing components\n");
		return (EXIT_FAILURE);
	}
	cmd->in_pipe = false;
	if (is_builtin(cmd) && !cmd->next)
		return (exec_builtin(cmd));
	cmd->in_pipe = true;
	exec_cmd(cmd);
	printf("Exit status: %d\n",cmd->minishell->exit_status); //  test
	return (cmd->minishell->exit_status);
}
