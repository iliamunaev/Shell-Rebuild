/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:10:49 by pvershin          #+#    #+#             */
/*   Updated: 2025/04/29 19:02:51 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Runs the interactive mode of Minishell.
 *
 * This is the main loop of the shell where user input is continuously read,
 * parsed, and executed. It performs the following steps:
 * - Reads input using readline
 * - Adds input to history (if not empty)
 * - Parses the input into a command structure
 * - Handles signals triggered by the user (like Ctrl+C)
 * - Executes the parsed command(s)
 * - Frees allocated memory each iteration
 *
 * It handles graceful exits on EOF (Ctrl+D) and manages global signal state.
 *
 * @param mshell A pointer to the Minishell state structure.
 * @return The exit status of the last executed command.
 */
uint8_t	run_interactive_mode(t_mshell *mshell)
{
	char	*input;
	t_cmd	*cmd;
	uint8_t	exit_status;

	while (1)
	{
		// printf("\n-------------DEBUG: main() pid=%d\n", getpid());

		input = read_user_input();
		if (!input)
			return (EXIT_FAILURE);
		cmd = run_parser(mshell, input);
		if (!cmd)
		{
			if (handle_null_command(mshell, input))
				continue ;
		}
		if (handle_signal_after_parse(mshell, cmd, input))
			continue ;
		exit_status = run_executor(cmd);
		free_cmd(cmd);
		free(input);
		handle_signal_after_exec(mshell);
	}
	return (exit_status);
}

/**
 * @brief Program entry point for Minishell.
 *
 * Initializes shell state and enters interactive mode.
 * - Ignores command-line arguments (Minishell is interactive only)
 * - Sets up signal handlers
 * - Initializes shell memory, environment, and data structures
 * - Launches the main interactive shell loop
 * - Cleans up resources and clears history on exit
 *
 * @param argc Argument count (unused)
 * @param argv Argument values (unused)
 * @param envp Environment variables used to initialize shell state
 * @return Exit status returned from the last command executed
 */
int	main(int argc, char **argv, char **envp)
{
	t_mshell	*minishell;
	uint8_t		exit_status;

	if (argc > 1)
	{
		print_error("-minishell: Usage without arguments\n");
		return (EXIT_FAILURE);
	}
	(void)argv;
	setup_signal_handlers();
	minishell = init_mshell(envp);
	if (!minishell)
		return (EXIT_FAILURE);
	exit_status = run_interactive_mode(minishell);
	free_minishell(minishell);
	rl_clear_history();
	return (exit_status);
}
