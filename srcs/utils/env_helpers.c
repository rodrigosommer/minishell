/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:00:00 by rsommer           #+#    #+#             */
/*   Updated: 2025/01/31 00:00:00 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *env_var)
{
	int		i;
	char	*name;

	i = 0;
	while (env_var[i] && env_var[i] != '=')
		i++;
	name = ft_substr(env_var, 0, i);
	return (name);
}

int	find_env_var_index(char **env, char *var_name)
{
	int		i;
	char	*existing_name;

	i = 0;
	while (env[i])
	{
		existing_name = get_var_name(env[i]);
		if (existing_name && ft_strcmp(existing_name, var_name) == 0)
		{
			free(existing_name);
			return (i);
		}
		free(existing_name);
		i++;
	}
	return (-1);
}

char	*find_env_var(char **env, char *name)
{
	int		i;
	char	*var_name;

	i = 0;
	while (env[i])
	{
		var_name = get_var_name(env[i]);
		if (var_name && ft_strcmp(var_name, name) == 0)
		{
			free(var_name);
			return (env[i]);
		}
		free(var_name);
		i++;
	}
	return (NULL);
}
