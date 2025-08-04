/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:38:42 by hjuma             #+#    #+#             */
/*   Updated: 2025/08/04 14:38:44 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_unquoted_len(char *str)
{
	int	i;
	int	len;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	len = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			len++;
		i++;
	}
	return (len);
}

static void	copy_unquoted(char *dest, char *src)
{
	int	i;
	int	j;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (src[i])
	{
		if (src[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (src[i] == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			dest[j++] = src[i];
		i++;
	}
	dest[j] = '\0';
}

int	has_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

char	*remove_quotes(char *str)
{
	char	*result;
	int		len;

	if (!str)
		return (NULL);
	len = count_unquoted_len(str);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	copy_unquoted(result, str);
	return (result);
}
