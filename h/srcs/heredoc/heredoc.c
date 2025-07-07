#include "../../includes/minishell.h"

static void	process_heredoc_line(char *str, t_node *redir, t_environ *env)
{
	char	*new_word;
	char	*tmp;

	new_word = NULL;
	while (*str)
	{
		if (*str == DOLLAR_SIGN)
		{
			dollar_sign(&str, &new_word, env);
			tmp = new_word;
			while (*tmp)
				do_write(redir->file_fd, tmp++, 1);
			if (new_word)
			{
				free(new_word);
				new_word = NULL;
			}
		}
		else
			do_write(redir->file_fd, str++, 1);
	}
	do_write(redir->file_fd, "\n", 1);
}

void	do_heredoc(t_node *redir, t_environ *env)
{
	char	*buff;

	if (redir == NULL || redir->kind != ND_REDIR_HEREDOC)
		return ;
	g_global.status = 0;
	rl_done = 0;
	rl_event_hook = signal_monitor;
	signal(SIGINT, signal_handler_heredoc);
	while (g_global.status != 1 || g_global.flg_redir != 1)
	{
		buff = readline("heredoc> ");
		if (buff == NULL)
			break ;
		if (!ft_strcmp(buff, redir->delimiter->word))
			break ;
		process_heredoc_line(buff, redir, env);
		free(buff);
		buff = NULL;
	}
	rl_event_hook = NULL;
	signal(SIGINT, signal_handler);
	if (buff != NULL)
		free(buff);
}

static void	open_heredoc(t_node *redir, t_environ *env, size_t i)
{
	char	*ci;

	if (redir == NULL || g_global.flg_redir != 0
		|| redir->kind != ND_REDIR_HEREDOC)
		return ;
	ci = ft_itoa(i);
	redir->filename = new_token(NULL, TK_WORD);
	redir->filename->word = ft_strjoin(".heredoc", ci);
	free(ci);
	delete_heredoc(redir->filename->word);
	redir->file_fd = do_open_redir_append(redir->filename->word);
	if (redir->file_fd < 0)
	{
		g_global.flg_redir = 1;
		return ;
	}
	do_heredoc(redir, env);
	do_close(redir->file_fd);
	open_heredoc(redir->next, env, i);
}

void	check_heredoc(t_node *node, t_environ *env)
{
	size_t	i;
	t_node	*tmp;

	i = 0;
	tmp = node;
	g_global.flg_redir = 0;
	while (tmp)
	{
		open_heredoc(tmp->redirects, env, i);
		if (g_global.flg_redir != 0)
			break ;
		tmp = tmp->next;
		i++;
	}
}
