/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_hash_table_utils_2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 09:49:13 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/05/12 14:02:21 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file setup_hash_table_utils_2.c
 * @brief Utility functions for parsing environment variable strings
 *        and computing hash values for environment storage.
 */
#include "minishell.h"

/**
 * @brief Parses and inserts one env entry to hash table.
 *
 * @param mshell Shell instance.
 * @param entry Key=value environment string.
 */
int	insert_env_var(t_mshell *mshell, char *entry)
{
	t_mshell_var	*tmp;

	tmp = split_key_value(entry);
	if (!tmp)
		return (EXIT_FAILURE);
	if (set_variable(mshell, tmp->key, tmp->value, 1) != EXIT_SUCCESS)
	{
		free_pair(&tmp);
		print_error("-minishell: failed to insert env var\n");
		return (EXIT_FAILURE);
	}
	free_pair(&tmp);
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes SHLVL to 1 if not already present in the environment.
 *
 * @param minishell Shell instance.
 * @return EXIT_SUCCESS or EXIT_FAILURE.
 */
uint8_t	update_shlvl_setup_no_env(t_mshell *minishell)
{
	char	*str_shlvl;

	str_shlvl = ms_getenv(minishell, "SHLVL");
	if (!str_shlvl)
	{
		if (set_variable(minishell, "SHLVL", "1", 1) != EXIT_SUCCESS)
		{
			print_error("-minishell: update_shlvl, set_variable failed\n");
			return (EXIT_FAILURE);
		}
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Sets or updates an environment variable.
 *
 * If key exists, updates value. Otherwise inserts new variable.
 *
 * @param mshell Pointer to Minishell.
 * @param key Variable name.
 * @param value Variable value.
 * @param assigned Assignment flag.
 * @return EXIT_SUCCESS or EXIT_FAILURE.
 */
int	set_variable(t_mshell *mshell, char *key, char *value, int assigned)
{
	t_mshell_var	*mshell_var;
	unsigned int	index;

	if (!mshell || !key || !mshell->hash_table)
	{
		print_error("-minishell: set_variable: null pointer argument\n");
		return (EXIT_FAILURE);
	}
	index = hash_function(key);
	mshell_var = mshell->hash_table->buckets[index];
	while (mshell_var)
	{
		if (ft_strcmp(mshell_var->key, key) == 0)
			return (update_existing_var(mshell_var, value, assigned));
		mshell_var = mshell_var->next;
	}
	return (insert_new_var(mshell, key, value, assigned));
}

/**
 * @brief Inserts a new variable into the hash table.
 *
 * Computes hash index and inserts at bucket head.
 *
 * @param mshell Pointer to shell instance.
 * @param key Variable name.
 * @param value Variable value.
 * @param assigned Assignment flag.
 * @return EXIT_SUCCESS or EXIT_FAILURE.
 */
int	insert_new_var(t_mshell *mshell, char *key,
		char *value, int assigned)
{
	t_mshell_var	*new_var;
	unsigned int	index;

	if (!mshell || !mshell->hash_table || !key)
	{
		print_error("-minishell: insert_new_var: null argument\n");
		return (EXIT_FAILURE);
	}
	new_var = create_new_var(key, value, assigned);
	if (!new_var)
		return (EXIT_FAILURE);
	index = hash_function(key);
	new_var->next = mshell->hash_table->buckets[index];
	mshell->hash_table->buckets[index] = new_var;
	return (EXIT_SUCCESS);
}

/**
 * @brief Updates value and assignment flag of an existing variable.
 *
 * Replaces the variable's value if a new one is given.
 *
 * @param var Pointer to the variable to update.
 * @param value New value string (can be NULL).
 * @param assigned 1 if value is assigned, 0 otherwise.
 * @return EXIT_SUCCESS or EXIT_FAILURE on allocation error.
 */
int	update_existing_var(t_mshell_var *mshell_var, char *value, int assigned)
{
	char	*dup;

	if (value)
	{
		dup = ft_strdup(value);
		if (!dup)
		{
			print_error("-minishell: update_existing_var, strdup failed\n");
			return (EXIT_FAILURE);
		}
		free(mshell_var->value);
		mshell_var->value = dup;
	}
	mshell_var->val_assigned = assigned;
	return (EXIT_SUCCESS);
}
