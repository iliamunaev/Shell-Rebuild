/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:56:20 by Ilia Munaev       #+#    #+#             */
/*   Updated: 2025/05/08 13:33:56 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <stdbool.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <string.h>
# include <signal.h>
# include <sys/types.h>
# include <errno.h>
# include <stdint.h>
# include <limits.h>
# include <sys/stat.h>
# include <string.h>
# include "libft.h"
# include "executor.h"
# include "tokenizer.h"
# include "parser.h"
# include "signals.h"
# include "command.h"

// limits
# define ERROR_BUF_SIZE 256
# define MS_PATHMAX 4096
# define HASH_SIZE 128
# define HEREDOC_MAX_SIZE 1048576
# define CMD_MAX_SIZE 1048576

// errors
# define WRITE_HERED_ERR -2
# define REDIR_ERR -3
# define HEREDOC_INTERRUPTED -4
# define ERROR_UNEXPECTED_TOKEN -5
/**
 * @struct	s_builtin_dispatch
 * @brief	Maps a builtin command name to its handler function.
 *
 * - `name`:	Name of the builtin command (e.g., "cd", "export", "exit").
 * - `func`:	Pointer to the function that implements the builtin command.
 *				The function returns a status code (0 = success, 1+ = error).
 */
typedef struct s_builtin_dispatch
{
	const char			*name;
	uint8_t				(*func)(t_cmd *);
}						t_builtin_disp;

/**
 * @struct	s_mshell_var
 * @brief	Represents a single environment variable in minishell.
 *
 * - `key`:			The key of the environment variable (e.g., "PATH").
 * - `value`:		The value associated with the variable
 * (e.g., "/usr/bin:/bin").
 * - `val_assigned`:	Indicates if the variable has value (1) or not (0).
 * - `next`:		Pointer to the next variable in the
 * 	linked list (for handling collisions in hash table).
 */
typedef struct s_mshell_var
{
	char				*key;
	char				*value;
	int					val_assigned;
	struct s_mshell_var	*next;
}						t_mshell_var;

/**
 * @struct	s_hash_table
 * @brief	Represents the hash table for environment variables.
 *
 * - `buckets`:	An array of linked lists (chained hashing) for
 * 				storing environment variables.
 *				The index in the array is determined by a hash function.
 */
typedef struct s_hash_table
{
	t_mshell_var		*buckets[HASH_SIZE];
}						t_hash_tbl;

typedef struct s_mshell
{
	char				**env;
	t_hash_tbl			*hash_table;
	bool				pipe;
	uint8_t				exit_status;
	uint8_t				syntax_exit_status;
}						t_mshell;
// run progrmm

char			*read_user_input(void);
bool			handle_null_command(t_mshell *mshell, char **input);
bool			handle_signal_after_parse(t_mshell *mshell, t_cmd **cmd,
					char **input);
void			handle_signal_after_exec(t_mshell *mshell);
bool			is_input_interactive(void);
uint8_t			run_script_mode(t_mshell *mshell, const char *file);
uint8_t			run_interactive_mode(t_mshell *mshell);

// init minishell
t_mshell		*init_mshell(char **envp);
char			**setup_env(char **envp);
char			**setup_builtin(void);
char			*find_binary(t_cmd *cmd);
char			*create_env_entry(t_mshell_var *var);
void			free_old_env(char **env);

// setup hash table
int				setup_hash_table(t_mshell *mshell);
bool			add_env_entry(t_mshell_var *current,
					char **new_env,
					int *index);
bool			process_env_bucket(t_mshell_var *bucket,
					char **new_env,
					int *index);
int				set_variable(t_mshell *mshell,
					char *key,
					char *value,
					int assigned);
void			update_existing_variable(t_mshell_var *current,
					t_mshell_var *mshell_var,
					int val_assigned,
					t_mshell *mshell);
unsigned int	hash_function(const char *key);
int				update_env(t_mshell *mshell);
char			*search_paths(char **paths, t_cmd *cmd);
void			free_pair_and_return_null(t_mshell_var *pair);
t_mshell_var	*create_new_var(char *key, char *value, int assigned);
int				update_existing_var(t_mshell_var *var, char
					*value, int assigned);
int				update_existing_var(t_mshell_var *var,
					char *value, int assigned);
int				insert_new_var(t_mshell *mshell, char *key,
					char *value, int assigned);
int				set_variable(t_mshell *mshell, char *key,
					char *value, int assigned);

// parser
t_cmd			*run_parser(t_mshell *shell, char *input);

// executor
uint8_t			run_executor(t_cmd *cmd);
size_t			ft_arr_size(char **arr);
char			*ms_getenv(t_mshell *mshell, char *key);

// test
bool			is_debug_mode(void);
void			debug_printf(const char *format, ...);
void			print_error(const char *msg);

// free memory utils
void			free_minishell(t_mshell **minishell);
void			free_cmd(t_cmd **cmd);
void			free_mshell_var(t_mshell_var *var);
void			free_cmd_node(t_cmd *cmd);

int				readline_interrupt_hook(void);
void			free_ptr(void **ptr);

#endif /* MINISHELL_H */
