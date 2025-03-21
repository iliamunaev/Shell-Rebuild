/**
 * @file handle_unset.c
 * @brief Functions for handling the `unset` built-in command in Minishell.
 */
#include "minishell.h"

/**
 * @brief Removes a variable from the Minishell environment's hash table.
 *
 * This function searches for a variable with the given `key` in the hash
 * table.
 * If found, it removes the variable by unlinking it from the corresponding
 * bucket list and frees its allocated memory. After deletion,
 * `update_env(mshell)` is called to synchronize the shell's environment
 * array.
 *
 * @param mshell Pointer to the Minishell structure containing the hash table.
 * @param key The name of the variable to be removed.
 */
static void	remove_var_from_ht(t_mshell *mshell, char *key)
{
	unsigned long	index;
	t_mshell_var	*prev;
	t_mshell_var	*current;

	prev = NULL;
	if (!mshell || !key)
		return ;
	index = hash_function(key);
	current = mshell->hash_table->buckets[index];
	while (current)
	{
		if (current->key && ft_strcmp(current->key, key) == 0)
		{
			if (prev == NULL)
				mshell->hash_table->buckets[index] = current->next;
			else
				prev->next = current->next;
			free_mshell_var(current);
			update_env(mshell);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/**
 * @brief Iterates through all arguments of `unset` and removes each variable.
 *
 * This function loops over `cmd->argv`, starting from the first argument
 * (`argv[1]`),
 * and removes each specified variable from the Minishell environment using
 * `remove_var_from_ht()`.
 *
 * @param cmd Pointer to the command structure containing command arguments.
 * @return Always returns `EXIT_SUCCESS`.
 */
static uint8_t	do_unset_loop(t_cmd *cmd)
{
	int		i;

	i = 1;
	while (cmd->argv[i])
	{
		remove_var_from_ht(cmd->minishell, cmd->argv[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Handles the `unset` built-in command in Minishell.
 *
 * The `unset` command removes one or more environment variables from the
 * shell.
 * - If no arguments are provided, the function returns `EXIT_SUCCESS`.
 * - If the command structure (`cmd`) or Minishell instance is missing, it
 * prints an error.
 * - Otherwise, it loops through the arguments and removes each specified
 * variable.
 *
 * If `unset` is executed in a subshell (e.g., inside a pipeline), the
 * process may exit
 * after execution, depending on the shell's implementation.
 *
 * **Usage Example:**
 * ```
 * unset VAR1 VAR2 VAR3
 * ```
 *
 * @param cmd Pointer to the command structure (contains argv, minishell
 *            instance, etc.).
 * @return `EXIT_SUCCESS` if all requested variables were removed or no
 *         arguments were provided.
 *         `EXIT_FAILURE` if an error occurs (e.g., missing command structure).
 */
uint8_t	handle_unset(t_cmd *cmd)
{
	if (!cmd || !cmd->minishell)
	{
		print_error("minishell: unset: no cmd or minishell instanse\n");
		return (EXIT_FAILURE);
	}
	if (!cmd->argv[1])
		return (EXIT_SUCCESS);
	return (do_unset_loop(cmd));
}
