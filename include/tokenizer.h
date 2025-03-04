/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pvershin <pvershin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:17:27 by pvershin          #+#    #+#             */
/*   Updated: 2025/03/06 11:38:56 by pvershin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include <minishell.h>

typedef enum e_possible_tokens
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND_OUT,
	TOKEN_BACKGROUND,
	TOKEN_EOF
}	t_TokenType;

typedef struct s_tkn
{
	t_TokenType	type;
	char		*value;
}	t_Token;

// Token array for storing all tokens
typedef struct s_tokens_array
{
	t_Token	*tokens;
	int		count;
	int		capacity;
}	t_TokenArray;

// Initialize the tokenizer with input string
void	tokenizer_init(const char *input);

// Get the next token from the input
t_Token	get_next_token(void);

// Free resources used by a token
void	free_token(t_Token *token);

// Free tokenizer resources
void	tokenizer_cleanup(void);

#endif
