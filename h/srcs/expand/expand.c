#include "../../includes/minishell.h"

static void	remove_quotes(char **p, char quote_char, char **new_word)
{
	(*p)++;
	if (**p == quote_char)
		append_char(new_word, '\0');
	else
	{
		while (**p != quote_char)
			append_char(new_word, *(*p)++);
	}
	(*p)++;
}

static void	delemiter_quote_check(t_token *tok)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	while (*p && !is_metacharacter(*p))
	{
		if (*p == SINGLE_QUOTE_CHAR)
			remove_quotes(&p, SINGLE_QUOTE_CHAR, &new_word);
		else if (*p == DOUBLE_QUOTE_CHAR)
			remove_quotes(&p, DOUBLE_QUOTE_CHAR, &new_word);
		else
			append_char(&new_word, *p++);
	}
	free(tok->word);
	tok->word = new_word;
	delemiter_quote_check(tok->next);
}

static void	expand_quote_removal(t_node *node, t_environ *env)
{
	if (node == NULL)
		return ;
	process_word_token(node->args, env);
	process_word_token(node->filename, env);
	delemiter_quote_check(node->delimiter);
	expand_quote_removal(node->redirects, env);
	expand_quote_removal(node->next, env);
}

void	expand(t_node *node, t_environ *env)
{
	expand_quote_removal(node, env);
}
