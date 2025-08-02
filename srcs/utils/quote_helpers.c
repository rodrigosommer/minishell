/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_inside_quotes(char *input, char *pos)
{
	int		in_single_quote;
	int		in_double_quote;
	char	*p;

	in_single_quote = 0;
	in_double_quote = 0;
	p = input;
	while (p < pos)
	{
		if (*p == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*p == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		p++;
	}
	return (in_single_quote || in_double_quote);
}
