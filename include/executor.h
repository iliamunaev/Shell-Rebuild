#ifndef EXECUTOR_H
# define EXECUTOR_H

// Heredoc temp file
# define HEREDOC_TFILE "heredoc_tmp.txt"

# define ERROR_BUF_SIZE 256

//limits for file name and full path
# define MS_PATHMAX 4096
# define NAME_MAX 255

/*------FORWARD DECLARATIONS-----------------------------------------*/

typedef struct s_cmd	t_cmd;
typedef struct s_mshell	t_mshell;
typedef struct s_mshell_var	t_mshell_var;
typedef struct s_hash_table	t_hash_table;

/*------FUNCTIONS---------------------------------------------------*/

// standart cmd exec functions
uint8_t			exec_in_parent_process(t_cmd *cmd);
uint8_t exec_in_child_process(t_cmd *cmd);
uint8_t			exec_cmd(t_cmd *cmd);

// builtin functions
bool			is_builtin(t_cmd *cmd);
uint8_t			exec_builtin(t_cmd *cmd);

// exit
uint8_t			handle_exit(t_cmd *cmd);
long long	ft_atoll_exit(const char *str, bool *overflow);

uint8_t			handle_echo(t_cmd *cmd);
uint8_t			handle_pwd(t_cmd *cmd);
uint8_t			handle_env(t_cmd *cmd);

// export
uint8_t			handle_export(t_cmd *cmd);
void			handle_sorted_env(t_mshell *mshell);
void			free_keys(char **keys, int num_kyes);

uint8_t			handle_unset(t_cmd *cmd);
t_mshell_var	*create_new_variable( char *key,  char *value, int assigned);

// cd
uint8_t			handle_cd(t_cmd *cmd);
bool			is_valid_value(t_cmd *cmd);
bool 			get_directory(char *cwd, t_cmd *cmd);



// redirections <, <<, >, >>
uint8_t			apply_redirections(t_cmd *cmd);
void			handle_redirections(t_cmd *cmd, int in_fd);
uint8_t			apply_heredoc(t_cmd *cmd);

// void	cleanup_heredoc(t_cmd *cmd);

// utils
bool			is_valid_varname(const char *key_value_pair);
t_mshell_var	*split_key_value(char *key_value_pair);
bool			is_pipeline_limit(int *cmd_count);
void	set_pipe_flag(t_cmd *cmd);
// exit utils
// void			fatal_error_child(char *cmd, int exit_status);
void			fatal_error_child(t_cmd *cmd, int exit_status);
void			fatal_error(char *cmd, int exit_status);
void			env_error_handler(t_cmd *cmd);
uint8_t			cmd_error_handler(t_cmd *cmd, uint8_t exit_status);
uint8_t			exit_numeric_error(char *arg);
uint8_t			invalid_opt_exit(const char *cmd_name, const char *option);

#endif /* EXECUTOR_H */
