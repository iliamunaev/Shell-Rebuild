/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_binary.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:50:39 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/05/12 14:00:23 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file find_binary.c
 * @brief Resolves the full path to an external binary for execution.
 *
 * This file contains logic for resolving a command's executable binary path.
 * It supports:
 * - Absolute or relative paths (`/usr/bin/ls`, `./script.sh`)
 * - Searching through `$PATH` directories for the binary name
 * - Falling back when `$PATH` is unset or empty
 * - Returning `NULL` for built-in commands
 */
#include "minishell.h"

/**
 * @brief Handles execution when the command is given as a direct path.
 *
 * Duplicates the `cmd->argv[0]` value (e.g., `/bin/ls` or `./script.sh`).
 * This function assumes the input is already a valid path.
 *
 * @param cmd Pointer to the command structure.
 * @return A duplicated string of the path to the binary.
 */
static char	*handle_direct_path(t_cmd *cmd)
{
	char	*binary;

	binary = ft_strdup(cmd->argv[0]);
	return (binary);
}

/**
 * @brief Searches for the binary in the directories listed in the
 * `PATH` variable.
 *
 * - Splits the `PATH` environment variable by `:` and searches each directory.
 * - Returns the full path to the binary if found.
 * - If `PATH` is not set or memory allocation fails, sets exit status to 127.
 *
 * @param cmd Pointer to the command structure.
 * @return Full path to binary on success, or `NULL` if not found.
 */
static char	*handle_path_search(t_cmd *cmd)
{
	char	*env;
	char	**paths;
	char	*binary;

	env = ms_getenv(cmd->minishell, "PATH");
	if (!env || !*env)
	{
		cmd->minishell->exit_status = 127;
		return (NULL);
	}
	paths = ft_split(env, ':');
	if (!paths)
	{
		cmd->minishell->exit_status = 127;
		return (NULL);
	}
	binary = search_paths(paths, cmd);
	ft_free_arrstrs(paths);
	return (binary);
}

/**
 * @brief Handles path resolution for binaries when PATH is empty or not found.
 *
 * If PATH is not set, we check if the command itself is directly executable.
 * If yes, return a strdup of it. Otherwise, set exit_status to 127
 * and return NULL.
 *
 * @param cmd Pointer to the command structure.
 * @return Full path (heap-allocated) or NULL.
 */
static char	*handle_missing_path(t_cmd *cmd)
{
	char	*dup;

	if (access(cmd->argv[0], F_OK) == 0)
	{
		dup = ft_strdup(cmd->argv[0]);
		if (!dup)
		{
			print_error("-minishell: ft_strdup failed in find_binary\n");
			cmd->minishell->exit_status = 1;
			return (NULL);
		}
		return (dup);
	}
	cmd->minishell->exit_status = 127;
	return (NULL);
}

/**
 * @brief Finds the full path to the binary to execute for a given command.
 *
 * Handles different cases:
 * - Built-ins: returns `NULL` (they don’t need a path).
 * - Direct paths: `/bin/ls`, `./script.sh`
 * - Binary names searched via `$PATH`.
 * - Fallback when `$PATH` is missing: checks if command is directly executable.
 *
 * @param cmd Pointer to the command structure.
 * @return Full path to the binary (heap-allocated) or `NULL` on failure.
 */
char	*find_binary(t_cmd *cmd)
{
	char	*binary;
	char	*path_env;

	if (!cmd || !cmd->argv || !cmd->argv[0] || ft_strlen(cmd->argv[0]) == 0)
	{
		cmd->minishell->exit_status = 127;
		return (NULL);
	}
	if (is_builtin(cmd))
		return (NULL);
	if (cmd->argv[0][0] == '/' || cmd->argv[0][0] == '.')
		return (handle_direct_path(cmd));
	path_env = ms_getenv(cmd->minishell, "PATH");
	if (!path_env || path_env[0] == '\0')
		return (handle_missing_path(cmd));
	binary = handle_path_search(cmd);
	return (binary);
}
