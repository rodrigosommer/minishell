/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name_from_str(char *str, int *i)
{
	int		start;
	char	*name;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	return (name);
}

char	*expand_var(char *name, t_shell *shell)
{
	char	*value;

	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(shell->last_exit_status));
	value = get_env_value(name, shell->env);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

char	*expand_variables(char *str, t_shell *shell)
{
	char	*result;
	char	*temp;
	char	*addition;
	int		i;

	if (!str || !shell)
	{
		if (str)
			return (ft_strdup(str));
		return (ft_strdup(""));
	}
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		addition = process_char_with_quotes(str, &i, shell);
		temp = ft_strjoin(result, addition);
		free(result);
		free(addition);
		result = temp;
	}
	return (result);
}
