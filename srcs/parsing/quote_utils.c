/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:04:22 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/26 17:04:24 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <stdbool.h>

static char	*expand_env_var(char *str, int *i, t_env *env)
{
	int		start;
	char	*var_name;
	char	*var_value;
	char	*before;
	char	*after;
	char	*result;
	char	*tmp;

	start = ++(*i);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
	{
		(*i)++;
	}
	var_name = ft_substr(str, start, *i - start);
	if (!var_name)
		return (NULL);
	var_value = get_env_value(env, var_name);
	free(var_name);
	if (!var_value)
		var_value = "";
	before = ft_substr(str, 0, start - 1);
	after = ft_strdup(str + *i);
	result = ft_strjoin(before, var_value);
	free(before);
	if (after)
	{
		tmp = ft_strjoin(result, after);
		free(result);
		free(after);
		result = tmp;
	}
	(*i) = start - 1 + ft_strlen(var_value);
	return (result);
}

char	*process_quotes_and_expand(char *token, t_env *env, int exit_status)
{
	char	*result;
	int		i;
	bool	in_single_quote;
	bool	in_double_quote;
	bool	escaped;
	char	*expanded;
	char	*exit_str;
	char	*new_result;
	int		j;
	char	*final;

	if (!token)
		return (NULL);
	result = ft_strdup(token);
	if (!result)
		return (NULL);
	i = 0;
	in_single_quote = false;
	in_double_quote = false;
	escaped = false;
	while (result[i])
	{
		if (!escaped && result[i] == '\\')
		{
			escaped = true;
			i++;
			continue ;
		}
		if (!escaped)
		{
			if (result[i] == '\'' && !in_double_quote)
			{
				in_single_quote = !in_single_quote;
			}
			else if (result[i] == '\"' && !in_single_quote)
			{
				in_double_quote = !in_double_quote;
			}
		}
		if (!escaped && !in_single_quote && result[i] == '$')
		{
			expanded = NULL;
			if (result[i + 1] == '?')
			{
				exit_str = ft_itoa(exit_status);
				new_result = ft_calloc(ft_strlen(result) + ft_strlen(exit_str),
						sizeof(char));
				if (new_result)
				{
					ft_strlcpy(new_result, result, i + 1);
					ft_strlcat(new_result, exit_str, ft_strlen(result)
						+ ft_strlen(exit_str));
					ft_strlcat(new_result, result + i + 2, ft_strlen(result)
						+ ft_strlen(exit_str));
					free(result);
					result = new_result;
					i += ft_strlen(exit_str) - 1;
				}
				free(exit_str);
			}
			else if (ft_isalpha(result[i + 1]) || result[i + 1] == '_')
			{
				j = i;
				expanded = expand_env_var(result, &j, env);
				if (expanded)
				{
					free(result);
					result = expanded;
					i = j;
					in_single_quote = false;
					in_double_quote = false;
					escaped = false;
					continue ;
				}
			}
		}
		escaped = false;
		i++;
	}
	final = ft_calloc(ft_strlen(result) + 1, sizeof(char));
	if (!final)
	{
		free(result);
		return (NULL);
	}
	j = 0;
	i = 0;
	in_single_quote = false;
	in_double_quote = false;
	escaped = false;
	while (result[i])
	{
		if (!escaped && result[i] == '\\')
		{
			escaped = true;
			if (in_single_quote)
			{
				final[j++] = result[i];
			}
			i++;
			continue ;
		}
		if (!escaped)
		{
			if (result[i] == '\'' && !in_double_quote)
			{
				in_single_quote = !in_single_quote;
				i++;
				continue ;
			}
			else if (result[i] == '\"' && !in_single_quote)
			{
				in_double_quote = !in_double_quote;
				i++;
				continue ;
			}
		}
		final[j++] = result[i];
		escaped = false;
		i++;
	}
	free(result);
	return (final);
}
