/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 16:46:44 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/17 11:50:06 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>

//Definition of AT_* constants
# include <fcntl.h>

// Maximum number of arguments per command.
# define MAX_ARGS 64

// Maximum number of commands in a pipeline.
# define MAX_CMDS 10

// Delimiter used for tokenizing input.
# define DELIM " "

/**
 * @enum e_redir_type
 * @brief Enumeration for different types of I/O redirection.
 *
 * - `R_INPUT`:		`<` Redirects standard input from a file.
 * - `R_OUTPUT`:	`>` Redirects standard output to a file.
 * - `R_APPEND`:	`>>` Redirects standard output in append mode.
 * - `R_HEREDOC`:	`<<` Reads input until a specified delimiter is found.
 */
typedef enum e_redir_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPEND,
	R_HEREDOC
}	t_redir_type;

/**
 * @struct s_redir
 * @brief Structure for handling I/O redirections.
 *
 * - `type`:		The type of redirection.
 * - `filename`:	For `R_INPUT`, `R_OUTPUT`, or `R_APPEND`,
 * 					this stores the filename.
 *					For `R_HEREDOC`, it stores the delimiter string.
 */
typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
}	t_redir;

/**
 * @struct s_command
 * @brief Represents a single command in a pipeline.
 *
 * - `argv`:		NULL-terminated array of command arguments.
 * - `binary`:		Binary from PATH env for execve().
 * - `in_redir`:	Pointer to input redirection struct, if any.
 * - `out_redir`:	Pointer to output redirection struct, if any.
 * - `next`:		Pointer to the next command in the pipeline.
 *
 * Example pipeline: `ls -l | grep minishell | wc -l`
 * - cmd1:	`ls -l`
 * - cmd2:	`grep minishell`
 * - cmd3:	`wc -l`
 * - cmd4:	`NULL` (end of pipeline)
 */
typedef struct s_command
{
	char				**argv;
	char				*binary;
	t_redir				*in_redir;
	t_redir				*out_redir;
	struct s_command	*next;
}	t_command;

// utils
void	print_error(char *cmd, const char *msg);

#endif /* MINISHELL_H */
