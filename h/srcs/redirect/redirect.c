#include "../../includes/minishell.h"

static int	stashfd(int fd)
{
	int	tmp_fd;

	tmp_fd = do_dup(fd);
	if (tmp_fd < 0)
		fatal_error("dup");
	do_close(fd);
	return (tmp_fd);
}

void	open_redir_file(t_node *redir)
{
	if (redir == NULL || g_global.flg_redir != 0)
		return ;
	if (redir->kind == ND_REDIR_OUT)
		redir->file_fd = do_open_redir_out(redir->filename->word);
	else if (redir->kind == ND_REDIR_IN)
		redir->file_fd = do_open_redir_in(redir->filename->word);
	else if (redir->kind == ND_REDIR_APPEND)
		redir->file_fd = do_open_redir_append(redir->filename->word);
	else if (redir->kind == ND_REDIR_HEREDOC)
		redir->file_fd = do_open_redir_in(redir->filename->word);
	else
		assert_error("open_redir_file");
	if (redir->file_fd < 0)
	{
		g_global.flg_redir = 1;
		return ;
	}
	open_redir_file(redir->next);
}

static void	do_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	if (redir->kind == ND_REDIR_OUT || redir->kind == ND_REDIR_IN \
		|| redir->kind == ND_REDIR_APPEND || redir->kind == ND_REDIR_HEREDOC)
	{
		redir->stacktmp_fd = stashfd(redir->target_fd);
		do_dup2(redir->file_fd, redir->target_fd);
	}
	else
		assert_error("do_redirect");
	do_redirect(redir->next);
}

void	redirection(t_node *redir)
{
	g_global.flg_redir = 0;
	open_redir_file(redir);
	if (g_global.flg_redir != 0)
		return ;
	do_redirect(redir);
}
