#include "minishell.h"

static uint8_t	validate_dot(t_cmd *cmd)
{
	if (ft_strcmp(cmd->argv[0], ".") == 0)
	{
		if (!cmd->argv[1])
		{
			print_error("-minishell: .: filename argument required\n");
			print_error(".: usage: . filename [arguments]\n");
			cmd->minishell->exit_status = 2;
			return (2);
		}
	}
	return (EXIT_SUCCESS);
}
/**
 * @brief Executes a command, either a built-in or an external binary.
 *
 * If the command is a built-in, it calls `exec_builtin()`. Otherwise, it
 * attempts to execute the binary using `execve()`. If execution fails,
 * an appropriate error message is printed, and the process exits.
 *
 * @param cmd Pointer to the command structure.
 */
static void	execute_command(t_cmd *cmd)
{
	if (cmd->binary == NULL)
	{
		if (is_builtin(cmd))
		_exit(exec_builtin(cmd));
		else
			cmd_missing_command_error(cmd);
	}
	if (validate_dot(cmd) == 2)
		_exit(2);

	execve(cmd->binary, cmd->argv, cmd->minishell->env);
	// fprintf(stderr, "errno = %d (%s)\n", errno, strerror(errno)); // test
	child_execve_error(cmd);
}
static int	handle_dup_and_close(int in_fd, int *fds, t_cmd *cmd)
{
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			return (-1);
		close(in_fd);
	}
	if (cmd->next && dup2(fds[1], STDOUT_FILENO) == -1)
		return (-1);
	if (fds[0] != -1)
		close(fds[0]);
	if (fds[1] != -1)
		close(fds[1]);
	if (apply_redirections(cmd) != EXIT_SUCCESS)
		return (-1);
	return (EXIT_SUCCESS);
}

static int	handle_child(t_cmd *cmd, int in_fd, int *fds)
{
	if (handle_dup_and_close(in_fd, fds, cmd) == -1)
		_exit(EXIT_FAILURE);
	execute_command(cmd);
	return (EXIT_SUCCESS);
}

static int	wait_for_children(pid_t *pids, int count, uint8_t *exit_status)
{
	int i = 0;
	while (i < count)
	{
		int status;
		if (waitpid(pids[i], &status, 0) != -1 && i == count - 1)
		{
			if (WIFEXITED(status))
				*exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*exit_status = 128 + WTERMSIG(status);
			else
				*exit_status = EXIT_FAILURE;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
static int	create_pipe_if_needed(t_cmd *cmd, int *fds)
{
	fds[0] = -1;
	fds[1] = -1;
	if (cmd->next && pipe(fds) == -1)
	{
		perror("-minishell: pipe");
		return (-1);
	}
	return (0);
}

static pid_t	fork_command(t_cmd *cmd, int in_fd, int *fds)
{
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("-minishell: fork");
		return (-1);
	}
	else if (pid == 0)
	{
		handle_child(cmd, in_fd, fds);
	}
	return (pid);
}

static void	cleanup_parent_fds(int *in_fd, int *fds)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (fds[1] != -1)
		close(fds[1]);
	*in_fd = fds[0];
}

static bool	check_pipeline_limit(int cmd_count)
{
	if (cmd_count >= MAX_CMDS)
	{
		print_error("-minishell: too many commands in pipeline\n");
		return (true);
	}
	return (false);
}

uint8_t	exec_in_child_process(t_cmd *cmd)
{
	uint8_t		exit_status = EXIT_FAILURE;
	int			in_fd = STDIN_FILENO;
	int			fds[2];
	int			cmd_count = 0;
	pid_t		pids[MAX_CMDS];

	while (cmd)
	{
		if (check_pipeline_limit(cmd_count))
			return (EXIT_FAILURE);

		if (create_pipe_if_needed(cmd, fds) == -1)
			return (EXIT_FAILURE);

		pids[cmd_count] = fork_command(cmd, in_fd, fds);
		if (pids[cmd_count] == -1)
			return (EXIT_FAILURE);

		cleanup_parent_fds(&in_fd, fds);

		cmd = cmd->next;
		cmd_count++;
	}
	wait_for_children(pids, cmd_count, &exit_status);
	return (exit_status);
}
