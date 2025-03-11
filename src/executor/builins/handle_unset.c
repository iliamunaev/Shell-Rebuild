/**
 * @file handle_unset.c
 * @brief Functions for handling the `unset` built-in command in Minishell.
 */
#include "minishell.h"

uint8_t	handle_unset(t_cmd *cmd)
{
	int		i;
	uint8_t	exit_status;
	uint8_t	ret;

	if (!cmd->argv[1])
	{
		print_export_from_ht(cmd->minishell);
		cmd->minishell->exit_status = EXIT_SUCCESS;
		if (cmd->in_pipe)
			exit(EXIT_SUCCESS);
		return (EXIT_SUCCESS);
	}
	exit_status = (EXIT_SUCCESS);
	i = 1;
	while (cmd->argv[i])
	{
		ret = process_export_arg(cmd, cmd->argv[i]);
		if (ret != EXIT_SUCCESS)
			exit_status = ret;
		i++;
	}
	update_env(cmd->minishell);

	printf("Exit status: %d\n",cmd->minishell->exit_status); //  test
	if (cmd->in_pipe)
		exit(exit_status);
	return (exit_status);
}
