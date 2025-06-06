/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:44:00 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/05/16 10:57:31 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file cd_utils.c
 * @brief Utility functions used by the `cd` built-in implementation
 * in Minishell.
 *
 * This file provides helper functions that support the core logic in
 * `handle_cd.c`,
 * including directory resolution and environment fallbacks.
 */
#include "minishell.h"

/**
 * @brief Reports an error when too many arguments are passed to `cd`.
 *
 * The `cd` built-in in Minishell supports only 0 or 1 arguments. If more than
 * one is detected (e.g., `cd dir1 dir2`), this function is called to print a
 * descriptive error message.
 *
 * @return Always returns `EXIT_FAILURE` (1) to signal argument misuse.
 */
uint8_t	cd_too_many_args(void)
{
	return (error_return("cd: too many arguments\n", EXIT_FAILURE));
}

/**
 * @brief Updates the `PWD` and `OLDPWD` environment variables after a
 * directory change.
 *
 * This function:
 * - Retrieves the new working directory.
 * - Sets `OLDPWD` to the previous directory.
 * - Sets `PWD` to the current directory.
 * - Triggers a rebuild of the exported environment with `update_env()`.
 *
 * @param cmd      Pointer to the current command structure.
 * @param old_cwd  The previous working directory before the `cd` call.
 */
void	update_pwd_variables(t_cmd *cmd, char *old_cwd)
{
	char	new_cwd[MS_PATHMAX];

	ft_bzero(new_cwd, MS_PATHMAX);
	if (!get_directory(new_cwd, cmd))
		ft_strlcpy(new_cwd, "", MS_PATHMAX);
	if (set_variable(cmd->minishell, "OLDPWD", old_cwd, 1) != EXIT_SUCCESS)
	{
		print_error("-minishell: update_pwd_variables, OLDPWD failed\n");
		return ;
	}
	if (set_variable(cmd->minishell, "PWD", new_cwd, 1) != EXIT_SUCCESS)
	{
		print_error("-minishell: update_pwd_variables, OLDPWD failed\n");
		return ;
	}
	if (update_env(cmd->minishell) != EXIT_SUCCESS)
	{
		print_error("-minishell: update_pwd_variables, update_env failed\n");
		return ;
	}
}

/**
 * @brief Retrieves the current working directory with fallback support.
 *
 * Attempts to obtain the current working directory using `getcwd()`. If it fails
 * (e.g., due to permission errors or deleted working directory), the function
 * falls back to retrieving the `$PWD` environment variable from the shell's
 * internal environment.
 *
 * The result is stored in the provided buffer `cwd`, which must be at least
 * `MS_PATHMAX` bytes in size.
 *
 * @param cwd  Buffer to store the resolved current working directory path.
 * @param cmd  Pointer to the command structure, used to access shell state
 * and environment.
 * @return `true` if `getcwd()` succeeds, `false` if `$PWD` is used as
 * a fallback.
 *
 * @note The function does not allocate memory. Always zero or validate `cwd
 *  before use.
 */
bool	get_directory(char *cwd, t_cmd *cmd)
{
	char	*temp_pwd;

	if (getcwd(cwd, MS_PATHMAX))
		return (true);
	temp_pwd = ms_getenv(cmd->minishell, "PWD");
	if (temp_pwd)
		ft_strlcpy(cwd, temp_pwd, MS_PATHMAX);
	else
		ft_strlcpy(cwd, "", MS_PATHMAX);
	return (false);
}

/**
 * @brief Main logic for changing directory and updating PWD/OLDPWD.
 *
 * Handles both cd - and cd <path> logic and updates the environment
 * accordingly after a successful change.
 *
 * @param cmd Pointer to the command structure.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
uint8_t	change_and_update_pwd(t_cmd *cmd)
{
	char	old_cwd[MS_PATHMAX];
	uint8_t	status;

	if (!cmd || !cmd->argv[1] || !cmd->minishell)
		return (EXIT_FAILURE);
	ft_bzero(old_cwd, MS_PATHMAX);
	(void)get_directory(old_cwd, cmd);
	if (ft_strcmp(cmd->argv[1], "-") == 0)
		status = handle_cd_dash(cmd);
	else
		status = handle_cd_path(cmd->argv[1]);
	if (status == EXIT_FAILURE)
		return (EXIT_FAILURE);
	update_pwd_variables(cmd, old_cwd);
	return (EXIT_SUCCESS);
}
