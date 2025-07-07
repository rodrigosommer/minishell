#include "../../includes/minishell.h"

bool	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool	is_keyword(const char *s, const char *keyword)
{
	return (ft_memcmp(s, keyword, ft_strlen(keyword)) == 0);
}

bool	is_redirection_operator(const char *s)
{
	static char *const	operators[] = {">", "<", ">>", "<<"};
	size_t				i;

	i = 0;
	while (i < sizeof(operators) / sizeof(*operators))
	{
		if (is_keyword(s, operators[i]))
			return (true);
		i++;
	}
	return (false);
}

bool	is_metacharacter(char c)
{
	if (c && (c == '|' || c == '&' || c == ';' || c == '(' || c == ')'
			|| c == '<' || c == '>' || c == ' ' || c == '\t' || c == '\n'))
		return (true);
	else
		return (false);
}

bool	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}
