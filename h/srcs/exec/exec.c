#include "../../includes/minishell.h"

static size_t	get_environ_cnt(t_environ *node)
{
	size_t	cnt;

	cnt = 0;
	while (node != NULL)
	{
		cnt++;
		node = node->next;
	}
	return (cnt);
}

static size_t	get_node_cnt(t_node *node)
{
	size_t	cnt;

	cnt = 0;
	while (node != NULL)
	{
		cnt++;
		node = node->next;
	}
	return (cnt);
}

void	execution(t_node *node, t_environ *environ)
{
	size_t	cnt_node;
	pid_t	pid;
	int		wstatus;

	wstatus = 0;
	cnt_node = get_node_cnt(node);
	if (cnt_node <= 1)
	{
		if (search_bi_cmd(node, environ) != 1)
			return ;
		signal(SIGINT, SIG_IGN);
		pid = fork();
		if (pid < 0)
			fatal_error("fork");
		else if (pid == 0)
			exec_cmd(node, environ);
		else
			wait(&wstatus);
	}
	else
	{
		pipex(node, cnt_node, environ);
		waitpid_pipex(node, &wstatus);
	}
	finalize(node, wstatus);
}

static char	**env_list_to_array(t_environ *environ)
{
	t_environ	*tmp_env;
	size_t		i;
	char		**list_env;
	char		*tmp;

	tmp_env = environ;
	list_env = ft_calloc(get_environ_cnt(tmp_env) + 1, sizeof(char *));
	if (!list_env)
		fatal_error("calloc");
	i = 0;
	while (tmp_env)
	{
		tmp = ft_strjoin(tmp_env->name, "=");
		list_env[i] = ft_strjoin(tmp, tmp_env->value);
		tmp_env = tmp_env->next;
		i++;
	}
	return (list_env);
}

void	exec_cmd(t_node *node, t_environ *mini_environ)
{
	char	**argv;

	redirection(node->redirects);
	if (g_global.flg_redir != 0)
		exit(EXIT_FAILURE);
	argv = token_list_to_array(node->args);
	argv[0] = get_cmd_array(argv[0]);
	if (argv[0] != NULL)
	{
		signal(SIGINT, signal_handler_waiting_input);
		signal(SIGQUIT, signal_handler_waiting_input);
		if (execve(argv[0], argv, env_list_to_array(mini_environ)) == -1)
			error_exit(argv[0]);
	}
	else
		error_cmd(node->args->word);
}
