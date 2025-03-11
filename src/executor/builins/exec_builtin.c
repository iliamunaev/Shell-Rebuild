#include "minishell.h"

uint8_t	exec_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (handle_exit(cmd));
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (handle_echo(cmd));
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (handle_pwd(cmd));
	else if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (handle_cd(cmd));
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (handle_env(cmd));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (handle_export(cmd));
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (handle_unset(cmd));
	else
		return (EXIT_FAILURE);
	return (EXIT_FAILURE);
}
