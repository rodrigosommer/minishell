/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:38:34 by hjuma             #+#    #+#             */
/*   Updated: 2025/08/04 14:38:36 by hjuma            ###   ########.fr       */
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
