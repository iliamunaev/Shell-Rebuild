/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 13:09:26 by pvershin          #+#    #+#             */
/*   Updated: 2025/05/08 13:58:35 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Initialize a token array
t_TokenArray	*token_array_init(void)
{
	t_TokenArray	*array;

	array = (t_TokenArray *)malloc(sizeof(t_TokenArray)); // tested
	if (!array)
	{
		print_error("-minishell: failed to allocate token array\n");
		return(NULL);
	}
	array->capacity = 16;
	array->tokens = (t_Token *)malloc(sizeof(t_Token) * array->capacity); // tested
	if (!array->tokens)
	{
		print_error("-minishell: failed to allocate tokens buffer\n");
		free(array);
		return(NULL);
	}
	array->count = 0;
	return (array);
}

// Add a token to the array
void	token_array_add(t_TokenArray *array, t_Token token)
{
	if (array->count >= array->capacity)
	{
		array->capacity *= 2;
		array->tokens = (t_Token *)realloc(array->tokens, sizeof(t_Token)
				* array->capacity);
		if (!array->tokens)
		{
			print_error("Failed to reallocate tokens buffer\n");
			exit(1);
		}
	}
	array->tokens[array->count++] = token;
}

// Free token array resources
void	token_array_free(t_TokenArray *array)
{
	int	i;

	if (!array)
	{
		return ;
	}
	i = 0;
	while (i < array->count)
	{
		free_token(&array->tokens[i]);
		i++;
	}
	free(array->tokens);
	free(array);
}

static void	skip_joined_words(t_TokenArray *tokens, int *i)
{
	while (*i < tokens->count
		&& tokens->tokens[*i].type == TOKEN_WORD
		&& tokens->tokens[*i].needs_join == 0)
	{
		(*i)++;
	}
}

int	count_new_tokens(t_TokenArray *tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < tokens->count)
	{
		if (tokens->tokens[i].type == TOKEN_EMPTY)
		{
			i++;
			continue ;
		}
		count++;
		if (tokens->tokens[i].type != TOKEN_WORD)
		{
			i++;
			continue ;
		}
		i++;
		skip_joined_words(tokens, &i);
	}
	return (count);
}
