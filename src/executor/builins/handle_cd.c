#include "minishell.h"

uint8_t	handle_cd(t_cmd *cmd)
{
	uint8_t	exit_status;
	char	*path;

	// if (!cmd->argv[1])
	// 	path = $HOME  // replace with func
	// else
	path = cmd->argv[1];
	if (chdir(path) != 0)
	{
		cmd_error_handler(cmd);
		exit_status = EXIT_FAILURE;
	}
	else
	{
		exit_status = EXIT_SUCCESS;
	}
	cmd->minishell->exit_status = exit_status;

	printf("Exit status: %d\n",cmd->minishell->exit_status); //  test
	if (cmd->in_pipe)
		exit (exit_status);
	return (exit_status);
}
