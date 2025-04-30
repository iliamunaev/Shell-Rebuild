/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:45:04 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/04/30 18:38:56 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file env.c
 * @brief Implementation of the `env` built-in command for Minishell.
 *
 * This file provides the logic for handling the `env` command, which
 * displays all exported environment variables currently stored by Minishell.
 * It mimics the behavior of the POSIX `env` command.
 */
#include "minishell.h"

/**
 * @brief Prints all exported environment variables.
 *
 * Iterates through the shell's internal environment array and prints each
 * variable to standard output. Only variables that are actually present in
 * the internal `env` list are printed—unset or hidden variables are not shown.
 *
 * @param cmd Pointer to the command structure, which contains
 * the shell environment.
 */
void	print_env(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (cmd->minishell->env[i])
	{
		(void)printf("%s\n", cmd->minishell->env[i]);
		i++;
	}
}

/**
 * @brief Handles the `env` built-in command.
 *
 * Prints all environment variables currently stored in the shell's internal
 * environment. If any arguments are passed to the `env` command, it prints
 * an error message and returns with exit status `127`, mimicking Bash behavior.
 *
 * **Behavior:**
 * - No arguments: prints all exported variables.
 * - Arguments: prints an error and returns `127`.
 *
 * @param cmd Pointer to the command structure containing the shell state
 * and arguments.
 * @return `EXIT_SUCCESS` (0) on success,
 *         `127` if called with arguments (invalid usage).
 */
// uint8_t	handle_env(t_cmd *cmd)
// {
// 	uint8_t	exit_status;

// 	if (!cmd)
// 		return (no_cmd_error("env"));
// 	if (cmd->argv[1])
// 	{
// 		print_error("env: ‘");
// 		print_error(cmd->argv[1]);
// 		print_error("‘: No such file or directory\n");
// 		exit_status = 127;
// 		return (exit_status);
// 	}
// 	print_env(cmd);
// 	exit_status = EXIT_SUCCESS;
// 	return (exit_status);
// }
uint8_t	handle_env(t_cmd *cmd)
{
	char		buffer[PRINT_BUF_SIZE];
	size_t		i;
	char		*arg;
	const char	*prefix;
	const char	*suffix;

	i = 0;
	if (!cmd)
		return (no_cmd_error("env"));
	if (cmd->argv[1])
	{
		prefix = "env: ‘";
		suffix = "’: No such file or directory\n";
		arg = cmd->argv[1];
		while (*prefix && i < sizeof(buffer) - 1)
			buffer[i++] = *prefix++;
		while (*arg && i < sizeof(buffer) - 1)
			buffer[i++] = *arg++;
		while (*suffix && i < sizeof(buffer) - 1)
			buffer[i++] = *suffix++;
		buffer[i] = '\0';
		write(STDERR_FILENO, buffer, i);
		return (127);
	}
	print_env(cmd);
	return (EXIT_SUCCESS);
}
