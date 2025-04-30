/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:46:52 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/04/30 17:41:13 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file execute_command.c
 * @brief Handles execution of commands (built-in and external) in Minishell.
 *
 * Contains logic to:
 * - Validate and execute built-in commands.
 * - Execute external binaries using `execve`.
 * - Handle syntax and shell-specific cases such as `.`/`..`, `env`,
 * and empty inputs.
 * - Manage cleanup and exit codes inside child processes.
 */
#include "minishell.h"

/**
 * @brief Executes a built-in command in a child process and exits.
 *
 * This function is called in the forked child process when the command
 * is a built-in with no external binary path. It executes the built-in,
 * frees the shell context, and exits with the appropriate status code.
 *
 * @param cmd Pointer to the command structure.
 */
static void	handle_builtin_and_exit(t_cmd *cmd)
{
	uint8_t	exit_status;

	exit_status = exec_builtins(cmd);
	free_minishell(cmd->minishell);
	_exit(exit_status);
}

/**
 * @brief Executes an external binary command using `execve()`.
 *
 * First validates special dot (`.`) and dot-dot (`..`) commands.
 * If validation passes, it attempts to execute the binary with `execve`.
 * On failure, it handles and prints an appropriate error message.
 *
 * @param cmd Pointer to the command structure.
 */
void	exec_cmd(t_cmd *cmd)
{
	uint8_t	exit_status;

	exit_status = validate_dots(cmd);
	if (exit_status != EXIT_SUCCESS)
	{
		free_minishell(cmd->minishell);
		_exit(exit_status);
	}
	signal(SIGPIPE, SIG_DFL);
	execve(cmd->binary, cmd->argv, cmd->minishell->env);
	child_execve_error(cmd);
}

// /**
//  * @brief Entry point for executing a command in a child process.
//  *
//  * Handles several cases:
//  * - Skips execution if the command is empty.
//  * - Exits immediately if there was a previous syntax error.
//  * - Updates `SHLVL` if this is a recursive call to `./minishell`.
//  * - Processes empty strings as invalid commands.
//  * - For built-ins without a binary, calls the built-in directly.
//  * - For valid binaries, delegates to `exec_cmd()`.
//  *
//  * @param cmd Pointer to the command structure.
//  */
// void	execute_command(t_cmd *cmd)
// {

// 	uint8_t	exit_status;

// 	if (!cmd || !cmd->argv || !cmd->argv[0])
// 	{
// 		free_minishell(cmd->minishell);
// 		_exit(EXIT_SUCCESS);
// 	}
// 	if (cmd->minishell->syntax_exit_status != 0)
// 	{
// 		exit_status = cmd->minishell->syntax_exit_status;
// 		free_minishell(cmd->minishell);
// 		_exit(exit_status);
// 	}
// 	if (is_minishell_executable(cmd) && update_shlvl(cmd) == EXIT_FAILURE)
// 	{
// 		free_minishell(cmd->minishell);
// 		_exit(EXIT_FAILURE);
// 	}
// 	if (ft_strcmp(cmd->argv[0], "") == 0)
// 		handle_empty_command(cmd);
// 	if (!cmd->binary)
// 	{
// 		if (is_builtin(cmd))
// 		{
// 			if (ft_strcmp(cmd->argv[0], "env") == 0)
// 				update_underscore(cmd, cmd->binary);
// 			handle_builtin_and_exit(cmd);
// 		}
// 		else
// 			cmd_missing_command_error(cmd);
// 	}
// 	exec_cmd(cmd);
// }
void	execute_command_core(t_cmd *cmd)
{
	if (is_minishell_executable(cmd) && update_shlvl(cmd) == EXIT_FAILURE)
	{
		free_minishell(cmd->minishell);
		_exit(EXIT_FAILURE);
	}
	if (!cmd->binary)
	{
		if (is_builtin(cmd))
		{
			if (ft_strcmp(cmd->argv[0], "env") == 0)
				update_underscore(cmd, cmd->binary);
			handle_builtin_and_exit(cmd);
		}
		else
			cmd_missing_command_error(cmd);
	}
	exec_cmd(cmd);
}

void	execute_command(t_cmd *cmd)
{
	uint8_t	exit_status;

	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		free_minishell(cmd->minishell);
		_exit(EXIT_SUCCESS);
	}
	if (cmd->minishell->syntax_exit_status != 0)
	{
		exit_status = cmd->minishell->syntax_exit_status;
		free_minishell(cmd->minishell);
		_exit(exit_status);
	}
	if (ft_strcmp(cmd->argv[0], "") == 0)
		handle_empty_command(cmd);
	execute_command_core(cmd);
}
