/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_execve_error_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:43:16 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/04/30 18:26:37 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file child_execve_error.c
 * @brief Error handlers for execve failures in child processes.
 *
 * Provides functions to handle various `execve`-related errors
 * such as permission denied, invalid format, or trying to execute a directory.
 */
#include "minishell.h"

/**
 * @brief Handles the case when a directory is executed instead of a file.
 *
 * Checks if the command's binary path is a directory and exits with code 126
 * if true, printing an appropriate error message.
 *
 * @param cmd The command structure containing the binary path.
 */
void	handle_is_directory(t_cmd *cmd)
{
	struct stat	st;

	if (stat(cmd->binary, &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_and_exit((t_exit_info){
			.prefix = "-minishell: ",
			.arg = cmd->binary,
			.msg = ": Is a directory\n",
			.code = 126,
			.mshell = cmd->minishell
		});
	}
}

/**
 * @brief Handles "command not found" or "no such file" scenarios.
 *
 * - If the command is not found in PATH or doesn't exist at all,
 *   it prints an appropriate error message and exits with code 127.
 * - Follows shell behavior for commands with or without slashes.
 *
 * @param cmd The command structure containing argv and shell context.
 */
// void	handle_not_found_or_command(t_cmd *cmd)
// {
// 	char	*path;

// 	if (errno != ENOENT)
// 		return ;
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

void	handle_not_found_or_command(t_cmd *cmd)
{
	char	*path;
	char	buf[ERROR_BUF_SIZE];

	if (errno != ENOENT || !cmd || !cmd->argv || !cmd->argv[0])
		return ;

	buf[0] = '\0';
	ft_strlcpy(buf, "-minishell: ", ERROR_BUF_SIZE);
	ft_strlcat(buf, cmd->argv[0], ERROR_BUF_SIZE);

	path = ms_getenv(cmd->minishell, "PATH");
	if (ft_strchr(cmd->argv[0], '/') || !path || path[0] == '\0')
		ft_strlcat(buf, ": No such file or directory\n", ERROR_BUF_SIZE);
	else
		ft_strlcat(buf, ": command not found\n", ERROR_BUF_SIZE);

	write(STDERR_FILENO, buf, ft_strlen(buf));
	free_minishell(cmd->minishell);
	_exit(127);
}

/**
 * @brief Handles the case where the binary is not executable due to permissions.
 *
 * If `errno` is set to `EACCES`, it prints a permission denied error
 * and exits with status 126.
 *
 * @param cmd The command structure with execution context.
 */
void	handle_permission_denied(t_cmd *cmd)
{
	if (errno == EACCES)
	{
		print_and_exit((t_exit_info){
			.prefix = "-minishell: ",
			.arg = cmd->binary,
			.msg = ": Permission denied\n",
			.code = 126,
			.mshell = cmd->minishell
		});
	}
}

/**
 * @brief Handles the `ENOEXEC` error from `execve`.
 *
 * Prints an "Exec format error" message if the binary has an invalid format
 * and exits with status 126.
 *
 * @param cmd The command being executed.
 */
void	handle_exec_format_error(t_cmd *cmd)
{
	if (errno == ENOEXEC)
	{
		print_and_exit((t_exit_info){
			.prefix = "-minishell: ",
			.arg = cmd->binary,
			.msg = ": Exec format error\n",
			.code = 126,
			.mshell = cmd->minishell
		});
	}
}

/**
 * @brief Handles any generic `execve` error not covered by specific cases.
 *
 * Prints the strerror output and frees all allocated shell resources
 * before exiting with `EXIT_FAILURE`.
 *
 * @param cmd The command that caused the error.
 */
// void	handle_generic_execve_error(t_cmd *cmd)
// {
// 	print_error("-minishell: execve: ");
// 	print_error(strerror(errno));
// 	print_error("\n");
// 	free_minishell(cmd->minishell);
// 	_exit(EXIT_FAILURE);
// }
void	handle_generic_execve_error(t_cmd *cmd)
{
	char	buf[ERROR_BUF_SIZE];

	buf[0] = '\0';
	ft_strlcpy(buf, "-minishell: execve: ", ERROR_BUF_SIZE);
	ft_strlcat(buf, strerror(errno), ERROR_BUF_SIZE);
	ft_strlcat(buf, "\n", ERROR_BUF_SIZE);
	write(STDERR_FILENO, buf, ft_strlen(buf));
	free_minishell(cmd->minishell);
	_exit(EXIT_FAILURE);
}
