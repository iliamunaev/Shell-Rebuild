/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Pavel Vershinin <pvershin@student.hive.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:19:19 by pvershin          #+#    #+#             */
/*   Updated: 2025/05/15 19:53:22 by Pavel Versh      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file tokenizer5.c
 * @brief Parsing redirection and operator tokens.
 */

#include "../include/minishell.h"

/**
 * @brief Parses redirection tokens "<<" or ">>".
 *
 * @param tokenizer Pointer to the tokenizer.
 * @return t_Token Parsed redirection token.
 */
t_Token	tokenizer_parse_redirection(t_Tokenizer *tokenizer, t_mshell *minishell)
{
	t_Token	token;

	ft_bzero(&token, sizeof(t_Token));
	if (*tokenizer->input == '<' && *(tokenizer->input + 1) == '<')
	{
		tokenizer->input += 2;
		token.type = TOKEN_HEREDOC;
		token.value = ft_strdup("<<");
		if (!token.value)
			minishell->allocation_error = true;
	}
	else if (*tokenizer->input == '>' && *(tokenizer->input + 1) == '>')
	{
		tokenizer->input += 2;
		token.type = TOKEN_APPEND_OUT;
		token.value = ft_strdup(">>");
		if (!token.value)
			minishell->allocation_error = true;
	}
	return (token);
}

/**
 * @brief Parses single-character operator tokens |, <, >, &.
 *
 * @param tokenizer Pointer to the tokenizer.
 * @return t_Token Parsed operator token.
 */
t_Token	tokenizer_parse_operator(t_Tokenizer *tokenizer, t_mshell *mshell)
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
		mshell->allocation_error = true;
		return (token);
	}
	token.value[0] = *tokenizer->input++;
	token.value[1] = '\0';
	return (token);
}
