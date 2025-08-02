/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*handle_input_redirect(char *input, int *i)
{
	if (input[*i + 1] == '<')
	{
		*i += 2;
		return (create_token(TOKEN_REDIRECT_HEREDOC, ft_strdup("<<")));
	}
	(*i)++;
	return (create_token(TOKEN_REDIRECT_IN, ft_strdup("<")));
}

static t_token	*handle_output_redirect(char *input, int *i)
{
	if (input[*i + 1] == '>')
	{
		*i += 2;
		return (create_token(TOKEN_REDIRECT_APPEND, ft_strdup(">>")));
	}
	(*i)++;
	return (create_token(TOKEN_REDIRECT_OUT, ft_strdup(">")));
}

t_token	*handle_redirect(char *input, int *i)
{
	if (input[*i] == '<')
		return (handle_input_redirect(input, i));
	else
		return (handle_output_redirect(input, i));
}

t_token	*handle_operators(char *input, int *i)
{
	if (input[*i] == '|')
	{
		if (input[*i + 1] == '|')
		{
			*i += 2;
			return (create_token(TOKEN_OR, ft_strdup("||")));
		}
		(*i)++;
		return (create_token(TOKEN_PIPE, ft_strdup("|")));
	}
	else
	{
		if (input[*i + 1] == '&')
		{
			*i += 2;
			return (create_token(TOKEN_AND, ft_strdup("&&")));
		}
		(*i)++;
		return (create_token(TOKEN_WORD, ft_strdup("&")));
	}
}

t_token	*process_token(char *input, int *i)
{
	if (input[*i] == '|' || input[*i] == '&')
	{
		return (handle_operators(input, i));
	}
	else if (input[*i] == '<' || input[*i] == '>')
	{
		return (handle_redirect(input, i));
	}
	else if (input[*i] == '(')
	{
		(*i)++;
		return (create_token(TOKEN_LPAREN, ft_strdup("(")));
	}
	else if (input[*i] == ')')
	{
		(*i)++;
		return (create_token(TOKEN_RPAREN, ft_strdup(")")));
	}
	else
	{
		return (create_token(TOKEN_WORD, get_word(input, i)));
	}
}
