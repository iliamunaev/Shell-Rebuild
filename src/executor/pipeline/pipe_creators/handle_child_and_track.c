/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_child_and_track.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:46:41 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/05/06 23:06:51 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file handle_child_and_track.c
 * @brief Handles child process creation and pipe setup for piped
 * commands in Minishell.
 *
 * This file contains logic for:
 * - Forking a child process in a pipeline.
 * - Duplicating pipe file descriptors (`dup2`).
 * - Applying redirections and executing commands.
 * - Closing unused heredoc file descriptors to avoid leaks.
 */
#include "minishell.h"

/**
 * @brief Closes all heredoc file descriptors in the child process,
 *        except for the current command's.
 *
 * Prevents the child process from inheriting unused heredoc pipes,
 * which could result in hanging reads or FD leaks.
 *
 * @param current The command currently being executed in the child.
 * @param full_cmd_list The full list of commands in the current pipeline.
 */
void	close_unused_heredocs_child(t_cmd *current, t_cmd *full_cmd_list)
{
	t_cmd	*cmd;
	t_redir	*redir;
	t_list	*node;

	cmd = full_cmd_list;
	while (cmd)
	{
		if (cmd != current)
		{
			node = cmd->redirs;
			while (node)
			{
				redir = (t_redir *)node->content;
				if (redir->type == R_HEREDOC && redir->fd >= 0)
					safe_close(&redir->fd);
				node = node->next;
			}
		}
		cmd = cmd->next;
	}
}

static bool	setup_child_io(t_cmd *cmd, int in_fd, int *pipe_fd, t_cmd *cmd_list)
{
	if (cmd->next)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			close_all_heredoc_fds(cmd_list);
			perror_exit_child(cmd, "-child: dup2 pipe_fd[1]", EXIT_FAILURE);
			return (false);
		}
	}
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror_exit_child(cmd, "-child: dup2 in_fd", EXIT_FAILURE);
			return (false);
		}
	}
	return (true);
}

static void	child_process(t_cmd *cmd, int in_fd, int *pipe_fd, t_cmd *cmd_list)
{
	t_cmd	*head;

	head = get_cmd_head(cmd);
	close_unused_heredocs_child(cmd, cmd_list);
	if (!setup_child_io(cmd, in_fd, pipe_fd, cmd_list))
	{
		free_cmd(&head);
		free_minishell(&cmd->minishell);
		_exit(EXIT_FAILURE);
	}
	if (apply_redirections(cmd) != EXIT_SUCCESS)
	{
		close_unused_fds(in_fd, pipe_fd);
		free_minishell(&cmd->minishell);
		free_cmd(&head);
		_exit(EXIT_FAILURE);
	}
	if (close_unused_fds(in_fd, pipe_fd) != EXIT_SUCCESS)
	{
		free_minishell(&cmd->minishell);
		free_cmd(&head);
		_exit(EXIT_FAILURE);
	}
	execute_command(cmd);
}

/**
 * @brief Forks a child process to execute a command in a pipeline.
 *
 * - Stores the PID in the pipeline info.
 * - If in the child process, runs `child_process()`.
 * - Increments the PID index counter.
 *
 * @param cmd The command to be executed in the child.
 * @param info Pointer to the pipeline information structure.
 */
void	handle_child_and_track(t_cmd *cmd, t_pipe_info *info)
{
	pid_t	pid;
	t_cmd	*head;

	head = get_cmd_head(cmd);
	pid = fork();
	if (pid == -1)
	{
		perror("-exec_in_pipes: fork");
		free_minishell(&cmd->minishell);
		free_cmd(&head);
		exit(EXIT_FAILURE);
	}
	info->pids[*info->idx] = pid;
	if (pid == 0)
	{
		signal(SIGINT,  SIG_DFL);   // Ctrl+C kills child
		signal(SIGQUIT, SIG_DFL);   // Ctrl+\ kills child
		signal(SIGPIPE, SIG_DFL);   // Broken pipe → silent death
		child_process(cmd, info->in_fd, info->pipe_fd, info->cmd_list);
		free_cmd(&head);
	}
	(*info->idx)++;
}
