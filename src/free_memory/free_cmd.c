/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:49:35 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/05/08 13:36:46 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file free_cmd.c
 * @brief Functions for freeing memory allocated for command structures.
 */
#include "minishell.h"

/**
 * @brief Frees a list of redirection structures (`t_list` of `t_redir`).
 *
 * Iterates through a linked list of redirection nodes, freeing each:
 * - Closes file descriptors for heredocs
 * - Frees filenames and `t_redir` structures
 * - Frees the list node itself
 *
 * @param redirs Pointer to the head of the redirection list.
 */
static void	free_redirs(t_list *redirs)
{
	t_list	*tmp;
	t_redir	*redir;

	while (redirs)
	{
		tmp = redirs->next;
		redir = redirs->content;
		if (redir && redir->type == R_HEREDOC && redir->fd >= 0)
			close(redir->fd);
		if (redir)
			free(redir->filename);
		free(redir);
		free(redirs);
		redirs = tmp;
	}
}

/**
 * @brief Frees the contents of a single `t_cmd` node.
 *
 * Frees:
 * - The `argv` array and its strings
 * - The binary path string
 * - The list of redirections
 *
 * Does not free the `t_cmd` struct itself. Intended for use by `free_cmd()`.
 *
 * @param cmd The command node whose contents are to be freed.
 */
static void	free_single_cmd(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	if (cmd->argv)
	{
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv);
		cmd->argv = NULL;
	}
	if (cmd->binary)
	{
		free(cmd->binary);
		cmd->binary = NULL;
	}
	if (cmd->redirs)
	{
		free_redirs(cmd->redirs);
		cmd->redirs = NULL;
	}
}

/**
 * @brief Frees a linked list of `t_cmd` nodes and their contents.
 *
 * Iterates through the list, freeing each node’s internal resources via
 * `free_single_cmd()`, then frees the node itself.
 * Sets the original pointer to NULL on completion.
 *
 * @param cmd_ptr Address of the pointer to the head of the list.
 */
void	free_cmd(t_cmd **cmd_ptr)
{
	t_cmd	*cmd;
	t_cmd	*next;

	if (!cmd_ptr || !*cmd_ptr)
		return ;
	cmd = *cmd_ptr;
	while (cmd)
	{
		next = cmd->next;
		free_single_cmd(cmd);
		free(cmd);
		cmd = next;
	}
	*cmd_ptr = NULL;
}

/**
 * @brief Frees and nullifies a pointer.
 *
 * Checks if the pointer and its contents are non-null,
 * then frees the memory and sets the pointer to NULL.
 *
 * @param ptr A double pointer to the memory to free.
 */
void	free_ptr(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

void	free_cmd_node(t_cmd *cmd)
{
	if (!cmd)
		return ;
	free_single_cmd(cmd);
	free(cmd);
}
