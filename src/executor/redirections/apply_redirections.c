#include "minishell.h"

uint8_t	apply_redirections(t_cmd *cmd)
{
	if (!cmd)
	{
		print_error("minishell: cmd struct not found\n");
		return (EXIT_FAILURE);
	}
	if (cmd->in_redir && cmd->in_redir->type == R_HEREDOC)
		return (apply_heredoc(cmd));

	else if (cmd->in_redir && cmd->in_redir->type == R_INPUT)
		return (apply_in_redirection(cmd));

	else if (cmd->out_redir && cmd->out_redir->type == R_APPEND)
		return (apply_out_redirection(cmd));

	else if (cmd->out_redir && cmd->out_redir->type == R_OUTPUT)
		return (apply_out_redirection(cmd));

	else
		return (EXIT_SUCCESS);
}
