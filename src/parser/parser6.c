/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvershin <pvershin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:19:34 by Pavel Versh       #+#    #+#             */
/*   Updated: 2025/05/16 10:26:40 by pvershin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**
 * @brief Finalizes the command list by normalizing argv and resolving binaries.
 *
 * For each command:
 * - Fills remaining argv slots with NULL.
 * - Resolves and assigns the binary path.
 *
 * @param head The head of the command linked list.
 */
t_cmd	*finalize_commands(t_cmd *head)
{
	int		j;
	t_cmd	*cmd_ptr;
	t_cmd	*ohead;

	cmd_ptr = head;
	ohead = head;
	while (cmd_ptr)
	{
		if (cmd_ptr->argv)
		{
			j = 0;
			while (j < MAX_ARGS && cmd_ptr->argv[j])
				j++;
			while (j < MAX_ARGS)
				cmd_ptr->argv[j++] = NULL;
		}
		cmd_ptr->binary = find_binary(cmd_ptr);
		cmd_ptr->origin_head = head;
		cmd_ptr = cmd_ptr->next;
	}
	return (ohead);
}

/**
 * @brief Append an argument to an existing command.
 */
static int	append_word_argument(t_cmd *current, char *value)
{
	int	i;

	i = 0;
	while (current->argv[i])
		++i;
	if (i < MAX_ARGS)
	{
		current->argv[i + 1] = NULL;
		current->argv[i] = ft_strdup(value);
		if (!current->argv[i])
			return (1);
	}
	return (0);
}

/**
 * @brief Processes a word token and adds it to the current command's argv.
 *
 * If there is no current command, creates a new one and adds it to the list.
 * Appends the word value to the command's argv.
 *
 * @param shell Minishell context.
 * @param cmd_list Pointer to the list of parsed commands.
 * @param current Pointer to the current command being filled.
 * @param token The current word token to be processed.
 * @return 0 on success, -1 on error (e.g., memory allocation failure).
 */
static int	process_word_token(t_mshell *sh, t_list **cmd_list, t_cmd **current,
		t_Token *token)
{
	t_list	*ptr;

	if (!*current)
	{
		*current = create_empty_command(sh);
		if (!*current)
			return (-1);
		ptr = ft_lstnew(*current);
		if (!ptr)
		{
			sh->allocation_error = true;
			free_cmd(current);
			return (-1);
		}
		ft_lstadd_back(cmd_list, ptr);
	}
	return (sh->allocation_error = append_word_argument(*current,
			token->value));
}

/**
 * @brief Iterates through tokens and builds command list.
 *
 * @param ctx Full parser context with references to shell, tokens,
 * and current state.
 * @return 0 on success, -1 on failure.
 */
int	parse_tokens(t_parse_ctx *ctx)
{
	int	retval;

	while (ctx->tokens && ctx->i < ctx->tokens->count)
	{
		if (ctx->tokens->tokens[ctx->i].type == TOKEN_PIPE)
			process_pipe_token(ctx->current, &ctx->i);
		else if (is_input_redir(ctx->tokens->tokens[ctx->i].type)
			|| is_output_redir(ctx->tokens->tokens[ctx->i].type))
		{
			retval = handle_redir(ctx);
			if (retval != 0)
				return (retval);
		}
		else if (ctx->tokens->tokens[ctx->i].type == TOKEN_WORD)
		{
			if (process_word_token(ctx->shell, ctx->cmd_list, ctx->current,
					&ctx->tokens->tokens[ctx->i]) < 0)
				return (-1);
			ctx->i++;
		}
		else
			ctx->i++;
	}
	return (0);
}

void	init_parse_context(t_parse_ctx *ctx)
{
	ctx->i = 0;
	ctx->redir.shell = ctx->shell;
	ctx->redir.tokens = ctx->tokens;
	ctx->redir.cmd_list = ctx->cmd_list;
	ctx->redir.current = ctx->current;
	ctx->redir.i = &ctx->i;
}
