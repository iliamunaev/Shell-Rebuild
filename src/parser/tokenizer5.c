/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:19:19 by pvershin          #+#    #+#             */
/*   Updated: 2025/05/08 14:44:57 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file tokenizer5.c
 * @brief Parsing redirection and operator tokens.
 */

#include "../include/minishell.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Parses redirection tokens "<<" or ">>".
 *
 * @param tokenizer Pointer to the tokenizer.
 * @return t_Token Parsed redirection token.
 */
t_Token	tokenizer_parse_redirection(t_Tokenizer *tokenizer)
{
	t_Token	token;

	ft_bzero(&token, sizeof(t_Token));
	if (*tokenizer->input == '<' && *(tokenizer->input + 1) == '<')
	{
		tokenizer->input += 2;
		token.type = TOKEN_HEREDOC;
		token.value = ft_strdup("<<");
	}
	else if (*tokenizer->input == '>' && *(tokenizer->input + 1) == '>')
	{
		tokenizer->input += 2;
		token.type = TOKEN_APPEND_OUT;
		token.value = ft_strdup(">>");
	}
	return (token);
}

/**
 * @brief Parses single-character operator tokens |, <, >, &.
 *
 * @param tokenizer Pointer to the tokenizer.
 * @return t_Token Parsed operator token.
 */
t_Token	tokenizer_parse_operator(t_Tokenizer *tokenizer)
{
	t_Token	token;

	ft_bzero(&token, sizeof(t_Token));
	if (*tokenizer->input == '|')
		token.type = TOKEN_PIPE;
	else if (*tokenizer->input == '<')
		token.type = TOKEN_REDIRECT_IN;
	else if (*tokenizer->input == '>')
		token.type = TOKEN_REDIRECT_OUT;
	else if (*tokenizer->input == '&')
		token.type = TOKEN_BACKGROUND;
	token.value = malloc(2);
	if (!token.value)
	{
		print_error("-minishell: token.value malloc failed\n");
		tokenizer_destroy(tokenizer);
		token.type = TOKEN_VALUE_FAIL;
		return (token);
	}
	token.value[0] = *tokenizer->input++;
	token.value[1] = '\0';
	return (token);
}
