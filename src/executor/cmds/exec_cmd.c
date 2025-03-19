#include "minishell.h"

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
	// if (cmd->in_redir && cmd->in_redir->type == R_HEREDOC)
	// {
	// 	handle_heredoc(cmd);  // This will set up the heredoc
	// }
	if (cmd->binary == NULL)
	{
		if (is_builtin(cmd))
			_exit(exec_builtin(cmd));
		else
		{
			command_not_found_handle(cmd);
			_exit(127);
		}
	}
	execve(cmd->binary, cmd->argv, cmd->minishell->env);
	fatal_error_child(cmd, errno);
}

/**
 * @brief Handles the execution of a command in a child process.
 *
 * This function sets up input/output redirections and pipes as needed,
 * then executes the command using `execute_command()`. The child process
 * terminates on failure.
 *
 * @param cmd Pointer to the command structure.
 * @param in_fd File descriptor for input redirection
 *              (e.g., from a previous pipe).
 * @param fds Pipe file descriptors [read end, write end].
 */
static void	child_process(t_cmd *cmd, int in_fd, int fds[2])
{
    // if (cmd->in_redir && cmd->in_redir->type == R_HEREDOC) {
    //     handle_heredoc(cmd);
    // }
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			fatal_error_child(cmd, EXIT_FAILURE);
		if (close(in_fd) == -1)
			fatal_error_child(cmd, EXIT_FAILURE);
	}
	if (cmd->next)
	{
		if (dup2(fds[1], STDOUT_FILENO) == -1)
			fatal_error_child(cmd, EXIT_FAILURE);
	}
	if (fds[0] >= 0)
		if (close(fds[0]) == -1)
			fatal_error_child(cmd, EXIT_FAILURE);
	if (fds[1] >= 0)
		if (close(fds[1]) == -1)
			fatal_error_child(cmd, EXIT_FAILURE);
	if (cmd->in_redir || cmd->out_redir)
		handle_redirections(cmd, STDIN_FILENO);
	execute_command(cmd);
}

/**
 * @brief Handles file descriptor cleanup and waits for the child process.
 *
 * The parent process manages pipes, closes unnecessary file descriptors,
 * and waits for the child process to finish. It also updates the shell's
 * exit status based on the child's termination status.
 *
 * @param cmd Pointer to the command structure.
 * @param in_fd File descriptor for input redirection.
 * @param fds Pipe file descriptors [read end, write end].
 * @param pid Process ID of the forked child process.
 * @return The exit status of the executed command.
 */
static uint8_t	parent_process(t_cmd *cmd, int in_fd, int fds[2], pid_t pid)
{
	int		status;
	uint8_t	exit_status;

	if (cmd->next)
		close(fds[1]);
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (!cmd->next && fds[0] >= 0)
		close(fds[0]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_status = 128 + WTERMSIG(status);
	else
		exit_status = EXIT_FAILURE;
	return (exit_status);
}


/**
 * @brief Forks a new process and executes the command.
 *
 * This function creates a child process using `fork()`. If the fork is
 * successful, the child process executes `child_process()`, while the parent
 * process continues execution.
 *
 * @param cmd Pointer to the command structure.
 * @param in_fd File descriptor for input redirection.
 * @param fds Pipe file descriptors [read end, write end].
 * @return Process ID of the child process.
 */
static pid_t	fork_and_execute(const t_cmd *cmd, int in_fd, int fds[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		fatal_error("fork", EXIT_FAILURE);
	if (pid == 0)
		child_process((t_cmd *)cmd, in_fd, fds);
	return (pid);
}

/**
 * @brief Executes a command or a pipeline of commands.
 *
 * This function handles execution of built-in and external commands,
 * including pipes and input/output redirections.
 *
 * @param cmd Pointer to the command structure.
 * @return The exit status of the last executed command.
 */
uint8_t	exec_cmd(t_cmd *cmd)
{
	uint8_t	exit_status;
	int		in_fd;
	int		fds[2];
	int		cmd_count;
	pid_t	pid;

	exit_status = EXIT_FAILURE;
	in_fd = STDIN_FILENO;
	cmd_count = 0;
	while (cmd)
	{
		if (is_pipeline_limit(&cmd_count))
			return (exit_status);
		fds[0] = -1;
		fds[1] = -1;
		if (cmd->next && pipe(fds) == -1)
		{
			perror("pipe");
			return (exit_status);
		}
		pid = fork_and_execute(cmd, in_fd, fds);
		if (pid > 0)
			exit_status = parent_process(cmd, in_fd, fds, pid);
		if (cmd->next)
			in_fd = fds[0];
		else
			in_fd = STDIN_FILENO;
		cmd = cmd->next;
	}
	return (exit_status);
}
