#include "minishell.h"

uint8_t	apply_in_redirection(t_cmd *cmd)
{
	int	in_fd;

	if (pre_exec_validation(cmd, R_INPUT) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	in_fd = open(cmd->in_redir->filename, O_RDONLY);

	if (in_fd < 0)
	{
		print_error("-minishell: ");
		perror(cmd->in_redir->filename);
		return (EXIT_FAILURE);
	}
	if (dup2(in_fd, STDIN_FILENO) == -1)
	{
		if (close(in_fd) == -1)
			perror_return("-minishell: close",EXIT_FAILURE);
		perror_return("-minishell: dup2", EXIT_FAILURE);
	}
	if (close(in_fd) == -1)
		perror_return("-minishell: close", EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
// to check errno test
// fprintf(stderr, "close errno: %d (%s)\n", errno, strerror(errno));
