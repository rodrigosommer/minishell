#include "../../includes/minishell.h"

void	delete_heredoc(char *filename)
{
	if (!access(filename, R_OK))
		do_unlink(filename);
}

void	loop_node_delete_heredoc(t_node *node)
{
	t_node	*tmp;

	if (node == NULL)
		return ;
	tmp = node;
	while (tmp)
	{
		if (tmp->kind == ND_REDIR_HEREDOC && tmp->filename)
			delete_heredoc(tmp->filename->word);
		tmp = tmp->redirects;
	}
	loop_node_delete_heredoc(node->next);
}
