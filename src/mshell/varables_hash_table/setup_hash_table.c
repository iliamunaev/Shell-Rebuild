/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_hash_table.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:50:27 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/05/09 16:52:28 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parses and inserts one env entry to hash table.
 *
 * @param mshell Shell instance.
 * @param entry Key=value environment string.
 */
static void	insert_env_var(t_mshell *mshell, char *entry)
{
	t_mshell_var	*tmp;

	tmp = split_key_value(entry);
	if (!tmp)
		return ;
	(void)set_variable(mshell, tmp->key, tmp->value, 1);
	free_pair_and_return_null(tmp);
}

/**
 * @brief Ensures OLDPWD is initialized from HOME in the hash table.
 *
 * @param mshell Shell instance.
 * @return EXIT_SUCCESS or EXIT_FAILURE.
 */
static int	add_oldpwd_from_home(t_mshell *mshell)
{
	char	*home;
	char	working_dir[MS_PATHMAX];

	home = ms_getenv(mshell, "HOME");
	if (!home)
	{
		home = getcwd(working_dir, MS_PATHMAX);
		set_variable(mshell, "PWD", home, 1);
		// print_error("minishell: load_env_into_ht: get home dir failed\n");
		// return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	
	}
	if (set_variable(mshell, "OLDPWD", home, 1) != EXIT_SUCCESS)
	{
		print_error("-minishell: load_env_into_ht, set_variable failed\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Loads the environment array into the hash table.
 *
 * Iterates over mshell->env and populates hash table.
 *
 * @param mshell Shell instance.
 * @return EXIT_SUCCESS or EXIT_FAILURE.
 */
static int	load_env_into_ht(t_mshell *mshell)
{
	int	i;

	i = 0;
	while (mshell->env[i])
		insert_env_var(mshell, mshell->env[i++]);
	return (add_oldpwd_from_home(mshell));
}

/**
 * @brief Initializes an empty hash table.
 *
 * Allocates and zeroes all bucket pointers.
 *
 * @return Pointer to hash table, or NULL on failure.
 */
static t_hash_tbl	*init_hash_tbl(void)
{
	t_hash_tbl	*ht;
	int			i;

	ht = malloc(sizeof(t_hash_tbl));
	if (!ht)
		return (print_error("-minishell: hash_table malloc failed\n"), NULL);
	i = 0;
	while (i < HASH_SIZE)
		ht->buckets[i++] = NULL;
	return (ht);
}

/**
 * @brief Sets up the shell's hash table and populates it from env.
 *
 * @param mshell Shell instance.
 * @return EXIT_SUCCESS or EXIT_FAILURE.
 */
int	setup_hash_table(t_mshell *mshell)
{
	mshell->hash_table = init_hash_tbl();
	if (!mshell->hash_table)
		return (EXIT_FAILURE);
	if (load_env_into_ht(mshell) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (update_env(mshell) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
