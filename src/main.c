/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 08:10:49 by pvershin          #+#    #+#             */
/*   Updated: 2025/04/29 12:24:40 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

uint8_t	run_command_mode(t_mshell *mshell, char *input)
{
	t_cmd	*cmd;
	uint8_t	exit_status;

	cmd = run_parser(mshell, input);
	if (!cmd)
		return (EXIT_FAILURE);
	exit_status = run_executor(cmd);
	free_cmd(cmd);
	cmd = NULL;
	return (exit_status);
}

uint8_t	run_script_mode(t_mshell *mshell, const char *file)
{
	char	*input;
	uint8_t	exit_status;
	t_cmd	*cmd;
	int		in_fd;

	in_fd = open(file, O_RDONLY);
	if (in_fd < 0)
	{
		print_error("minishell: cannot open script file\n");
		return (EXIT_FAILURE);
	}
	input = NULL;
	while ((input = get_next_line(in_fd)) != NULL)
	{
		if (g_signal_flag)
		{
			mshell->exit_status = 130;
			g_signal_flag = 0;
		}
		cmd = run_parser(mshell, input);
		if (!cmd)
		{
			free(input);
			input = NULL;
			continue ;
		}
		exit_status = run_executor(cmd);
		free_cmd(cmd);
		cmd = NULL;
		free(input);
		input = NULL;
	}
	safe_close(&in_fd);
	return (exit_status);
}

// uint8_t	run_interactive_mode(t_mshell *mshell)
// {
// 	char	*input;
// 	uint8_t	exit_status;
// 	t_cmd	*cmd;
// 	char	**lines;
// 	int		i;

// 	lines = NULL;
// 	while (1)
// 	{
// 		input = readline("minishell: ");
// 		if (!input) // EOF (Ctrl+D)
// 			return (EXIT_FAILURE);
// 		if (*input)
// 			add_history(input);
// 		lines = ft_split(input, '\n');
// 		if (!lines)
// 		{
// 			free(input);
// 			continue ;
// 		}

// 		i = 0;
// 		while (lines && lines[i])
// 		{
// 			// Ignore empty lines
// 			if (lines[i][0] == '\0')
// 			{
// 				i++;
// 				continue ;
// 			}
// 			cmd = run_parser(mshell, lines[i]);
// 			if (cmd)
// 			{
// 				exit_status = run_executor(cmd);
// 				free_cmd(cmd);
// 			}
// 			i++;
// 		}
// 		ft_free_arrstrs(lines);
// 		free(input);
// 		input = NULL;
// 	}
// 	return (exit_status);
// }


uint8_t	run_interactive_mode(t_mshell *mshell)
{
	char	*input;
	uint8_t	exit_status;
	t_cmd	*cmd;

	while (1)
	{
		input = readline("minishell: ");
		if (!input)
			return (EXIT_FAILURE);
		if (*input)
			add_history(input);
		cmd = run_parser(mshell, input);
		if (!cmd)
		{
			free(input);
			if (g_signal_flag)
			{
				mshell->exit_status = 130;
				g_signal_flag = 0;
			}
			continue ;
		}
		if (g_signal_flag)
		{
			mshell->exit_status = 130;
			g_signal_flag = 0;
			free_cmd(cmd);
			free(input);
			continue;
		}
		exit_status = run_executor(cmd);
		free_cmd(cmd);
		free(input);
		input = NULL;
		if (g_signal_flag)
		{
			mshell->exit_status = 130;
			g_signal_flag = 0;
		}
	}
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_mshell	*minishell;
	uint8_t		exit_status;
#ifdef BIGTEST
	char		*line;
	char		*trimmed;
#endif
	setup_signal_handlers(); // Set up signal handlers
	minishell = init_mshell(envp);
	if (!minishell)
		return (EXIT_FAILURE);
	// Handle -c option
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
		exit_status = run_command_mode(minishell, argv[2]);
	// Handle script mode
	else if (argc == 2)
		exit_status = run_script_mode(minishell, argv[1]);
	// Handle interactive mode
// Handle interactive mode
#ifdef BIGTEST
	else if (isatty(fileno(stdin)))
		exit_status = run_interactive_mode(minishell);
	else
	{
		line = get_next_line(fileno(stdin));
		if (!line)
			exit_status = EXIT_FAILURE;
		else
		{
			trimmed = ft_strtrim(line, "\n");
			free(line);
			exit_status = run_command_mode(minishell, trimmed);
			free(trimmed);
		}
	}
#else
	else
		exit_status = run_interactive_mode(minishell);
#endif
	free_minishell(minishell);
	rl_clear_history();
	return (exit_status);
}
