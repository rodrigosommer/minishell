#include "../../includes/minishell.h"

static t_token	*word(char **rest, char *line);
static t_token	*operator(char **rest, char *line);

t_token	*tokenize(char *line)
{
	t_token	head;
	t_token	*token;

	g_global.syntax_error = false;
	head.next = NULL;
	token = &head;
	while (*line)
	{
		if (consume_blank(&line, line))
			continue ;
		else if (is_metacharacter(*line))
		{
			token->next = operator(&line, line);
			token = token->next;
		}
		else if (is_word(line))
		{
			token->next = word(&line, line);
			token = token->next;
		}
		else
			tokenize_error("Unexpected Token", &line, line, 0);
	}
	token->next = new_token(NULL, TK_EOF);
	return (head.next);
}

t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*tok;

	tok = ft_calloc(1, sizeof(*tok));
	if (tok == NULL)
		perror("calloc");
	tok->word = word;
	tok->kind = kind;
	return (tok);
}

static t_token	*operator(char **rest, char *line)
{
	static char *const	operators[] = {">>", "<<", "||", "&", "&&", ";", ";;",
		">", "<", "(", ")", "|", "\n"};
	size_t				i;
	char				*op;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (is_keyword(line, operators[i]))
		{
			op = ft_strdup(operators[i]);
			if (op == NULL)
				fatal_error("strdup");
			*rest = line + ft_strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	assert_error("Unexpected operator");
	return (NULL);
}

static t_token	*word(char **rest, char *line)
{
	const char	*start = line;
	char		*word;
	int			quote_err_flg;

	quote_err_flg = 0;
	quote_err_flg = check_quote(&line);
	if (quote_err_flg == -1)
		tokenize_error("Unclosed single quote", rest, line, quote_err_flg);
	if (quote_err_flg == -2)
		tokenize_error("Unclosed double quote", rest, line, quote_err_flg);
	word = ft_strndup(start, line - start);
	if (word == NULL)
		fatal_error("strndup");
	*rest = line;
	return (new_token(word, TK_WORD));
}
