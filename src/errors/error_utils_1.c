/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:43:42 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/04/30 18:19:57 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file error_utils_1.c
 * @brief Error reporting utilities for built-in and execution errors
 * in Minishell.
 *
 * Contains helper functions for printing error messages to stderr in
 * a consistent format.
 * These are used across built-in commands and execve-related failures.
 */
#include "minishell.h"

/**
 * @brief Writes an error message to `STDERR_FILENO`.
 *
 * @param msg The message to print.
 */
// void	print_error(const char *msg)
// {
// 	ft_putstr_fd(msg, STDERR_FILENO);
// }

void	print_error(const char *s)
{
	if (!s)
		return ;
	while (*s)
		write(STDERR_FILENO, s++, 1);
}

/**
 * @brief Prints a formatted error message for invalid `export` arguments.
 *
 * Format: `-minishell: export: \`KEY=VALUE\`: not a valid identifier`
 *
 * @param pair A key-value pair representing the invalid export variable.
 */
// void	export_error(t_mshell_var *pair)
// {
// 	print_error("-minishell: export: `");
// 	print_error(pair->key);
// 	if (pair->value)
// 	{
// 		print_error("=");
// 		print_error(pair->value);
// 	}
// 	print_error("': not a valid identifier\n");
// }
void	export_error(t_mshell_var *pair)
{
	char	error_buf[ERROR_BUF_SIZE];

	error_buf[0] = '\0';
	ft_strlcpy(error_buf, "-minishell: export: `", ERROR_BUF_SIZE);
	ft_strlcat(error_buf, pair->key, ERROR_BUF_SIZE);
	if (pair->value)
	{
		ft_strlcat(error_buf, "=", ERROR_BUF_SIZE);
		ft_strlcat(error_buf, pair->value, ERROR_BUF_SIZE);
	}
	ft_strlcat(error_buf, "': not a valid identifier\n", ERROR_BUF_SIZE);
	write(STDERR_FILENO, error_buf, ft_strlen(error_buf));
}
/**
 * @brief Prints an error message for invalid `unset` options.
 *
 * Format:
 * ```
 * -minishell: unset: <str>: invalid option
 * unset: usage: unset [name ...]
 * ```
 *
 * @param str The invalid option string (expected to be 1-2 characters).
 * @return Always returns `2` (invalid option status).
 */
// u_int8_t	unset_error(char *str)
// {
// 	print_error("-minishell: unset: ");
// 	(void)write(STDERR_FILENO, str, 2);
// 	print_error(": invalid option\n");
// 	print_error("unset: usage: unset [name ...]\n");
// 	return (2);
// }
uint8_t	unset_error(const char *str)
{
	char	error_buf[ERROR_BUF_SIZE];

	error_buf[0] = '\0';
	ft_strlcpy(error_buf, "-minishell: unset: ", ERROR_BUF_SIZE);
	ft_strlcat(error_buf, str, ERROR_BUF_SIZE);
	ft_strlcat(error_buf, ": invalid option\n", ERROR_BUF_SIZE);
	ft_strlcat(error_buf, "unset: usage: unset [name ...]\n", ERROR_BUF_SIZE);
	write(STDERR_FILENO, error_buf, ft_strlen(error_buf));
	return (2);
}
/**
 * @brief Handles and prints an error for a missing or invalid command.
 *
 * Behavior:
 * - If the command is not found or executable, an error is printed.
 * - Exits the shell with status `127`.
 *
 * @param cmd The command structure.
 */
// void	cmd_missing_command_error(t_cmd *cmd)
// {
// 	const char	*path;

// 	if (!cmd || !cmd->argv || !cmd->argv[0])
// 	{
// 		print_error("-minishell: invalid cmd structure\n");
// 		free_minishell(cmd->minishell);
// 		_exit(127);
// 	}
// 	print_error("-minishell: ");
// 	print_error(cmd->argv[0]);
// 	path = ms_getenv(cmd->minishell, "PATH");
// 	if (ft_strchr(cmd->argv[0], '/') || !path || path[0] == '\0')
// 		print_error(": No such file or directory\n");
// 	else
// 		print_error(": command not found\n");
// 	free_minishell(cmd->minishell);
// 	_exit(127);
// }

void	cmd_missing_command_error(t_cmd *cmd)
{
	char		error_buf[ERROR_BUF_SIZE];
	const char	*path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		ft_strlcpy(error_buf, "-minishell: invalid cmd structure\n", ERROR_BUF_SIZE);
		write(STDERR_FILENO, error_buf, ft_strlen(error_buf));
		if (cmd)
			free_minishell(cmd->minishell);
		_exit(127);
	}
	error_buf[0] = '\0';
	ft_strlcat(error_buf, "-minishell: ", ERROR_BUF_SIZE);
	ft_strlcat(error_buf, cmd->argv[0], ERROR_BUF_SIZE);
	path = ms_getenv(cmd->minishell, "PATH");
	if (ft_strchr(cmd->argv[0], '/') || !path || path[0] == '\0')
		ft_strlcat(error_buf, ": No such file or directory\n", ERROR_BUF_SIZE);
	else
		ft_strlcat(error_buf, ": command not found\n", ERROR_BUF_SIZE);
	write(STDERR_FILENO, error_buf, ft_strlen(error_buf));
	free_minishell(cmd->minishell);
	_exit(127);
}

/**
 * @brief Prints an error message and returns an exit status.
 *
 * Used as a shortcut for reporting an error and returning the
 * associated error code.
 *
 * @param msg The message to print.
 * @param exit_status The code to return.
 * @return The same `exit_status` passed in.
 */
// int	error_return(char *msg, int exit_status)
// {
// 	ft_putstr_fd("-minishell: ", STDERR_FILENO);
// 	if (msg)
// 		print_error(msg);
// 	return (exit_status);
// }

int	error_return(char *msg, int exit_status)
{
	char	error_buf[ERROR_BUF_SIZE];

	error_buf[0] = '\0';
	ft_strlcpy(error_buf, "-minishell: ", ERROR_BUF_SIZE);
	if (msg)
		ft_strlcat(error_buf, msg, ERROR_BUF_SIZE);
	ft_strlcat(error_buf, "\n", ERROR_BUF_SIZE);
	write(STDERR_FILENO, error_buf, ft_strlen(error_buf));
	return (exit_status);
}
