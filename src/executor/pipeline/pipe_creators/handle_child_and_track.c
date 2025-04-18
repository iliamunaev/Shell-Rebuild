#include "minishell.h"

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
				{
					if (close(redir->fd) == -1)
						perror("close_unused_heredocs_child: close");
				}
				node = node->next;
			}
		}
		cmd = cmd->next;
	}
}

static void	child_process(t_cmd *cmd, int in_fd, int *pipe_fd, t_cmd *cmd_list)
{
	close_unused_heredocs_child(cmd, cmd_list);
	if (cmd->next)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			perror_exit_child("-exec_in_pipes: dup2 pipe_fd[1]", EXIT_FAILURE);
	}
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			perror_exit_child("-exec_in_pipes: dup2 in_fd", EXIT_FAILURE);
	}
	if (apply_redirections(cmd) != EXIT_SUCCESS)
	{
		if (close_unused_fds(in_fd, pipe_fd) != EXIT_SUCCESS)
			_exit(EXIT_FAILURE);
		_exit(EXIT_FAILURE);
	}
	if (close_unused_fds(in_fd, pipe_fd) != EXIT_SUCCESS)
		_exit(EXIT_FAILURE);
	execute_command(cmd);
}

void	handle_child_and_track(t_cmd *cmd, t_pipe_info *info)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("-exec_in_pipes: fork");
		exit(EXIT_FAILURE);
	}
	info->pids[*info->idx] = pid;
	if (pid == 0)
		child_process(cmd, info->in_fd, info->pipe_fd, info->cmd_list);
	(*info->idx)++;
}
