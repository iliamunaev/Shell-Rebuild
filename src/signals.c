/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Pavel Vershinin <pvershin@student.hive.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:13:59 by pvershin          #+#    #+#             */
/*   Updated: 2025/05/15 15:44:18 by Pavel Versh      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_flag = 0;

extern void	rl_replace_line(const char *a, int b);

void	disable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == 0)
	{
		term.c_lflag &= ~ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &term);
	}
}

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_flag = 1;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}

// SIGQUIT (Ctrl-\) Handler - does nothing
void	handle_sigquit(int sig)
{
	(void)sig;
}

// Function to setup signal handling
void	setup_signal_handlers(void)
{
	disable_echoctl();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
}
