/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:17:51 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:17:54 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_env t_env;
typedef struct s_cmd t_cmd;
typedef struct s_data t_data;

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>
# include <string.h>
# include <fcntl.h>
# include <termios.h>
# include "executor_utils.h"
# include "../libft/libft.h"
# include "../ft_printf/ft_printf.h"

# define PROMPT "minishell> "
# define SYNTAX_ERROR_MSG "syntax error near unexpected token"

extern t_data *g_shell_data;
extern int	g_exit_status;
extern volatile sig_atomic_t g_command_executing;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_cmd
{
	char			**args;
	int				in_fd;
	int				out_fd;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
    t_env   *env;
    t_cmd   *cmd;
    int     exit_status;
    char    *input;
    int     original_stdin;
    int     original_stdout;
    int     executing_command;  // Flag for command execution state
}   t_data;

// Global variable for signals
extern volatile sig_atomic_t	g_signal;

// Main functions
void	init_shell(t_data *data, char **envp);
void	run_shell(t_data *data);
void	cleanup_shell(t_data *data);

// Builtins
int		builtin_echo(t_cmd *cmd);
int		builtin_cd(t_cmd *cmd, t_env *env);
int		builtin_pwd(void);
int		builtin_export(t_cmd *cmd, t_env **env);
int		builtin_unset(t_cmd *cmd, t_env **env);
int		builtin_env(t_env *env);
int		builtin_exit(t_cmd *cmd, t_data *data);

// Execution
int is_builtin(t_cmd *cmd);
int execute_builtin(t_cmd *cmd, t_env **env, t_data *data);
char **env_to_array(t_env *env);
void        execute_child(t_cmd *cmd, t_env *env, t_data *data);
int		execute_command(t_cmd *cmd, t_env *env, t_data *data);
void handle_redirections(t_cmd *cmd);
int handle_pipes(t_cmd *cmd, t_env *env, t_data *data);
void reset_terminal(void);

// Parsing
t_cmd	*parse_input(char *input, t_env *env, t_data *data);
char	**tokenize_input(char *input);
char	*expand_variables(char *token, t_env *env, int exit_status);
char *process_quotes_and_expand(char *token, t_env *env, int exit_status);

// Utils
int		handle_heredoc(char *delimiter);
t_env	*init_env(char **envp);
char	*get_env_value(t_env *env, char *key);
void	set_env_value(t_env **env, char *key, char *value);
char	*ft_strjoin3(char const *s1, char const *s2, char const *s3);
void	print_error(char *cmd, char *arg, char *error);
void	free_array(char **array);
void	free_commands(t_cmd *cmd);
void	free_env(t_env *env);
int	is_valid_env_key(char *key);
void clean_readline(void);

// Signals
void	setup_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	setup_child_signals(void);
void setup_terminal(void);
void restore_terminal(void);
int is_executing_command(void);

#endif
