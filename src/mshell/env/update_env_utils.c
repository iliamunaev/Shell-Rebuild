#include "minishell.h"

/**
 * @brief Creates a formatted environment entry string.
 *
 * Constructs a new string in the format `KEY=VALUE` using the provided
 * environment variable structure.
 *
 * @param var Pointer to the `t_mshell_var` structure containing key-value data.
 * @return A newly allocated string containing `KEY=VALUE`, or NULL on failure.
 */
char	*create_env_entry(t_mshell_var *var)
{
	size_t	total_len;
	char	*entry;
	char	*val;
	int		len_key;
	int		len_val;

	val = NULL;
	if (var->value)
		val = var->value;
	else
		val = "";
	len_key = ft_strlen(var->key);
	len_val = ft_strlen(val);
	total_len = len_key + len_val + 2;
	entry = malloc(total_len);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, var->key, total_len);
	ft_strlcat(entry, "=", total_len);
	ft_strlcat(entry, val, total_len);
	return (entry);
}

/**
 * @brief Frees the memory allocated for the old environment array.
 *
 * Iterates through the `env` array, freeing each entry, and then
 * frees the array itself.
 *
 * @param env The environment array to free.
 */
void	free_old_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}
