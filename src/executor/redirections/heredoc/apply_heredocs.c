/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_heredocs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:58:28 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/05/07 11:31:22 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file apply_heredocs.c
 * @brief Handles heredoc creation and FD assignment in Minishell.
 */
#include "minishell.h"

static void	close_old_heredocs(t_cmd *cmd_list, int current_fd)
{
	t_list	*redir_list;
	t_redir	*redir;

	if (!cmd_list)
		return ;
	while (cmd_list)
	{
		redir_list = cmd_list->redirs;
		while (redir_list)
		{
			redir = redir_list->content;
			if (redir->type == R_HEREDOC
				&& redir->fd >= 0
				&& redir->fd != current_fd)
				safe_close(&redir->fd);
			redir_list = redir_list->next;
		}
		cmd_list = cmd_list->next;
	}
}
void exit_heredoc_interrupted(t_cmd *cmd)
{

    close_all_heredoc_fds(cmd);
    free_minishell(&cmd->minishell);
    free_cmd(&cmd);
    _exit(130); // 128 + SIGINT
}

/**
 * @brief Setup signal handlers inside heredoc child process.
 */
void setup_heredoc_signals(void)
{
    struct sigaction sa;

    ft_memset(&sa, 0, sizeof(sa));
    sa.sa_handler = heredoc_sigint_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL) == -1)
        perror("sigaction(SIGINT) error in heredoc child");

    signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Signal handler used in heredoc subprocess.
 */
void heredoc_sigint_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
	// g_signal_flag = 1;

	fprintf(stderr, "DEBUG: %d: IN heredoc_sigint_handler before\n", getpid());

    // return(130);
}

/**
 * @brief Apply signal-safe logic before and after fork for heredoc.
 */
int new_heredoc_fd(t_cmd *cmd, const char *delim, t_cmd *current, t_cmd *full_cmd_list)
{
    int pipe_fd[2];
    pid_t pid;
    int status;
    t_cmd *head;
    sigset_t block_mask, orig_mask;

    head = get_cmd_head(cmd);

    // Block SIGINT & SIGQUIT during pipe/fork setup
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &block_mask, &orig_mask);

    if (pipe(pipe_fd) == -1)
    {
        sigprocmask(SIG_SETMASK, &orig_mask, NULL);
        return (perror_return("new_heredoc_fd: pipe", WRITE_HERED_ERR));
    }

    pid = fork();
    if (pid == -1)
    {
        safe_close(&pipe_fd[0]);
        safe_close(&pipe_fd[1]);
        sigprocmask(SIG_SETMASK, &orig_mask, NULL);
        return (perror_return("new_heredoc_fd: fork", WRITE_HERED_ERR));
    }
	else if (pid == 0)
	{
		// ---------- CHILD ----------
		fprintf(stderr, "DEBUG: %d: new_heredoc_fd: HERE in child\n", getpid());

		sigprocmask(SIG_SETMASK, &orig_mask, NULL);
		g_signal_flag = 0; // Reset before starting input
		fprintf(stderr, "DEBUG: %d: new_heredoc_fd CHILD, before setup_heredoc_signals\n", getpid());

		setup_heredoc_signals();

		close_old_heredocs(full_cmd_list, pipe_fd[0]);
		safe_close(&pipe_fd[0]);
		close_all_heredoc_fds(current);

		int result = write_heredoc_to_pipe(cmd, pipe_fd[1], delim);
		fprintf(stderr, "DEBUG: %d: new_heredoc_fd CHILD, result == %d\n", getpid(), result);

		safe_close(&pipe_fd[1]);
		close_all_heredoc_fds(full_cmd_list);
		free_minishell(&cmd->minishell);
		free_cmd(&head);

		if (result == HEREDOC_INTERRUPTED)
		{
			fprintf(stderr, "DEBUG: %d: new_heredoc_fd exiting with 130 due to HEREDOC_INTERRUPTED\n", getpid());
			// free_minishell(&cmd->minishell);
			// free_cmd(&head);
			_exit(130); // SIGINT exit
		}
		else if (result == WRITE_HERED_ERR)
		{
			fprintf(stderr, "DEBUG: %d: new_heredoc_fd exiting with WRITE_HERED_ERR (%d)\n", getpid(), WRITE_HERED_ERR);
			_exit(WRITE_HERED_ERR); // write error
		}
		fprintf(stderr, "DEBUG: %d: new_heredoc_fd exiting with EXIT_SUCCESS (%d)\n", getpid(), WRITE_HERED_ERR);

		_exit(EXIT_SUCCESS);
	}

    // ---------- PARENT ----------
	fprintf(stderr, "DEBUG: %d: new_heredoc_fd: HERE in parent\n", getpid());
    sigprocmask(SIG_SETMASK, &orig_mask, NULL);
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);

    safe_close(&pipe_fd[1]);
    waitpid(pid, &status, 0);

    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);

    int heredoc_result = handle_heredoc_status(status);

	fprintf(stderr, "DEBUG: %d: new_heredoc_fd, in parent, status resived: %d\n", getpid(),status);


    if (heredoc_result == HEREDOC_INTERRUPTED)
    {
        g_signal_flag = 1;
		safe_close(&pipe_fd[1]);
        safe_close(&pipe_fd[0]);
        close_all_heredoc_fds(full_cmd_list);

		fprintf(stderr, "DEBUG: %d: new_heredoc_fd, PARENT heredoc_result == HEREDOC_INTERRUPTED\n", getpid());

        return (HEREDOC_INTERRUPTED);
    }
    else if (heredoc_result == WRITE_HERED_ERR)
    {
        safe_close(&pipe_fd[0]);
		safe_close(&pipe_fd[1]);
        close_all_heredoc_fds(full_cmd_list);
        return (WRITE_HERED_ERR);
    }

    return (pipe_fd[0]);
}



