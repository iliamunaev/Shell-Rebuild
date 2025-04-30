/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:43:50 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/04/30 19:00:13 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file error_utils_2.c
 * @brief Utility functions for printing and handling errors with `perror()`
 * in Minishell.
 *
 * Provides helper functions to print system error messages using `perror()`
 * with custom prefixes,
 * and to exit or return with appropriate statuses in error conditions.
 */
#include "minishell.h"

/**
 * @brief Prints a system error message with `perror()` and returns a given
 * status.
 *
 * Prepends "-minishell: " to the output for consistent formatting.
 *
 * @param msg Message to pass to `perror()` for context.
 * @param exit_status Exit code to return.
 * @return The same `exit_status` passed in.
 */
// int	perror_return(char *msg, int exit_status)
// {
// 	ft_putstr_fd("-minishell: ", STDERR_FILENO);
// 	if (msg)
// 		perror(msg);
// 	return (exit_status);
// }

int	perror_return(const char *msg, int exit_status)
{
	char	buf[ERROR_BUF_SIZE];
	const char	*err_msg;

	buf[0] = '\0';
	ft_strlcpy(buf, "-minishell: ", ERROR_BUF_SIZE);
	if (msg)
	{
		ft_strlcat(buf, msg, ERROR_BUF_SIZE);
		ft_strlcat(buf, ": ", ERROR_BUF_SIZE);
	}
	err_msg = strerror(errno);
	ft_strlcat(buf, err_msg, ERROR_BUF_SIZE);
	ft_strlcat(buf, "\n", ERROR_BUF_SIZE);
	write(STDERR_FILENO, buf, ft_strlen(buf));
	return (exit_status);
}
/**
 * @brief Prints a system error using `perror()` and immediately exits the
 * child process.
 *
 * Used in forked child processes when an unrecoverable error occurs.
 *
 * @param msg Error context to pass to `perror()`.
 * @param exit_status Status code to exit the process with.
 * @return Never returns, always exits.
 */
uint8_t	perror_exit_child(t_cmd *cmd, char *msg, u_int8_t exit_status)
{
	if (msg)
		perror(msg);
	if (cmd->minishell)
		free_minishell(cmd->minishell);
	_exit(exit_status);
}

/**
 * @brief Prints a message indicating a missing command structure or arguments.
 *
 * Used when the `t_cmd` structure is unexpectedly `NULL` or has no arguments.
 *
 * Format: `-minishell: <msg>: no cmd or argv found`
 *
 * @param msg A custom string for context (e.g., the command name).
 * @return Always returns `EXIT_FAILURE` (1).
 */
// uint8_t	no_cmd_error(char *msg)
// {
// 	print_error("-minishell: ");
// 	print_error(msg);
// 	print_error(": no cmd or argv found\n");
// 	return (EXIT_FAILURE);
// }
uint8_t	no_cmd_error(const char *msg)
{
	char	buf[ERROR_BUF_SIZE];

	buf[0] = '\0';
	ft_strlcpy(buf, "-minishell: ", ERROR_BUF_SIZE);
	if (msg)
	{
		ft_strlcat(buf, msg, ERROR_BUF_SIZE);
		ft_strlcat(buf, ": ", ERROR_BUF_SIZE);
	}
	ft_strlcat(buf, "no cmd or argv found\n", ERROR_BUF_SIZE);
	write(STDERR_FILENO, buf, ft_strlen(buf));
	return (EXIT_FAILURE);
}
