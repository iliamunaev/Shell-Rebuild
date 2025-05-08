/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:05:00 by pvershin          #+#    #+#             */
/*   Updated: 2025/05/08 14:46:16 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokenizer.h"

/**
 * @brief Ensure current command exists in list, add if missing.
 */
static int	ensure_current_cmd(t_mshell *shell, t_list **cmd_list,
	t_cmd	**current)
{
	if (!*current)
	{
		*current = create_empty_command(shell);
		if (!*current)
			return (-1);
		ft_lstadd_back(cmd_list, ft_lstnew(*current));
	}
	return (0);
}

/**
 * @brief Create and append an input redirection node.
 */
static int	apply_input_redir(t_cmd **current, t_Token *token)
{
	t_redir	*redir;

	if (!token[1].value || token[1].type != TOKEN_WORD)
	{
		print_error("Syntax error: missing name for redirection\n");
		return (-1);
	}
	redir = malloc(sizeof(t_redir));
	if (!redir)
	{
		print_error("-minishell: parser, apply_input_redir, maaloc failed\n");
		return (-1);
	}
	redir->fd = -1;
	if (token->type == TOKEN_REDIRECT_IN)
		redir->type = R_INPUT;
	else
		redir->type = R_HEREDOC;
	redir->filename = ft_strdup(token[1].value);
	if (!redir->filename)
	{
		free(redir);
		return (-1);
	}
	redir->expand_in_heredoc = (token[1].quote_style == 0);
	ft_lstadd_back(&(*current)->redirs, ft_lstnew(redir));
	return (0);
}

/**
 * @brief Handle input redirection tokens.
 */
int	handle_input_redir(t_mshell *shell, t_list **cmd_list,
	t_cmd **current, t_Token *token)
{
	if (ensure_current_cmd(shell, cmd_list, current) < 0)
		return (-1);
	return (apply_input_redir(current, token));
}

/**
 * @brief Create and append an output redirection node.
 */
static int	apply_output_redir(t_cmd **current, t_Token *token)
{
	t_redir	*redir;

	if (!token[1].value || token[1].type != TOKEN_WORD)
	{
		print_error("Syntax error: missing name for redirection\n");
		return (-1);
	}
	redir = malloc(sizeof(t_redir));
	if (!redir)
	{
		print_error("-minishell: parser, apply_output_redir, maaloc failed\n");
		return (-1);
	}
	if (token->type == TOKEN_REDIRECT_OUT)
		redir->type = R_OUTPUT;
	else
		redir->type = R_APPEND;
	redir->filename = ft_strdup(token[1].value);
	if (!redir->filename)
	{
		free(redir);
		return (-1);
	}
	ft_lstadd_back(&(*current)->redirs, ft_lstnew(redir));
	return (0);
}

/**
 * @brief Handle output redirection tokens.
 */
int	handle_output_redir(t_mshell *shell, t_list **cmd_list,
	t_cmd **current, t_Token *token)
{
	if (ensure_current_cmd(shell, cmd_list, current) < 0)
		return (-1);
	return (apply_output_redir(current, token));
}
