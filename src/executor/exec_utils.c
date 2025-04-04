/**
 * @file exec_utils.c
 * @brief Utility functions for managing variables, pipeline limits, and exit statuses in Minishell.
 */
#include "minishell.h"

// void	cleanup_heredoc(t_cmd *cmd)
// {
// 	if (cmd->in_redir && cmd->in_redir->type == R_HEREDOC)
// 	{
// 		if (unlink(HEREDOC_TFILE) == -1)
// 			fatal_error("unlink", EXIT_FAILURE);
// 	}
// }

/**
 * @brief Splits a `KEY=VALUE` string into a `t_mshell_var` structure.
 *
 * - If an equal sign (`=`) is found, the string is split into a key and a value.
 * - If no equal sign is present, the value is set to `NULL`.
 *
 * @param key_value_pair The string to split.
 * @return A pointer to a newly allocated `t_mshell_var` structure, or NULL on failure.
 */
t_mshell_var	*split_key_value(char *key_value_pair)
{
	t_mshell_var	*mshell_var;
	char			*equal_sign;

	mshell_var = malloc(sizeof(t_mshell_var));
	if (!mshell_var)
	{
		print_error("Error (split_key_value): mshell_var malloc failed\n");
		return (NULL);
	}
	equal_sign = ft_strchr(key_value_pair, '=');
	if (equal_sign)
	{
		mshell_var->key = ft_substr(key_value_pair, 0, equal_sign - key_value_pair);
		mshell_var->value = ft_strdup(equal_sign + 1);
	}
	else
	{
		mshell_var->key = ft_strdup(key_value_pair);
		mshell_var->value = NULL;
	}
	return (mshell_var);
}

/**
 * @brief Checks if the number of piped commands exceeds the allowed limit.
 *
 * This function increments the command count and checks if it exceeds
 * `MAX_CMDS`. If the limit is reached, an error message is printed.
 *
 * @param cmd_count Pointer to the command counter.
 * @return `true` if the pipeline limit is exceeded, `false` otherwise.
 */
bool	is_pipeline_limit(int *cmd_count)
{
	if (++(*cmd_count) > MAX_CMDS)
	{
		print_error("Error: pipeline limit exceeded\n");
		return (true);
	}
	return (false);
}
