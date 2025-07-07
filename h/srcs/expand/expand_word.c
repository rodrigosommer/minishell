#include "../../includes/minishell.h"

void	append_char(char **s, char c)
{
	size_t	size;
	char	*new;

	size = 2;
	if (*s)
		size += ft_strlen(*s);
	new = malloc(size);
	if (new == NULL)
		fatal_error("malloc");
	if (*s)
		ft_strlcpy(new, *s, size);
	new[size - 2] = c;
	new[size - 1] = '\0';
	if (*s)
		free(*s);
	*s = new;
}

static void	process_quotes(char **p, char quote_char, char **new_word,
		t_environ *env)
{
	(*p)++;
	if (**p == quote_char)
		append_char(new_word, '\0');
	else
	{
		while (**p != quote_char)
		{
			if (**p == DOLLAR_SIGN && quote_char == DOUBLE_QUOTE_CHAR)
				dollar_sign(p, new_word, env);
			else
				append_char(new_word, *(*p)++);
		}
	}
	(*p)++;
}

static void	process_word_token_helper(char **p, char **new_word, t_environ *env)
{
	while (**p && !is_metacharacter(**p))
	{
		if (**p == DOLLAR_SIGN)
			dollar_sign(p, new_word, env);
		else if (**p == SINGLE_QUOTE_CHAR)
			process_quotes(p, SINGLE_QUOTE_CHAR, new_word, env);
		else if (**p == DOUBLE_QUOTE_CHAR)
			process_quotes(p, DOUBLE_QUOTE_CHAR, new_word, env);
		else
			append_char(new_word, *(*p)++);
	}
}

void	process_word_token(t_token *tok, t_environ *env)
{
	char	*new_word;
	char	*p;

	if (tok == NULL || tok->kind != TK_WORD || tok->word == NULL)
		return ;
	p = tok->word;
	new_word = NULL;
	process_word_token_helper(&p, &new_word, env);
	free(tok->word);
	tok->word = new_word;
	process_word_token(tok->next, env);
}
