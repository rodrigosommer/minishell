/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:18:53 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:18:54 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	count_tokens(char *input)
{
	int	count;
	int	in_quote;
	int	in_dquote;

	count = 0;
	in_quote = 0;
	in_dquote = 0;
	while (*input)
	{
		if (*input == '\'' && !in_dquote)
			in_quote = !in_quote;
		else if (*input == '\"' && !in_quote)
			in_dquote = !in_dquote;
		else if (!in_quote && !in_dquote && ft_isspace(*input))
		{
			while (ft_isspace(*input))
				input++;
			if (*input)
				count++;
			continue ;
		}
		input++;
	}
	return (count + 1);
}

char	**tokenize_input(char *input)
{
	char	**tokens;
	int		count;
	int		i;
	int		len;
	int		in_quote;
	int		in_dquote;

	count = count_tokens(input);
	tokens = malloc(sizeof(char *) * (count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	while (i < count)
	{
		while (ft_isspace(*input))
			input++;
		len = 0;
		in_quote = 0;
		in_dquote = 0;
		while ((in_quote || in_dquote || !ft_isspace(input[len])) && input[len])
		{
			if (input[len] == '\'' && !in_dquote)
				in_quote = !in_quote;
			else if (input[len] == '\"' && !in_quote)
				in_dquote = !in_dquote;
			len++;
		}
		tokens[i] = ft_substr(input, 0, len);
		if (!tokens[i])
		{
			free_array(tokens);
			return (NULL);
		}
		input += len;
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}
