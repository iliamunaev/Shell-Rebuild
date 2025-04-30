/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_execve_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:43:26 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/04/30 18:37:14 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * @file child_execve_error.c
 * @brief Utilities for handling execve-related errors in child processes.
 *
 * Provides structured error reporting and graceful exiting for failed execve
 * calls, including memory cleanup and exit status handling.
 */
#include "minishell.h"

/**
 * @brief Prints an error message and exits the process with a given code.
 *
 * This function combines:
 * - Optional prefix (e.g., shell name)
 * - Argument (e.g., command or file)
 * - Message (e.g., error description)
 *
 * It also frees the Minishell context if provided and then exits
 * with the specified status code.
 *
 * @param info A `t_exit_info` struct containing error message parts
 *             and exit metadata.
 */
// void	print_and_exit(t_exit_info info)
// {
// 	if (info.prefix)
// 		print_error(info.prefix);
// 	if (info.arg)
// 		print_error(info.arg);
// 	if (info.msg)
// 		print_error(info.msg);
// 	if (info.mshell)
// 		free_minishell(info.mshell);
// 	_exit(info.code);
// }
void	print_and_exit(t_exit_info info)
{
	char	buf[ERROR_BUF_SIZE];

	buf[0] = '\0';
	if (info.prefix)
		ft_strlcat(buf, info.prefix, ERROR_BUF_SIZE);
	if (info.arg)
		ft_strlcat(buf, info.arg, ERROR_BUF_SIZE);
	if (info.msg)
		ft_strlcat(buf, info.msg, ERROR_BUF_SIZE);
	if (*buf)
		write(STDERR_FILENO, buf, ft_strlen(buf));
	if (info.mshell)
		free_minishell(info.mshell);
	_exit(info.code);
}

/**
 * @brief Dispatches all possible execve error handlers.
 *
 * Attempts to detect and handle the specific cause of an execve failure by
 * invoking a series of checks:
 * - Is the binary a directory?
 * - Is it missing or not found?
 * - Does it have permission issues?
 * - Is it in an invalid format?
 * - Or a generic error?
 *
 * Terminates the process with the corresponding error code.
 *
 * @param cmd The command structure passed to execve.
 */
void	child_execve_error(t_cmd *cmd)
{
	handle_is_directory(cmd);
	handle_not_found_or_command(cmd);
	handle_permission_denied(cmd);
	handle_exec_format_error(cmd);
	handle_generic_execve_error(cmd);
}
