/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:13:12 by imunaev-          #+#    #+#             */
/*   Updated: 2025/05/08 13:20:34 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Read user input from the terminal.
 *
 * Prompts the user with "minishell: " and reads input using readline.
 * If the input is non-empty, it is added to the readline history.
 *
 * @return Pointer to the input string, or NULL if EOF (Ctrl+D) is received.
 */
char	*read_user_input(void)
{
	char	*input;
	char	*line;

	if (isatty(fileno(stdin)))
	{
		input = readline("minishell: ");
		if (!input)
			return (NULL);
		if (*input)
			add_history(input);
	}
	else
	{
		line = get_next_line(fileno(stdin));
		if (!line)
			return (NULL);
		input = ft_strtrim(line, "\n");
		free(line);
		if (!input)
			return (NULL);
	}
	return (input);
}

/**
 * @brief Handle cleanup when the parser fails to produce a valid command.
 *
 * Frees the input string and updates the exit status if a signal was received.
 * Always returns true to indicate the loop should continue.
 *
 * @param mshell Pointer to the shell state.
 * @param input Pointer to the user's input string to be freed.
 * @return true to indicate continuation of the interactive loop.
 */
bool	handle_null_command(t_mshell *mshell, char **input_ptr)
{
	// char	*input;

	// input = *input_ptr;
	// if (input)
	// 	free(input);
	if (g_signal_flag)
	{
		mshell->exit_status = 130;
		g_signal_flag = 0;
	}
	*input_ptr = NULL;
	return (true);
}

/**
 * @brief Handle signal interruption after command parsing.
 *
 * If a signal (e.g., Ctrl+C) was received after parsing,
 * this function frees both the command and input, resets the signal flag,
 * and updates the shell's exit status.
 *
 * @param mshell Pointer to the shell state.
 * @param cmd Parsed command structure to be freed.
 * @param input Original user input to be freed.
 * @return true if a signal was handled, false otherwise.
 */
bool	handle_signal_after_parse(t_mshell *mshell, t_cmd **cmd_ptr,
			char **input_ptr)
{
	t_cmd	*cmd;
	char	*input;

	cmd = *cmd_ptr;
	input = *input_ptr;
	if (g_signal_flag)
	{
		mshell->exit_status = 130;
		g_signal_flag = 0;
		if (cmd)
		{
			free_cmd(&cmd);
			*cmd_ptr = NULL;
		}
		if (input)
		{
			free(input);
			*input_ptr = NULL;
		}
		return (true);
	}
	return (false);
}

/**
 * @brief Handle signal status after command execution.
 *
 * Checks and clears any global signal flag and updates
 * the shell's exit status accordingly.
 *
 * @param mshell Pointer to the shell state.
 */
void	handle_signal_after_exec(t_mshell *mshell)
{
	if (g_signal_flag)
	{
		mshell->exit_status = 130;
		g_signal_flag = 0;
	}
}

bool	is_input_interactive(void)
{
	return (isatty(STDIN_FILENO));
}
