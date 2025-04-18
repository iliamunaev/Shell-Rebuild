#include "minishell.h"

static void	handle_builtin_and_exit(t_cmd *cmd)
{
	uint8_t	exit_status;

	exit_status = exec_builtins(cmd);
	free_minishell(cmd->minishell);
	_exit(exit_status);
}

void	exec_cmd(t_cmd *cmd)
{
	uint8_t	exit_status;

	exit_status = validate_dots(cmd);
	if (exit_status != EXIT_SUCCESS)
		_exit(exit_status);
	execve(cmd->binary, cmd->argv, cmd->minishell->env);
	child_execve_error(cmd);
}

void	execute_command(t_cmd *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		_exit(EXIT_SUCCESS);
	}
	if (cmd->minishell->syntax_exit_status != 0)
		_exit(cmd->minishell->syntax_exit_status);
	if (is_minishell_executable(cmd) && update_shlvl(cmd) == EXIT_FAILURE)
		_exit(EXIT_FAILURE);
	if (ft_strcmp(cmd->argv[0], "") == 0)
		handle_empty_command(cmd);
	if (!cmd->binary)
	{
		if (is_builtin(cmd))
		{
			if (ft_strcmp(cmd->argv[0], "env") == 0)
				update_underscore(cmd, cmd->binary);
			handle_builtin_and_exit(cmd);
		}
		else
			cmd_missing_command_error(cmd);
	}
	exec_cmd(cmd);
}
