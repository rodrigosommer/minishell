/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:47 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:21:59 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*expand_env_var(char *str, int *i, t_env *env)
{
	char	*var_name;
	char	*var_value;
	char	*result;
	char	*before;
	char	*after;
	char	*tmp;
	int		start;

	start = ++(*i);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
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
	*i = start - 1;
	return (result);
}

char	*expand_variables(char *token, t_env *env, int exit_status)
{
	char	*result;
	int		i;
	int		in_squote;
	int		in_dquote;
	int		escaped;
	char	*exit_str;
	char	*new;
	char	*expanded;

	if (!token)
		return (NULL);
	result = ft_strdup(token);
	if (!result)
		return (NULL);
	i = 0;
	in_squote = 0;
	in_dquote = 0;
	escaped = 0;
	while (result[i])
	{
		if (!escaped && result[i] == '\\')
		{
			escaped = 1;
			i++;
			continue ;
		}
		if (!escaped)
		{
			if (result[i] == '\'' && !in_dquote)
				in_squote = !in_squote;
			else if (result[i] == '\"' && !in_squote)
				in_dquote = !in_dquote;
		}
		if (!in_squote && !escaped && result[i] == '$')
		{
			if (result[i + 1] == '?')
			{
				exit_str = ft_itoa(exit_status);
				new = ft_strreplace(result, "$?", exit_str);
				free(exit_str);
				free(result);
				result = new ? new : ft_strdup("");
				if (!result)
					return (NULL);
				i = -1;
				in_squote = 0;
				in_dquote = 0;
			}
			else if (ft_isalpha(result[i + 1]) || result[i + 1] == '_')
			{
				expanded = expand_env_var(result, &i, env);
				free(result);
				result = expanded;
				if (!result)
					return (NULL);
				i = -1;
				in_squote = 0;
				in_dquote = 0;
			}
		}
		escaped = 0;
		i++;
	}
	return (result);
}
