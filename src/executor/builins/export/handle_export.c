/**
 * @file handle_export.c
 * @brief Functions for handling the `export` built-in command in Minishell.
 */
#include "minishell.h"

/**
 * @brief Validates if a given string is a valid variable name for minishell.
 *
 * A valid variable name:
 * - Starts with an alphabetic character or underscore ('_').
 * - Contains only alphanumeric characters or underscores ('_').
 * - Stops validation at the first '=' character or at the end of the string.
 *
 * @param key_value_pair The string to validate (e.g., "MYVAR=value").
 * @return `true` if valid, `false` otherwise.
 */
bool	is_valid_varname(const char *key)
{
	int	i;

	if (!key || !(ft_isalpha(key[0]) || key[0] == '_'))
		return (false);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!(ft_isalnum(key[i]) || key[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Processes a single argument for the `export` command.
 *
 * Checks if the argument is a valid variable name and, if so, adds or updates
 * it in the shell’s environment. If an `=` is present, the variable is
 * assigned a value.
 *
 * @param cmd Pointer to the command structure.
 * @param arg The argument containing a variable name (and optional value).
 * @return `EXIT_SUCCESS` if successful, `EXIT_FAILURE` if the argument is
 *          invalid.
 */
static uint8_t	process_export_arg(t_cmd *cmd, char *arg)
{
	char			*eq;
	int				assigned;
	t_mshell_var	*pair;

	pair = split_key_value(arg);
	if (!is_valid_varname(pair->key))
	{
		print_error("-minishell: export: '");
		print_error(pair->key);
		print_error("': not a valid identifier\n");
		return (EXIT_FAILURE);
	}
	eq = ft_strchr(arg, '=');
	assigned = 0;
	if (eq)
		assigned = 1;
	set_variable(cmd->minishell, pair->key, pair->value, assigned);
	free(pair->key);
	free(pair->value);
	free(pair);
	return (EXIT_SUCCESS);
}

/**
 * @brief Handles the `export` built-in command.
 *
 * - If called without arguments, prints the exported variables.
 * - If called with arguments, it processes and updates the environment.
 * - If executed in a pipeline, the process exits with the appropriate status.
 *
 * @param cmd Pointer to the command structure.
 * @return `EXIT_SUCCESS` if successful, or an appropriate error code.
 */
uint8_t	handle_export(t_cmd *cmd)
{
	int		i;
	uint8_t	exit_status;

	if (!cmd->argv[1])
	{
		handle_sorted_env(cmd->minishell);
		exit_status = EXIT_SUCCESS;
		return (exit_status);
	}
	i = 1;
	while (cmd->argv[i])
	{
		exit_status = process_export_arg(cmd, cmd->argv[i]);
		i++;
	}
	update_env(cmd->minishell);
	return (exit_status);
}
