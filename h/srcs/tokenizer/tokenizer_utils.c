#include "../../includes/minishell.h"

size_t	get_token_count(t_token *token)
{
	size_t	count;
	t_token	*cursor;

	count = 0;
	cursor = token;
	while (cursor->word != NULL && cursor->kind != TK_EOF)
	{
		cursor = cursor->next;
		count++;
	}
	return (count);
}

char	**token_list_to_array(t_token *token)
{
	size_t	count;
	size_t	i;
	char	**tok_array;
	t_token	*cursor;

	count = get_token_count(token);
	tok_array = ft_calloc(count + 1, sizeof(char *));
	if (!tok_array)
		fatal_error("calloc");
	cursor = token;
	i = 0;
	while (cursor->word != NULL && cursor->kind != TK_EOF)
	{
		tok_array[i] = ft_strdup(cursor->word);
		if (!tok_array[i])
			fatal_error("strdup");
		i++;
		cursor = cursor->next;
	}
	tok_array[i] = NULL;
	return (tok_array);
}

bool	consume_blank(char **rest, char *line)
{
	if (is_blank(*line))
	{
		while (*line && is_blank(*line))
			line++;
		*rest = line;
		return (true);
	}
	*rest = line;
	return (false);
}
