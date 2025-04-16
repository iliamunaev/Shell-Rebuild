#include "../include/minishell.h"

/**
 * @brief Tokenizes the input string into a token array.
 *
 * @param input User input string.
 * @return Pointer to a new t_TokenArray, or NULL on error.
 */
static t_TokenArray	*tokenize_input(char *input)
{
	t_Tokenizer		*tokenizer;
	t_TokenArray	*tokens;
	t_Token			token;

	tokenizer = tokenizer_create(input);
	if (!tokenizer)
		return (NULL);
	tokens = token_array_init();
	if (!tokens)
		return (NULL);
	token = get_next_token(tokenizer);
	while (token.type != TOKEN_EOF)
	{
		token_array_add(tokens, token);
		token = get_next_token(tokenizer);
	}
	tokenizer_destroy(tokenizer);
	return (tokens);
}


/**
 * @brief Checks if the input string matches any unsupported syntax pattern.
 *
 * @param shell The minishell instance.
 * @param input The original input string.
 * @return 1 if unsupported, 0 otherwise.
 */
static int	check_for_unsupported_syntax(t_mshell *shell, char *input)
{
	const char	*err_msg;
	int			code;

	if (!input || ft_strlen(input) == 0)
		return (0);
	code = known_unsupported_cmd(input, &err_msg);
	shell->syntax_exit_status = code;
	if (code)
	{
		if (err_msg)
			print_error((char *)err_msg);
		else
			print_error("minishell: unsupported syntax\n");
	}
	return (code);
}

/**
 * @brief Prints token debug information to stdout.
 *
 * @param tokens Token array to print.
 */
static void	debug_print_tokens(t_TokenArray *tokens)
{
	int	i;

	debug_printf("Found %d token(s):\n", tokens->count);
	i = 0;
	while (i < tokens->count)
	{
		debug_printf("\nToken %d:\n", i);
		print_token(tokens->tokens[i]);
		explain_token(tokens->tokens[i]);
		i++;
	}
}

/**
 * @brief Main parser entry point: tokenizes input and builds command list.
 *
 * @param minishell Minishell context.
 * @param input Raw user input.
 * @return Parsed command list, or NULL on error.
 */
t_cmd	*run_parser(t_mshell *minishell, char *input)
{
	t_TokenArray	*tokens;
	t_cmd			*cmd;

	debug_printf("\nTokenizing: %s\n\n", input);
	tokens = tokenize_input(input);
	if (!tokens)
		return (NULL);
	expand_tokens(tokens, minishell);
	check_for_unsupported_syntax(minishell, input);
	group_word_tokens(tokens);
	strip_words(tokens);
	cmd = create_command_from_tokens(minishell, tokens);
	debug_print_tokens(tokens);
	debug_print_parsed_commands(cmd);
	token_array_free(tokens);
	return (cmd);
}
