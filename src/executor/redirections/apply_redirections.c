/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:19:19 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/04/30 18:56:37 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_file_input_redirection(t_redir *r)
{
	int	fd;

	fd = open(r->filename, O_RDONLY);
	if (fd == -1)
		return (perror_return(r->filename, REDIR_ERR));
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		safe_close(&fd);
		return (perror_return("in dup2", REDIR_ERR));
	}
	safe_close(&fd);
	return (EXIT_SUCCESS);
}

static int	handle_output_redirection(t_redir *r, int *last_out_fd)
{
	int	flags;
	int	fd;

	safe_close(last_out_fd);
	flags = O_WRONLY | O_CREAT;
	if (r->type == R_APPEND)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(r->filename, flags, 0644);
	if (fd == -1)
		return (perror_return(r->filename, REDIR_ERR));
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		safe_close(&fd);
		return (perror_return("out dup2", REDIR_ERR));
	}
	safe_close(&fd);
	return (EXIT_SUCCESS);
}

static int	handle_input_redirection(t_redir *r, int *last_in_fd)
{
	safe_close(last_in_fd);
	if (r->type == R_HEREDOC)
		return (handle_heredoc_redirection(r));
	else
		return (handle_file_input_redirection(r));
}

// static int	handle_output_redirection_wrapper(t_redir *r, int *last_out_fd)
// {
// 	safe_close(last_out_fd);
// 	if (handle_output_redirection(r) == REDIR_ERR)
// 		return (REDIR_ERR);
// 	// *last_out_fd = -1;
// 	return (0);
// }

uint8_t	apply_redirections(t_cmd *cmd)
{
	t_list	*node;
	int		last_in_fd;
	int		last_out_fd;
	t_redir	*r;

	node = cmd->redirs;
	last_in_fd = -1;
	last_out_fd = -1;
	while (node)
	{
		r = node->content;
		if (r->type == R_INPUT || r->type == R_HEREDOC)
		{
			if (handle_input_redirection(r, &last_in_fd) == REDIR_ERR)
				return (EXIT_FAILURE);
		}
		else if (r->type == R_OUTPUT || r->type == R_APPEND)
		{
			if (handle_output_redirection(r, &last_out_fd) == REDIR_ERR)
				return (EXIT_FAILURE);
		}
		node = node->next;
	}
	return (EXIT_SUCCESS);
}

// uint8_t	apply_redirections(t_cmd *cmd)
// {
// 	t_list	*node;
// 	int		last_in_fd;
// 	int		last_out_fd;
// 	t_redir	*r;
// 	int		result;

// 	node = cmd->redirs;
// 	last_in_fd = -1;
// 	last_out_fd = -1;
// 	while (node)
// 	{
// 		r = node->content;
// 		if (r->type == R_INPUT || r->type == R_HEREDOC)
// 		{
// 			safe_close(&last_in_fd);
// 			if (r->type == R_HEREDOC)
// 				result = handle_heredoc_redirection(r);
// 			else
// 				result = handle_file_input_redirection(r);
// 			if (result == REDIR_ERR)
// 				return (EXIT_FAILURE);
// 			last_in_fd = r->fd;
// 		}
// 		else if (r->type == R_OUTPUT || r->type == R_APPEND)
// 		{
// 			safe_close(&last_out_fd);
// 			if (handle_output_redirection(r) == REDIR_ERR)
// 				return (EXIT_FAILURE);
// 			last_out_fd = -1;
// 		}
// 		node = node->next;
// 	}
// 	return (EXIT_SUCCESS);
// }
