#include "../../includes/minishell.h"

int	do_open_redir_out(char *filepath)
{
	int	fd;

	fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, \
				S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd < 0)
		error_file(filepath);
	return (fd);
}

int	do_open_redir_in(char *filepath)
{
	int	fd;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		error_file(filepath);
	return (fd);
}

int	do_open_redir_append(char *filepath)
{
	int	fd;

	fd = open(filepath, O_WRONLY | O_CREAT | O_APPEND, \
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd < 0)
		error_file(filepath);
	return (fd);
}

void	reset_redirect(t_node *redir)
{
	if (redir == NULL)
		return ;
	reset_redirect(redir->next);
	if (redir->kind == ND_REDIR_OUT || redir->kind == ND_REDIR_IN \
		|| redir->kind == ND_REDIR_APPEND || redir->kind == ND_REDIR_HEREDOC)
	{
		do_close(redir->file_fd);
		do_close(redir->target_fd);
		do_dup2(redir->stacktmp_fd, redir->target_fd);
	}
}

void	close_fd(t_node *redir)
{
	if (redir == NULL || redir->file_fd <= 0)
		return ;
	do_close(redir->file_fd);
	close_fd(redir->next);
}
