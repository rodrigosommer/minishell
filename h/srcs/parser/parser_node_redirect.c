#include "../../includes/minishell.h"

t_node	*redirect_out(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_OUT, NULL);
	node->filename = tokdup(tok->next);
	node->target_fd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

t_node	*redirect_in(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_IN, NULL);
	node->filename = tokdup(tok->next);
	node->target_fd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}

t_node	*redirect_append(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_APPEND, NULL);
	node->filename = tokdup(tok->next);
	node->target_fd = STDOUT_FILENO;
	*rest = tok->next->next;
	return (node);
}

t_node	*redirect_heredoc(t_token **rest, t_token *tok)
{
	t_node	*node;

	node = new_node(ND_REDIR_HEREDOC, NULL);
	node->delimiter = tokdup(tok->next);
	node->target_fd = STDIN_FILENO;
	*rest = tok->next->next;
	return (node);
}
