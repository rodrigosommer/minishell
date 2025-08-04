/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 14:38:11 by hjuma             #+#    #+#             */
/*   Updated: 2025/08/04 14:38:14 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_vars(char **env)
{
	int	count;

	count = 0;
	while (env[count])
		count++;
	return (count);
}

char	**add_env_var(char **env, char *var)
{
	int		count;
	char	*var_name;
	int		existing_index;

	var_name = get_var_name(var);
	if (!var_name)
		return (env);
	existing_index = find_env_var_index(env, var_name);
	free(var_name);
	if (existing_index >= 0)
	{
		free(env[existing_index]);
		env[existing_index] = ft_strdup(var);
		return (env);
	}
	count = count_env_vars(env);
	return (create_new_env(env, var, count));
}

char	**create_new_env(char **env, char *var, int count)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = ft_strdup(var);
	new_env[i + 1] = NULL;
	free_env(env);
	return (new_env);
}

char	*get_env_value(char *name, char **env)
{
	char	*env_var;
	char	*value;

	env_var = find_env_var(env, name);
	if (!env_var)
		return (NULL);
	value = ft_strchr(env_var, '=');
	if (!value)
		return (NULL);
	return (value + 1);
}
