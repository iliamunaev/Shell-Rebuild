/**
 * @file handle_unset.c
 * @brief Functions for handling the `unset` built-in command in Minishell.
 */
#include "minishell.h"

void	process_var(t_mshell_var *hash_var)
{
	if (hash_var)
	{
		free(hash_var->key);
		free(hash_var->value);
		hash_var->key = NULL;
		hash_var->value = NULL;
		hash_var->val_assigned = 0;
	}
}


t_mshell_var	*find_arg(t_cmd *cmd)
{
    unsigned long	index;
	t_mshell_var	*current;
	char			*key;

	if (!cmd || !cmd->argv[1])
		return (NULL);

	key = cmd->argv[1];	
	index = hash_function(cmd->argv[1]);
    current = cmd->minishell->hash_table->buckets[index];
    while (current)
	{
        if (current->key && ft_strcmp(current->key, key) == 0)
            return (current);
        current = current->next;
    }
    return (NULL);
}


void	process_unset_arg(t_cmd *cmd)
{
	t_mshell_var	*unset_arg;
	if (!cmd || cmd->argv[0] || !cmd->argv[1])
		return ;
	unset_arg = find_arg(cmd);
	if (!unset_arg)
		return ;
	if (unset_arg->val_assigned == 1)
	{
		process_var(unset_arg);
		update_env(cmd->minishell);
	}
	else
		process_var(unset_arg);
}

uint8_t	handle_unset(t_cmd *cmd)
{
	uint8_t	status;
	int		i;

	status = EXIT_SUCCESS;
	if (!cmd || cmd->argv[0])
	{
		status = EXIT_FAILURE;
		cmd->minishell->exit_status = status;
		if (cmd->in_pipe)
			exit(status);
		return (status);
	}
	cmd->minishell->exit_status = status;
	if (!cmd->argv[1])
	{
		if (cmd->in_pipe)
			exit(status);
		return (status);
	}
	i = 1;
	while (cmd->argv[i])
	{
		process_unset_arg(cmd);
		i++;
	}
	printf("Exit status: %d\n",cmd->minishell->exit_status);
	if (cmd->in_pipe)
		exit(status);
	return (status);
}

/*

void remove_var_from_ht(t_mshell *mshell, const char *key)
{
    unsigned long index;
    t_mshell_var *prev = NULL;
    t_mshell_var *current;

    if (!mshell || !key)
        return;

    index = hash_function(key);
    current = mshell->hash_table->buckets[index];

    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            // Unlink from list
            if (prev == NULL)
                mshell->hash_table->buckets[index] = current->next;
            else
                prev->next = current->next;

            // Free memory
            free(current->key);
            free(current->value);
            free(current);

            update_env(mshell);  // Sync env list
            return;
        }
        prev = current;
        current = current->next;
    }
}

void process_unset_arg(t_cmd *cmd)
{
    if (!cmd || !cmd->argv[1]) // Ensure valid argument
        return;

    remove_var_from_ht(cmd->minishell, cmd->argv[1]);
}

uint8_t handle_unset(t_cmd *cmd)
{
    uint8_t status = EXIT_SUCCESS;
    int i = 1;

    if (!cmd || !cmd->argv[1])  // No arguments
    {
        status = EXIT_FAILURE;
        cmd->minishell->exit_status = status;
        if (cmd->in_pipe)
            exit(status);
        return status;
    }

    // Unset each variable in argv[]
    while (cmd->argv[i])
    {
        process_unset_arg(cmd);
        i++;
    }

    cmd->minishell->exit_status = status;
    if (cmd->in_pipe)
        exit(status);

    return status;
}

*/