static bool	assign_heredoc_fd(t_cmd *cmd,
							t_redir *redirection,
							t_cmd *current,
							t_cmd *full_cmd_list)
{
	redirection->fd = new_heredoc_fd(cmd,
			redirection->filename,
			current,
			full_cmd_list);
	if (redirection->fd == WRITE_HERED_ERR)
		return (false);
	if (redirection->fd == HEREDOC_INTERRUPTED)
	{
		fprintf(stderr, "DEBUG: %d: assign_heredoc_fd, redirection->fd == HEREDOC_INTERRUPTED\n", getpid());

		g_signal_flag = 1;
		redirection->fd = -1;
		return (true);
	}
	return (true);
}

static bool	handle_cmd_heredocs(t_cmd *cmd, t_cmd *full_cmd_list)
{
	t_list	*redir_list;
	t_redir	*redirection;

	redir_list = cmd->redirs;
	while (redir_list)
	{
		redirection = redir_list->content;
		if (is_heredoc(redirection))
		{
			if (!assign_heredoc_fd(cmd, redirection, cmd, full_cmd_list))
				return (false);
		}
		redir_list = redir_list->next;
	}
	return (true);
}

uint8_t	apply_heredocs(t_cmd *cmd)
{
	fprintf(stderr, "DEBUG: %d: apply_heredocs, START\n", getpid());

	t_cmd	*initial_cmd_list;

	initial_cmd_list = cmd;
	if (!cmd)
		return (error_return("apply_heredocs: cmd not found\n", EXIT_FAILURE));
	while (cmd)
	{
		// fprintf(stderr, "DEBUG: process: %d, apply_heredocs, g_signal_flag BEFORE handle_cmd_heredocs: %d\n", getpid(), g_signal_flag);

		if (!handle_cmd_heredocs(cmd, initial_cmd_list))
		{
			fprintf(stderr, "DEBUG: %d: apply_heredocs, g_signal_flag RETURN: %d\n", getpid(), g_signal_flag);

			close_all_heredoc_fds(initial_cmd_list);

			fprintf(stderr, "DEBUG: %d: apply_heredocs, RETURN: %d\n", getpid(), EXIT_FAILURE);

			return (error_return("apply_heredocs: failed\n", EXIT_FAILURE));
		}

		if (g_signal_flag)
		{
			close_all_heredoc_fds(initial_cmd_list);
			fprintf(stderr, "DEBUG: %d: apply_heredocs, g_signal_flag RETURN: %d\n", getpid(), g_signal_flag);

			fprintf(stderr, "DEBUG: %d: apply_heredocs, RETURN: %d\n", getpid(), HEREDOC_INTERRUPTED);

			return (HEREDOC_INTERRUPTED);
		}
		cmd = cmd->next;
	}
	fprintf(stderr, "DEBUG: %d: apply_heredocs, RETURN: %d\n", getpid(), EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
