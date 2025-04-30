/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Ilia Munaev <ilyamunaev@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:46:42 by imunaev-          #+#    #+#             */
/*   Updated: 2025/04/30 19:01:01 by Ilia Munaev      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

/*------FORWARD DECLARATIONS-----------------------------------------*/

typedef struct s_cmd				t_cmd;
typedef struct s_mshell				t_mshell;
typedef struct s_mshell_var			t_mshell_var;
typedef struct s_hash_table			t_hash_tbl;
typedef enum e_redir_type			t_redir_type;
typedef struct s_redir				t_redir;
typedef struct s_builtin_dispatch	t_builtin_disp;

typedef struct s_exit_info
{
	const char	*prefix;
	const char	*arg;
	const char	*msg;
	int			code;
	t_mshell	*mshell;
}	t_exit_info;

typedef struct s_pipe_info
{
	int		in_fd;
	int		*pipe_fd;
	pid_t	*pids;
	int		*idx;
	t_cmd	*cmd_list;
}	t_pipe_info;

/*------FUNCTIONS---------------------------------------------------*/

// executor utils
bool					is_valid_varname(const char *key);
bool					is_exit_command(t_cmd *cmd);
void					cleanup_and_exit(t_cmd *cmd, int exit_status);
bool					command_too_long(char **argv);
void					update_underscore(t_cmd *cmd, char *binary_path);

// parent process
uint8_t					exec_in_current_process(t_cmd *cmd);

// pipes
uint8_t					exec_in_pipes(t_cmd *cmd);
void					execute_command(t_cmd *cmd);
void					handle_child_and_track(t_cmd *cmd, t_pipe_info *info);
uint8_t					wait_for_children(pid_t *pids, int count);
uint8_t					close_unused_fds(int in_fd, int *pipe_fd);

// pipe utils
bool					is_minishell_executable(t_cmd *cmd);
uint8_t					validate_dots(t_cmd *cmd);
uint8_t					update_shlvl(t_cmd *cmd);
void					handle_empty_command(t_cmd *cmd);
uint8_t					close_unused_fds(int in_fd, int *pipe_fd);

// builtin
const t_builtin_disp	*get_builtin_table(size_t *size);
bool					is_builtin(t_cmd *cmd);
uint8_t					exec_builtins(t_cmd *cmd);

// exit
uint8_t					handle_exit(t_cmd *cmd);
long long				ft_atoll_exit(const char *str, bool *overflow);
uint8_t					get_exit_status(char *arg);
bool					is_valid_numeric_exit_arg(const char *arg);

// echo
uint8_t					handle_echo(t_cmd *cmd);
int						is_echo_flag(char *arg);
int						is_wrapped_in_quotes(char *s);

// pwd
uint8_t					handle_pwd(t_cmd *cmd);

// env
uint8_t					handle_env(t_cmd *cmd);

// export
uint8_t					handle_export(t_cmd *cmd);
void					handle_sorted_env(t_mshell *mshell);
void					collect_keys(t_hash_tbl *ht, char ***keys, int *count);

// unset
uint8_t					handle_unset(t_cmd *cmd);
t_mshell_var			*create_new_var(char *key, char *value, int assigned);

// cd
uint8_t					handle_cd(t_cmd *cmd);
bool					get_directory(char *cwd, t_cmd *cmd);
void					update_pwd_variables(t_cmd *cmd, char *old_cwd);
uint8_t					cd_too_many_args(void);

// redirections <, >, >>
uint8_t					apply_redirections(t_cmd *cmd);

// heredoc <<
uint8_t					apply_heredocs(t_cmd *cmd);
bool					heredoc_exceeds_limit(size_t total_written);
bool					is_heredoc(t_redir *redirection);
void					close_all_heredoc_fds(t_cmd *cmd_list);
int						write_heredoc_to_pipe(t_cmd *cmd,
							int pipe_fd, const char *delim);
void					close_unused_heredocs_child(t_cmd *current,
							t_cmd *full_cmd_list);
int						handle_heredoc_redirection(t_redir *r);

// utils
t_mshell_var			*split_key_value(char *key_value_pair);
void					safe_close(int *fd);

// exit utils
int						perror_return(const char *msg, int exit_status);
uint8_t					perror_exit_child(t_cmd *cmd,
							char *msg, u_int8_t exit_status);
void					cmd_missing_command_error(t_cmd *cmd);
void					export_error(t_mshell_var *pair);
u_int8_t				unset_error(const char *str);
int						error_return(char *msg, int exit_status);
uint8_t					no_cmd_error(const char *msg);
void					child_execve_error(t_cmd *cmd);
void					handle_is_directory(t_cmd *cmd);
void					handle_not_found_or_command(t_cmd *cmd);
void					handle_permission_denied(t_cmd *cmd);
void					handle_exec_format_error(t_cmd *cmd);
void					handle_generic_execve_error(t_cmd *cmd);
void					print_and_exit(t_exit_info info);

#endif /* EXECUTOR_H */
