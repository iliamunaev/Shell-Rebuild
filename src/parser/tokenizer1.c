/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:12:47 by pvershin          #+#    #+#             */
/*   Updated: 2025/05/08 14:45:49 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file tokenizer1.c
 * @brief Tokenizer creation and destruction functions.
 */

#include "../include/minishell.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Creates a new tokenizer for the given input string.
 *
 * Allocates and initializes a t_Tokenizer structure.
 *
 * @param input The input string to tokenize.
 * @return Pointer to the newly created tokenizer,
	or NULL on allocation failure.
 */
t_Tokenizer	*tokenizer_create(const char *input)
{
	t_Tokenizer	*tokenizer;
	size_t		len;

	tokenizer = malloc(sizeof(t_Tokenizer));
	if (!tokenizer)
	{
		print_error("-minishell: tokenizer malloc failed\n");
		return (NULL);
	}
	len = ft_strlen(input);
	tokenizer->input = input;
	tokenizer->input_base = input;
	tokenizer->buffer_size = len + 1;
	tokenizer->buffer = malloc(tokenizer->buffer_size);
	if (!tokenizer->buffer)
	{
		print_error("-minishell: tokenizer buffer malloc failed\n");
		free(tokenizer);
		return (NULL);
	}
	return (tokenizer);
}

/**
 * @brief Frees resources associated with a tokenizer.
 *
 * @param tokenizer Pointer to the tokenizer to destroy.
 */
void	tokenizer_destroy(t_Tokenizer *tokenizer)
{
	if (!tokenizer)
		return ;
	free(tokenizer->buffer);
	free(tokenizer);
}

/**
 * @brief Frees resources associated with a token.
 *
 * @param token Pointer to the token to free.
 */
void	free_token(t_Token *token)
{
	if (token && token->value)
	{
		free(token->value);
		token->value = NULL;
	}
}
