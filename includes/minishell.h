/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include "../libft/libft.h"
# include "../ft_printf/ft_printf.h"

# define PROMPT "minishell$ "

extern int	g_signal_received;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_REDIRECT_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redirect
{
	t_token_type		type;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command
{
	char				**args;
	t_redirect			*redirects;
	t_token_type		operator;
	struct s_command	*next;
}	t_command;

typedef struct s_shell
{
	char		**env;
	int			last_exit_status;
	t_command	*commands;
}	t_shell;

typedef struct s_pipe_context
{
	int		**pipes;
	pid_t	*pids;
	int		cmd_count;
}	t_pipe_context;

int			main(int argc, char **argv, char **envp);
void		shell_loop(t_shell *shell);
void		handle_signals(void);
void		signal_handler(int sig);

t_token		*tokenize(char *input);
void		free_tokens(t_token *tokens);
t_token		*create_token(t_token_type type, char *value);
void		add_token(t_token **tokens, t_token *new_token);
char		*get_word(char *input, int *i);
void		skip_whitespace(char *input, int *i);
t_token		*handle_redirect(char *input, int *i);
t_token		*handle_operators(char *input, int *i);
t_token		*process_token(char *input, int *i);

t_command	*parse(t_token *tokens, t_shell *shell);
void		free_commands(t_command *commands);
t_command	*create_command(void);
t_command	*parse_command(t_token **tokens, t_shell *shell);
void		add_command(t_command **commands, t_command *new_cmd);
char		*preprocess_parentheses(char *input, t_shell *shell);
t_redirect	*create_redirect(t_token_type type, char *file);
void		add_redirect(t_redirect **redirects, t_redirect *new_redirect);
int			handle_redirect_token(t_token **tokens, t_command *cmd);
void		free_redirects(t_redirect *redirects);

int			count_args(t_token *tokens);
int			process_word_token(t_token **tokens, t_command *cmd,
				t_shell *shell, int *i);
void		remove_empty_args(t_command *cmd);

char		*expand_variables(char *str, t_shell *shell);
char		*get_env_value(char *name, char **env);
char		*get_var_name_from_str(char *str, int *i);
char		*expand_var(char *name, t_shell *shell);
char		*handle_var_expansion(char *str, int *i, t_shell *shell);
char		*handle_char_addition(char *str, int *i);
char		*process_char_with_quotes(char *str, int *i, t_shell *shell);
int			count_env_vars(char **env);
char		**add_env_var(char **env, char *var);
char		**create_new_env(char **env, char *var, int count);

char		*get_var_name(char *env_var);
int			find_env_var_index(char **env, char *var_name);
char		*find_env_var(char **env, char *name);

int			execute_command(t_command *cmd, t_shell *shell);
void		exec_child(char *path, char **args, char **env);
int			exec_parent(pid_t pid, char *path);
int			validate_path(char *path, char *command);
void		free_paths(char **paths);
char		*check_path(char *command, char **paths, int i);
char		*get_command_path(char *command, char **env);
int			execute_external_no_wait(char **args, char **env);
int			execute_pipeline(t_command *commands, t_shell *shell);
int			execute_single_command(t_command *cmd, t_shell *shell);
int			execute_logical_operators(t_command *commands, t_shell *shell);
int			wait_for_children(pid_t *pids, int cmd_count, int **pipes);
int			count_commands(t_command *commands);

void		close_pipes(int **pipes, int cmd_count);
int			**create_pipes(int cmd_count);
void		setup_pipe_fds(int **pipes, int cmd_idx, int cmd_count);
int			has_pipe_operators(t_command *commands);

int			setup_redirections(t_redirect *redirects);
void		restore_std_fds(int saved_stdin, int saved_stdout);
int			read_heredoc_lines(int write_fd, char *delimiter);

int			builtin_echo(char **args);
int			builtin_cd(char **args, t_shell *shell);
int			builtin_pwd(void);
int			builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);
int			builtin_env(char **env);
int			builtin_exit(char **args, t_shell *shell);
int			is_builtin(char *command);
int			execute_builtin(char **args, t_shell *shell);

char		**copy_env(char **env);
void		free_env(char **env);

char		*remove_quotes(char *str);
int			has_quotes(char *str);

char		**expand_wildcards(char *str);
void		free_wildcard_results(char **results);
void		expand_command_wildcards(t_command *cmd);
int			match_pattern(char *str, char *pattern);
int			count_matches(char *dir_path, char *pattern);
char		**allocate_result_array(int count);
char		*create_full_path(char *dir_path, char *filename);
char		**handle_no_wildcard(char *str);
int			is_inside_quotes(char *input, char *pos);

#endif
