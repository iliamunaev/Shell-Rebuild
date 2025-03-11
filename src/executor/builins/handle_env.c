/**
 * @file handle_env.c
 * @brief Functions for displaying environment variables in Minishell.
 */
#include "minishell.h"

/**
 * @brief Prints all assigned environment variables from a single hash table bucket.
 *
 * Iterates through the linked list of environment variables in the given bucket
 * and prints each assigned variable in the format `KEY=VALUE`. If the variable has
 * no value, it is displayed as `KEY=`.
 *
 * @param var Pointer to the first variable in the bucket.
 */
static void	print_env_bucket(t_mshell_var *var)
{
	while (var)
	{
		if (var->val_assigned)
		{
			if (var->value)
				printf("%s=%s\n", var->key, var->value);
			else
				printf("%s=\n", var->key);
		}
		var = var->next;
	}
}

/**
 * @brief Handles the `env` built-in command to display environment variables.
 *
 * Iterates through the hash table and prints all assigned environment variables.
 * If the command is executed in a pipeline, the process exits with the appropriate status.
 *
 * @param cmd Pointer to the command structure containing execution context.
 * @return `EXIT_SUCCESS` on success.
 */
uint8_t	handle_env(t_cmd *cmd)
{
	t_hash_table	*ht;
	int				i;
	uint8_t			exit_status;

	ht = cmd->minishell->hash_table;
	i = 0;
	while (i < HASH_SIZE)
	{
		print_env_bucket(ht->buckets[i]);
		i++;
	}
	exit_status = (EXIT_SUCCESS);
	cmd->minishell->exit_status = exit_status;

	printf("Exit status: %d\n",cmd->minishell->exit_status); //  test
	if (cmd->in_pipe)
		exit(exit_status);
	return (exit_status);
}
