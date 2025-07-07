#ifndef MINISHELL_H
# define MINISHELL_H

# include "../srcs/lib/libft/libft.h"
# include <fcntl.h>
# include <signal.h>
# include <stdbool.h>
//#include <global.h>
#include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/param.h>
#include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# define SINGLE_QUOTE_CHAR '\''
# define DOUBLE_QUOTE_CHAR '"'
# define ERROR_PREFIX "minishell: "
# define DOLLAR_SIGN '$'
# define SIG_GET 100
# define SIG_SET 200

typedef struct s_token		t_token;
typedef struct s_node		t_node;
typedef enum e_token_kind	t_token_kind;
typedef enum e_node_kind	t_node_kind;
typedef struct stat			t_stat;
typedef struct s_global		t_global;
typedef struct s_environ	t_environ;
extern t_global					g_global;

t_token						*new_token(char *word, t_token_kind kind);

struct						s_global
{
	bool					syntax_error;
	int						status;
	int						flg_redir;
	t_environ				*env_head;
};

enum						e_token_kind
{
	TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF,
};

enum						e_node_kind
{
	ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_APPEND,
	ND_REDIR_HEREDOC,
};

struct						s_node
{
	t_node_kind				kind;
	t_node					*next;
	t_node					*prev;
	t_token					*args;
	t_node					*redirects;
	int						target_fd;
	t_token					*filename;
	t_token					*delimiter;
	int						file_fd;
	int						stacktmp_fd;
	pid_t					pid;
	int						pfd[2];
};

struct						s_token
{
	char					*word;
	t_token_kind			kind;
	t_token					*next;
};

struct						s_environ
{
	char					*name;
	char					*value;
	struct s_environ		*next;
};

// ------------------------------------------------
// builtin
// ------------------------------------------------

int							bi_cd(char **argv, int argc);
int							bi_echo(char **argv);
int							bi_env(int argc, t_environ *environ);
int							bi_pwd(void);
int							int_overflow_check(char *str, long *num);
int							bi_exit(char **argv);
int							bi_export(t_environ *env, char **argv, int argc);
void						remove_variable(t_environ *environ, t_environ *var);
int							bi_unset(t_environ *environ, char **argv, int argc);
// utils
char						*make_name(char *str);
char						*make_name_export(char *str);
char						*make_value(char *str);
t_environ					*find_variable(t_environ *environ, char *str);
void						environ_nodeadd_back(t_environ *env,
								t_environ *new);
t_environ					*environ_node_new(char *name, char *value);

// ------------------------------------------------
// Error
// ------------------------------------------------

void						perror_prefix(void);
void						fatal_error(char *msg);
void						assert_error(char *msg);
void						tokenize_error(char *location, char **rest,
								char *line, int flg);
void						parse_error(t_token **rest, t_token *tok);
void						error_cmd(char *cmd);
void						put_error_msg(char *error_msg);
void						put_error_msg_endl(char *error_msg);
void						put_error_char(char c);
void						error_file(char *filename);
void						error_exit(char *cmd);

// ------------------------------------------------
// exec
// ------------------------------------------------

char						*get_cmd_array(char *cmd_line);
void						exec_cmd(t_node *node, t_environ *mini_environ);
void						execution(t_node *node, t_environ *environ);

// ------------------------------------------------
// expand
// ------------------------------------------------

void						expand(t_node *node, t_environ *env);
void						append_char(char **s, char c);
void						process_word_token(t_token *tok, t_environ *env);
void						dollar_sign(char **p, char **new_word,
								t_environ *env);

// ------------------------------------------------
// heredoc
// ------------------------------------------------

void						loop_node_delete_heredoc(t_node *node);
void						delete_heredoc(char *filename);
void						do_heredoc(t_node *redir, t_environ *env);
void						check_heredoc(t_node *node, t_environ *env);

// ------------------------------------------------
// Parser
// ------------------------------------------------

bool						parser_check_pipe(t_node *node, t_token *tok);
t_token						*tokdup(t_token *tok);
void						append_tok(t_token **tokens, t_token *tok);
t_node						*redirect_out(t_token **rest, t_token *tok);
t_node						*redirect_in(t_token **rest, t_token *tok);
t_node						*redirect_append(t_token **rest, t_token *tok);
t_node						*redirect_heredoc(t_token **rest, t_token *tok);
t_node						*parse(t_token *tok);
t_node						*new_node(t_node_kind kind, t_node *prev);
void						append_command_element(t_node *command,
								t_token **rest,
								t_token *tok);

// ------------------------------------------------
// pipe
// ------------------------------------------------
void						pipex(t_node *node, size_t cnt_node,
								t_environ *environ);
void						waitpid_pipex(t_node *node, int *wstatus);
void						pipex_utils(t_node *node, int flag,
								t_environ *environ);

// ------------------------------------------------
// recirection
// ------------------------------------------------
void						redirection(t_node *redir);
void						open_redir_file(t_node *redir);
void						reset_redirect(t_node *redir);
void						close_fd(t_node *redir);
int							do_open_redir_out(char *filepath);
int							do_open_redir_in(char *filepath);
int							do_open_redir_append(char *filepath);

// ------------------------------------------------
// signal
// ------------------------------------------------
void						signal_c_cmd(void);
void						signal_backslash(void);
void						signal_handler(int sig);
int							signal_setget_status(int style, int sig);
void						signal_handler_heredoc(int sig);
void						signal_handler_waiting_input(int sig);
int							signal_monitor(void);

// ------------------------------------------------
// tokenizer
// ------------------------------------------------

t_token						*tokenize(char *line);
t_token						*new_token(char *word, t_token_kind kind);
bool						is_blank(char c);
bool						is_keyword(const char *s, const char *keyword);
bool						is_redirection_operator(const char *s);
bool						is_metacharacter(char c);
bool						is_word(const char *s);
int							check_quote(char **line);
size_t						get_token_count(t_token *token);
char						**token_list_to_array(t_token *token);
bool						consume_blank(char **rest, char *line);

// ------------------------------------------------
// WRAPPER FUNCTION
// ------------------------------------------------
void						do_close(int fd);
void						do_write(int fd, const void *buf, size_t count);
int							do_dup(int oldfd);
void						do_dup2(int oldfd, int newfd);
void						do_pipe(int pipefd[2]);
void						do_unlink(char *str);

// ------------------------------------------------
// Destructors
// ------------------------------------------------

void						finalize(t_node *node, int wstatus);
void						free_token(t_token *tok);
void						free_nodelist(t_node *node);
void						free_argv(char **args);
void						set_wstatus(int wstatus);

// ------------------------------------------------
//
// ------------------------------------------------

//1_init_environ_list
int							first_strlen(char *str);
int							latter_strlen(char *str);
t_environ					*init_environ_list(void);

//2_builtin_search
int							search_bi_cmd(t_node *node, t_environ *environ);

//3_destructors

#endif
