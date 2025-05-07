/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:49:29 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/05/07 02:38:27 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file executor.c
 * @brief Functions for executing built-in and external commands in Minishell.
 *
 * This module determines how to execute parsed commands — either as built-in
 * shell commands in the current process or as external binaries via pipelines.
 * It handles heredoc setup, environment management, and proper exit handling
 * for commands such as `exit`.
 */
#include "minishell.h"

/**
 * @brief Executes commands in a pipeline or as external binaries.
 *
 * If the command list contains multiple commands, it forks and pipes
 * each command. If the command is a binary (not a built-in),
 * it is executed using `execve()` within a child process.
 * Also handles cleanup if the last command is `exit`.
 *
 * @param cmd Pointer to the first command in the pipeline.
 * @return Exit status of the last command executed.
 */
static uint8_t	execute_pipeline_or_binary(t_cmd *cmd)
{
	t_mshell	*minishell;
	uint8_t		exit_status;

	minishell = cmd->minishell;
	exit_status = exec_in_pipes(cmd);
	minishell->exit_status = exit_status;
	if (is_exit_command(cmd))
		cleanup_and_exit(cmd, exit_status);
	return (exit_status);
}

/**
 * @brief Executes a built-in command in the current process.
 *
 * This function runs built-in commands (like `cd`, `echo`, `exit`) directly
 * without forking a child. If the command is `exit`, it performs full
 * cleanup before exiting.
 *
 * @param cmd Pointer to the command structure.
 * @return Exit status of the built-in command.
 */
static uint8_t	execute_builtin(t_cmd *cmd)
{
	t_mshell	*minishell;
	uint8_t		exit_status;

	minishell = cmd->minishell;
	exit_status = exec_in_current_process(cmd);
	minishell->exit_status = exit_status;
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
	{
		if (isatty(STDIN_FILENO))
			ft_putendl_fd("exit", STDERR_FILENO);
		if (!(cmd->argv[1] && cmd->argv[2]
				&& is_valid_numeric_exit_arg(cmd->argv[1])))
		{
			cleanup_and_exit(cmd, exit_status);
		}
	}
	return (exit_status);
}

uint8_t	handle_executor_signal_exit(t_cmd *cmd,
	t_mshell *minishell,
	uint8_t exit_status)
{
	t_cmd	*head;

	fprintf(stderr, "DEBUG: handle_executor_signal_exit, exit_status: %d\n", exit_status);


	head = get_cmd_head(cmd);
	fprintf(stderr, "DEBUG: handle_executor_signal_exit, g_signal_flag: %d\n", g_signal_flag);
	if (g_signal_flag)
	{
		g_signal_flag = 0;

		if (!head)
			fprintf(stderr, "DEBUG: handle_executor_signal_exit, NO head\n");

		close_all_heredoc_fds(head);
		// free_minishell(&head->minishell);
		// free_cmd(&head);
		return (minishell->exit_status = 130);
	}
	close_all_heredoc_fds(cmd);
	return (exit_status);
}

uint8_t	run_executor_core(t_cmd *cmd)
{
	uint8_t	exit_status;

	if (!is_builtin(cmd) || cmd->next)
		exit_status = execute_pipeline_or_binary(cmd);
	else
		exit_status = execute_builtin(cmd);
	if (cmd->next == NULL)
		update_underscore(cmd, cmd->binary);
	close_all_heredoc_fds(cmd);
	return (exit_status);
}

uint8_t	run_executor(t_cmd *cmd)
{
	t_mshell	*minishell;
	uint8_t		exit_status;

	if (!cmd)
		return (error_return("run_executor: no cmd found\n", EXIT_FAILURE));
	minishell = cmd->minishell;
	if (!minishell || !minishell->env || !minishell->hash_table)
		return (error_return("run_executor: no mshell found\n", EXIT_FAILURE));
	if (command_too_long(cmd->argv))
		return (error_return("run_executor: command too long\n", 2));
	exit_status = apply_heredocs(cmd);

	fprintf(stderr, "DEBUG: apply_heredocs return exit_status: %d\n", exit_status);

	if (exit_status != EXIT_SUCCESS || g_signal_flag)
		return (handle_executor_signal_exit(cmd, minishell, exit_status));
	return (run_executor_core(cmd));
}
