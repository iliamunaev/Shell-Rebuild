#include "minishell.h"

uint8_t	apply_redirections(t_cmd *cmd)
{

	// if (!cmd)
	// {
	// 	print_error("minishell: cmd struct not found\n");
	// 	return (EXIT_FAILURE);
	// }

    if (cmd->in_redir && cmd->in_redir->type == R_HEREDOC)
    {
        // printf("---R_HEREDOC found\n"); // test
        return (apply_heredoc(cmd));
    }


	// else if (cmd->in_redir->type == R_INPUT)
	// 	return (handle_input_redirectiion(cmd));
	// else if (cmd->out_redir->type == R_APPEND)
	// 	return (handle_append_redirection(cmd));
	// else if (cmd->out_redir->type == R_OUTPUT)
	// 	return (handle_out_redirection(cmd));

	// printf("---No redirectons found\n"); // test
	return (EXIT_SUCCESS);
}
