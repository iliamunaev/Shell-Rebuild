/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_heredoc_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:00:09 by pvershin          #+#    #+#             */
/*   Updated: 2025/05/07 03:05:19 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



/**
 * @brief Setup signal handlers inside heredoc child process.
 */


/**
 * @brief Child: Reads user input and writes heredoc content to pipe.
 */
int	run_heredoc_child(int pipe_fd, const char *delim, t_mshell *mshell, int expand_flag)
{
	char	*line;
	char	*expanded_line;
	size_t	total_written;
	int     write_status;

	setup_heredoc_signals();
	disable_echoctl();
	line = NULL;
	total_written = 0;
	write_status = EXIT_SUCCESS;
	while (1)
	{
		fprintf(stderr, "DEBUG: run_heredoc_child, start\n");

		int status = read_next_heredoc_line(&line, delim);

		fprintf(stderr, "DEBUG: run_heredoc_child, read_next_heredoc_line status == %d\n", status);


		if (status == EXIT_SUCCESS)
			break ;
		if (status == HEREDOC_INTERRUPTED)
		{
			free(line);
			return (HEREDOC_INTERRUPTED);
		}


		total_written += ft_strlen(line) + 1;
		if (heredoc_exceeds_limit(total_written))
		{
			free(line);
			return (error_return("heredoc: large input\n", WRITE_HERED_ERR));
		}
		if (expand_flag)
        {
            expanded_line = expand_env_variables(line, mshell, 0);
            if (!expanded_line)
			{
                perror_return("heredoc expansion failed", WRITE_HERED_ERR);
                expanded_line = line;
            }
			else
                free(line);
            line = NULL;

            write_status = write_heredoc_line(pipe_fd, expanded_line);
            free(expanded_line);
            if (write_status == WRITE_HERED_ERR)
                 return (WRITE_HERED_ERR);
        }
        else
        {
            write_status = write_heredoc_line(pipe_fd, line);
            free(line);
            line = NULL;
            if (write_status == WRITE_HERED_ERR)
                return (WRITE_HERED_ERR);
        }
	}
	if (line)
		free(line);
	return (EXIT_SUCCESS);
}


