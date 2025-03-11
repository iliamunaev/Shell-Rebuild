/**
 * @file handle_export.c
 * @brief Functions for handling the `export` built-in command in Minishell.
 */
#include "minishell.h"

/**
 * @brief Prints all exported variables in the shell.
 *
 * Iterates through the hash table and prints assigned variables in the format:
 * - `declare -x KEY="VALUE"` if the variable has a value.
 * - `declare -x KEY` if the variable has no value.
 *
 * @param mshell Pointer to the Minishell structure.
 */
void	print_export_from_ht(t_mshell *mshell)
{
	t_mshell_var	*var;
	int				i;

	i = 0;
	while (i < HASH_SIZE)
	{
		var = mshell->hash_table->buckets[i];
		while (var)
		{
			if (var->key)
			{
				if (var->value)
					printf("declare -x %s=\"%s\"\n", var->key, var->value);
				else
					printf("declare -x %s\n", var->key);
			}
			var = var->next;
		}
		i++;
	}
}

/**
 * @brief Processes a single argument for the `export` command.
 *
 * Checks if the argument is a valid variable name and, if so, adds or updates
 * it in the shell’s environment. If an `=` is present, the variable is assigned a value.
 *
 * @param cmd Pointer to the command structure.
 * @param arg The argument containing a variable name (and optional value).
 * @return `EXIT_SUCCESS` if successful, `EXIT_FAILURE` if the argument is invalid.
 */
static uint8_t	process_export_arg(t_cmd *cmd, char *arg)
{
	char			*eq;
	int				assigned;
	t_mshell_var	*pair;

	if (!is_valid_varname(arg))
	{
		print_error("Error: not a valid identifier\n");
		return (EXIT_FAILURE);
	}
	pair = split_key_value(arg);
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
	uint8_t	ret;

	exit_status = (EXIT_SUCCESS);
	if (!cmd->argv[1])
	{
		print_export_from_ht(cmd->minishell);
		cmd->minishell->exit_status = EXIT_SUCCESS;
		if (cmd->in_pipe)
			exit(EXIT_SUCCESS);
		return (EXIT_SUCCESS);
	}
	exit_status = (EXIT_SUCCESS);
	i = 1;
	while (cmd->argv[i])
	{
		ret = process_export_arg(cmd, cmd->argv[i]);
		if (ret != EXIT_SUCCESS)
			exit_status = ret;
		i++;
	}
	update_env(cmd->minishell);

	printf("Exit status: %d\n",cmd->minishell->exit_status); //  test
	if (cmd->in_pipe)
		exit(exit_status);
	return (exit_status);
}
