#include "../../includes/minishell.h"

bool	parser_check_pipe(t_node *node, t_token *tok)
{
	t_token	*tmp;

	if (node->args == NULL || tok->next->kind == TK_EOF)
	{
		parse_error(&tok, tok);
		return (false);
	}
	tmp = node->args;
	while (tmp->next != NULL)
		tmp = tmp->next;
	if (tmp->kind != TK_WORD)
	{
		parse_error(&tok, tok);
		return (false);
	}
	return (true);
}
