#ifndef EXECUTOR_H
# define EXECUTOR_H

/*------FORWARD DECLARATIONS-----------------------------------------*/

typedef struct s_cmd				t_cmd;
typedef struct s_mshell				t_mshell;
typedef struct s_mshell_var			t_mshell_var;
typedef struct s_hash_table			t_hash_table;
typedef enum e_redir_type			t_redir_type;
typedef struct s_redir				t_redir;
typedef struct s_builtin_dispatch	t_builtin_dispatch;

typedef struct s_pipe_info
{
	int		in_fd;
	int		*pipe_fd;
	pid_t	*pids;
	int		*idx;
	t_cmd	*cmd_list;
}	t_pipe_info;

/*------FUNCTIONS---------------------------------------------------*/

// builtin
bool						is_builtin(t_cmd *cmd);
uint8_t						exec_builtins(t_cmd *cmd);
const t_builtin_dispatch	*get_builtin_table(size_t *size);

// parent process
uint8_t						exec_in_current_process(t_cmd *cmd);

// pipes
bool						is_heredoc(t_redir *redirection);
uint8_t						exec_in_pipes(t_cmd *cmd);
void						execute_command(t_cmd *cmd);
void						handle_child_and_track(t_cmd *cmd, t_pipe_info *info);
uint8_t						wait_for_children(pid_t *pids, int count);
uint8_t						close_unused_fds(int in_fd, int *pipe_fd);
uint8_t						close_heredoc_fds(t_cmd *cmd);
void						close_fds_and_prepare_next(int *in_fd, int *pipe_fd);
void						close_all_heredoc_fds(t_cmd *cmd_list);

// pipe utils
bool						is_minishell_executable(t_cmd *cmd);
uint8_t						validate_dots(t_cmd *cmd);
uint8_t						update_shlvl(t_cmd *cmd);
void						handle_empty_command(t_cmd *cmd);
uint8_t						validate_dots(t_cmd *cmd);

// exit
uint8_t						handle_exit(t_cmd *cmd);
long long					ft_atoll_exit(const char *str, bool *overflow);

// echo
uint8_t						handle_echo(t_cmd *cmd);
int							is_echo_flag(char *arg);
int							is_wrapped_in_quotes(char *s);

// pwd
uint8_t						handle_pwd(t_cmd *cmd);

// env
uint8_t						handle_env(t_cmd *cmd);

// export
uint8_t						handle_export(t_cmd *cmd);
void						handle_sorted_env(t_mshell *mshell);
void						collect_keys(t_hash_table *ht, char ***keys, int *count);

// unset
uint8_t						handle_unset(t_cmd *cmd);
t_mshell_var				*create_new_variable(char *key, char *value, int assigned);

// cd
uint8_t						handle_cd(t_cmd *cmd);
bool						get_directory(char *cwd, t_cmd *cmd);

// redirections <, <<, >, >>
uint8_t						apply_redirections(t_cmd *cmd);
uint8_t						apply_heredocs(t_cmd *cmd);

// utils
t_mshell_var				*split_key_value(char *key_value_pair);
bool						is_pipeline_limit(int *cmd_count);

// exit utils
void						child_execve_error(t_cmd *cmd);
int						perror_return(char *msg, int exit_status);
void						cmd_missing_command_error(t_cmd *cmd);
void						export_error(t_mshell_var *pair);
void						unset_error(char *str);
uint8_t						perror_exit_child(char *msg, u_int8_t exit_status);
int						error_return(char *msg, int exit_status);
#endif /* EXECUTOR_H */
