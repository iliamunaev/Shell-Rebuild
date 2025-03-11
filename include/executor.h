#ifndef EXECUTOR_H
# define EXECUTOR_H

// Heredoc temp file
# define HEREDOC_TFILE "heredoc_tmp.txt"

//limits for file name and full path
# define PATH_MAX 4096
# define NAME_MAX 255

/*------FORWARD DECLARATIONS-----------------------------------------*/

typedef struct s_cmd	t_cmd;
typedef struct s_mshell	t_mshell;
typedef struct s_mshell_var	t_mshell_var;
typedef struct s_hash_table	t_hash_table;

/*------FUNCTIONS---------------------------------------------------*/

// standart cmd exec functions
void			exec_cmd(t_cmd *cmd);

// builtin functions
bool			is_builtin(t_cmd *cmd);
uint8_t			exec_builtin(t_cmd *cmd);
uint8_t			handle_exit(t_cmd *cmd);
uint8_t			handle_echo(t_cmd *cmd);
uint8_t			handle_pwd(t_cmd *cmd);
uint8_t			handle_cd(t_cmd *cmd);
uint8_t			handle_env(t_cmd *cmd);
uint8_t			handle_export(t_cmd *cmd);
uint8_t			handle_unset(t_cmd *cmd);
t_mshell_var	*create_new_variable( char *key,  char *value, int assigned);

// redirections <, <<, >, >>
void			handle_redirections(t_cmd *cmd, int in_fd);
void			handle_heredoc(t_cmd *cmd);

// void	cleanup_heredoc(t_cmd *cmd);

// utils
bool			is_valid_varname(const char *key_value_pair);
t_mshell_var	*split_key_value(char *key_value_pair);
bool			is_pipeline_limit(int *cmd_count);

#endif /* EXECUTOR_H */
