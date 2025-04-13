#include "minishell.h"

uint8_t	write_heredoc_to_pipe(int pipe_fd, const char *delimiter)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, (char *)delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (write(pipe_fd, line, ft_strlen(line)) == -1 || write(pipe_fd, "\n", 1) == -1)
		{
			perror("-write_heredoc_to_pipe: write");
			free(line);
			return (EXIT_FAILURE);
		}
		free(line);
	}
	return (EXIT_SUCCESS);
}

static uint8_t	new_heredoc_fd(const char *delim)
{
	int pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("-new_heredoc_fd: pipe");
		return (-1);
	}
	if (write_heredoc_to_pipe(pipe_fd[1], delim) != EXIT_SUCCESS)
	{
		perror("-new_heredoc_fd: write");
		return (-1);
	}
	if (close(pipe_fd[1]) == -1)
	{
		perror("-new_heredoc_fd: close");
		return (-1);
	}
	return (pipe_fd[0]);
}

static bool	is_heredoc(t_redir *redirection)
{
	return (redirection->type == R_HEREDOC);
}
uint8_t apply_heredocs(t_cmd *cmd)
{
	t_cmd *start = cmd;

	while (cmd)
	{
		t_list *redir_list = cmd->redirs;

		while (redir_list)
		{
			t_redir *redirection = redir_list->content;

			if (is_heredoc(redirection))
			{
				redirection->fd = new_heredoc_fd(redirection->filename);
				if (redirection->fd == -1)
				{
					// printf("DEBUG: apply_heredocs: redirection->fd == '%d'\n", redirection->fd);
					close_all_heredoc_fds(start);
					return (EXIT_FAILURE);
				}
			}
			redir_list = redir_list->next;
		}
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

