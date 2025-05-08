/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:20:40 by pvershin          #+#    #+#             */
/*   Updated: 2025/05/08 14:26:03 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file tokenizer.h
 * @brief Tokenizer interface: creation, destruction, token retrieval, and
 * utilities.
 */
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
	TOKEN_HEREDOC,
	TOKEN_EMPTY,
	TOKEN_EOF,
	TOKEN_VALUE_FAIL
}				t_TokenType;

// quote_style; // 0 = none, 1 = single, 2 = double
typedef struct s_token
{
	char		*value;
	t_TokenType	type;
	int			in_single_quotes;
	int			in_double_quotes;
	bool		needs_join;
	int			quote_style;
}				t_Token;

// Token array for storing all tokens
typedef struct s_tokens_array
{
	t_Token		*tokens;
	int			count;
	int			capacity;
}				t_TokenArray;

typedef struct s_tokenizer
{
	const char	*input;
	const char	*input_base;
	char		*buffer;
	size_t		buffer_size;
}				t_Tokenizer;

typedef struct s_unsupported_cmd
{
	const char	*pattern;
	int			error_code;
	const char	*error_message;
}				t_unsupported_cmd;

// Get the next token from the input
// Get the next token from the input
t_Token			get_next_token(t_Tokenizer *tokenizer);

/**
 * @brief Parses a dollar-quoted string token ($\"...\").
 *
 * @param tokenizer Pointer to the tokenizer.
 * @param saw_space Indicates if token should be joined to previous.
 * @return t_Token Parsed token.
 */
t_Token			tokenizer_parse_special_dollar_quote(t_Tokenizer *tokenizer,
					int saw_space);

/**
 * @brief Parses a quoted string token (single or double).
 *
 * @param tokenizer Pointer to the tokenizer.
 * @param saw_space Indicates if token should be joined to previous.
 * @return t_Token Parsed token.
 */
t_Token			tokenizer_parse_quoted(t_Tokenizer *tokenizer, int saw_space);

/**
 * @brief Parses redirection tokens "<<" or ">>".
 *
 * @param tokenizer Pointer to the tokenizer.
 * @return t_Token Parsed token.
 */
t_Token			tokenizer_parse_redirection(t_Tokenizer *tokenizer);

/**
 * @brief Parses single-character operator tokens |, <, >, &.
 *
 * @param tokenizer Pointer to the tokenizer.
 * @return t_Token Parsed token.
 */
t_Token			tokenizer_parse_operator(t_Tokenizer *tokenizer);

/**
 * @brief Parses an unquoted word token until special or whitespace.
 *
 * @param tokenizer Pointer to the tokenizer.
 * @param saw_space Indicates if token should be joined to previous.
 * @return t_Token Parsed token.
 */
t_Token			tokenizer_parse_word(t_Tokenizer *tokenizer, int saw_space);

// Free resources used by a token
void			free_token(t_Token *token);

char			*expand_env_variables(const char *input, t_mshell *minishell,
					int quote_style);
void			print_token(t_Token token);
void			explain_token(t_Token token);
void			debug_print_parsed_commands(t_cmd *cmd);
int				strip_words(t_TokenArray *tokens);
bool			is_in_single_quotes(char *str);
bool			is_in_double_quotes(char *str);
void			expand_tokens(t_TokenArray *tokens, t_mshell *ms);

t_Tokenizer		*tokenizer_create(const char *input);
void			tokenizer_destroy(t_Tokenizer *tokenizer);

int				is_input_redir(t_TokenType type);
int				is_output_redir(t_TokenType type);
t_cmd			*finalize_cmd_list(t_list **cmd_list);
void			free_cmd_list(t_list **cmd_list);
void			handle_word_token(t_mshell *shell, t_list **cmd_list,
					t_cmd **current, char *value);
int				handle_input_redir(t_mshell *shell, t_list **cmd_list,
					t_cmd **current, t_Token *token);
int				handle_output_redir(t_mshell *shell, t_list **cmd_list,
					t_cmd **current, t_Token *token);
void			free_list_nodes_only(t_list **lst);

// Expansion context for helper functions
/*	const char	*input;      // input string being processed
	size_t		*i;          // current position index in input
	char		**result;    // pointer to result buffer
	t_mshell	*mshell;     // shell context (env, status)
	int			quote_style; // 0 = none, 1 = single, 2 = double
	int			single_q;    // flag: in single-quote mode
	int			double_q;    // flag: in double-quote mode
 */
typedef struct s_exp_ctx
{
	const char	*input;
	size_t		*i;
	char		**result;
	t_mshell	*mshell;
	int			quote_style;
	int			single_q;
	int			double_q;
}				t_exp_ctx;

// Append a single character to the expansion result
void			append_char(t_exp_ctx *ctx, char c);
// Handle single-quote characters in expansion
void			handle_single_quote(t_exp_ctx *ctx);
// Handle double-quote characters in expansion
void			handle_double_quote(t_exp_ctx *ctx);
// Handle backslash escapes in expansion
void			handle_backslash(t_exp_ctx *ctx);
// Handle tilde expansion or literal '~'
void			handle_tilde(t_exp_ctx *ctx);
// Handle dollar-sign variable expansions
void			handle_dollar(t_exp_ctx *ctx);

char			*get_env_value(const char *var, t_mshell *minishell);
char			*append_to_result(char *result, const char *append);
char			*handle_escape(const char *input, size_t *i, int single_q);
char			*expand_tilde(const char *input, size_t *i, t_mshell *mshell,
					int quote_style);
char			*get_exit_code(t_mshell *minishell);
bool			is_valid_redir_target(t_TokenArray *tokens, int i);
t_cmd			*create_empty_command(t_mshell *shell);
int				known_unsupported_cmd(const char *cmd, const char **message);
bool			ft_is_special_char(char c);
void			classify_token_if_needed(t_Token *token, char *expanded);
#endif
