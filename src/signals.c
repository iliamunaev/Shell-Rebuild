/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:13:59 by pvershin          #+#    #+#             */
/*   Updated: 2025/04/30 17:28:08 by Ilia Munaev      ###   ########.fr       */
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

/* SIGINT (Ctrl-C) Handler
// Tell readline that we are on a new line
rl_on_new_line();
// Clear the current input buffer
rl_replace_line("", 0);
// Redisplay the prompt without printing ^C
rl_redisplay();*/
/*void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}*/

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_flag = 1;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// SIGQUIT (Ctrl-\) Handler - does nothing
void	handle_sigquit(int sig)
{
	(void)sig;
}

// Handler for SIGSEGV (Segmentation fault)
static void	handle_sigsegv(int sig)
{
	(void)sig;
	_exit(139); // Conventionally 128 + signal number
}

// Function to setup signal handling
// void	setup_signal_handlers(void)
// {
// 	struct sigaction	sa;

// 	disable_echoctl();
// 	sa.sa_handler = handle_sigint;
// 	sa.sa_flags = SA_RESTART;
// 	sigemptyset(&sa.sa_mask);
// 	sigaction(SIGINT, &sa, NULL);
// 	signal(SIGQUIT, SIG_IGN);
// }
void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	disable_echoctl();

	// SIGINT handler
	sa.sa_handler = handle_sigint;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);

	// SIGSEGV handler
	sa.sa_handler = handle_sigsegv;
	sa.sa_flags = SA_RESETHAND; // Optional: resets handler after 1st call
	sigemptyset(&sa.sa_mask);
	sigaction(SIGSEGV, &sa, NULL);

	// Ignore SIGQUIT
	signal(SIGQUIT, SIG_IGN);
}
