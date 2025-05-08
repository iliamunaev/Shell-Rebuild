/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:10:38 by pvershin          #+#    #+#             */
/*   Updated: 2025/05/08 14:46:33 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	fill_new_tokens(t_TokenArray *new_tokens, t_TokenArray *old_tokens)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < old_tokens->count)
	{
		if (is_skippable_token(&old_tokens->tokens[i]))
		{
			i++;
			continue ;
		}
		if (old_tokens->tokens[i].type != TOKEN_WORD)
		{
			process_non_word(new_tokens, &j, &old_tokens->tokens[i]);
			i++;
		}
		else
			process_word(new_tokens, old_tokens, &i, &j);
	}
	new_tokens->count = j;
}

int	group_word_tokens(t_TokenArray *tokens)
{
	t_TokenArray	new_tokens;
	int				new_count;

	if (!tokens || tokens->count <= 1)
		return (0);
	new_count = count_new_tokens(tokens);
	new_tokens.tokens = malloc(sizeof(t_Token) * new_count);
	if (!new_tokens.tokens)
		return (-1);
	new_tokens.capacity = new_count;
	fill_new_tokens(&new_tokens, tokens);
	free(tokens->tokens);
	tokens->tokens = new_tokens.tokens;
	tokens->count = new_tokens.count;
	tokens->capacity = new_tokens.capacity;
	return (0);
}

/**
 * @brief Handles a pipe token by resetting the current command.
 *
 * This function is called when a pipe (`|`) token is encountered,
 * indicating the start of a new command in a pipeline.
 *
 * @param current Pointer to the current command pointer.
 * @param i Pointer to the current token index, which is incremented.
 */
void	process_pipe_token(t_cmd **current, int *i)
{
	*current = NULL;
	(*i)++;
}

int	init_redir_command_if_needed(t_redir_ctx *ctx)
{
	t_list	*new_node;

	*ctx->current = create_empty_command(ctx->shell);
	if (!*ctx->current)
	{
		return (-1);
	}
	new_node = ft_lstnew(*ctx->current);
	if (!new_node)
	{
		free_cmd(ctx->current);
		free_cmd_list(ctx->cmd_list);
		return (-1);
	}
	ft_lstadd_back(ctx->cmd_list, new_node);
	return (0);
}

int	apply_redirection(t_redir_ctx *ctx, t_TokenType type)
{
	t_Token	*token;

	token = &ctx->tokens->tokens[*ctx->i];
	if (is_input_redir(type))
	{
		if (handle_input_redir(ctx->shell, ctx->cmd_list,
				ctx->current, token) < 0)
			return (-1);
	}
	else
	{
		if (handle_output_redir(ctx->shell, ctx->cmd_list,
				ctx->current, token) < 0)
			return (-1);
	}
	return (0);
}
