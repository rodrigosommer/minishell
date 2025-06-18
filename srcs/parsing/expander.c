/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:21:47 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/18 13:41:44 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*expand_env_var(char *str, int *i, t_env *env)
{
	char	*var_name;
	char	*var_value;
	char	*result;
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
	result = ft_strjoin(var_value, str + *i);
	if (!result)
		return (NULL);
	return (result);
}

char	*expand_variables(char *token, t_env *env, int exit_status)
{
	char	*result;
	int		i;
	char	*tmp;
	char	*new;

	result = ft_strdup(token);
	if (!result)
		return (NULL);
	i = 0;
	while (result[i])
	{
		if (result[i] == '$' && result[i + 1] == '?')
		{
			tmp = ft_itoa(exit_status);
			if (!tmp)
			{
				free(result);
				return (NULL);
			}
			new = ft_strreplace(result, "$?", tmp);
			free(tmp);
			free(result);
			if (!new)
				return (NULL);
			result = new;
			i = 0;
		}
		else if (result[i] == '$' && (ft_isalpha(result[i + 1]) || result[i
					+ 1] == '_'))
		{
			token = expand_env_var(result, &i, env);
			free(result);
			if (!token)
				return (NULL);
			result = token;
		}
		else
			i++;
	}
	if (result && !ft_strcmp(result, token))
	{
		tmp = ft_strdup(result);
		free(result);
		return (tmp);
	}
	return (result);
}
